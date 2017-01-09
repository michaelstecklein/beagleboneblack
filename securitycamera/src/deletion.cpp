#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <queue>
#include <sys/statvfs.h>
#include "motiondetect.hpp"
#include "filelog.hpp"
#include "deletion.hpp"


#define TIMEOUT 1000000000 // 1 second
#define CLEANUP_THRESHHOLD 100000//25000000 // in KB. If #imgs*imgsize > this, will start clean up
#define CLEANUP_UNTIL_THRESHHOLD 50000//20000000 // in KB. If #imgs*imgsize < this, will stop clean up

typedef struct deletion_entry {
	char* fname;
	long entry_time;
} deletion_entry_t;

std::queue<deletion_entry_t> q;
volatile int queue_lock = 0; // 0 is unlocked, 1 locked
#define Q_LOCK()	while(queue_lock){} queue_lock=1;
#define Q_UNLOCK()	queue_lock=0;

/*void Q_LOCK(void) {
	long waitStartTime = get_nanos();
	while (queue_lock) {
		if (get_nanos() - waitStartTime > 50000000) {
			system("echo 'Stuck in Q_LOCK' >> error.log");
			printf("STUCK HEREEEE\n");
		}
	}
	queue_lock = 1;
}*/

long last_full_check = 0;

void deleteImgs(long long timeTag) {
	for (int i = 0; i < 5; i++) {
		char name[100];
		sprintf(name, "%s/img_%llu_%i.jpeg", RAW_CAPTURE_DIR, timeTag, i);
		deletion_entry_t de;
		de.fname = (char*) malloc(strlen(name) + 1);
		de.entry_time = get_nanos();
		strcpy(de.fname, name);
		Q_LOCK();
		q.push(de);
		Q_UNLOCK();
	}
}

int getImgSpace() {
	return IMG_SIZE * getImgCount();
}

void cleanUp() {
	char com[100];
	sprintf(com, "echo 'Drive getting full, cleaning up, time tag: %llu' >> activity.log", curr_time_tag);
	system(com);

	while (getImgSpace() > CLEANUP_UNTIL_THRESHHOLD) {
		const int groupSize = 50; // num files deleted at once
		char bufArr[groupSize][MAX_LINE_SIZE];
		popTopFiles(bufArr, groupSize);
		int numRemoved = 0;
		for (int i = 0; i < groupSize; i++) {
			if (remove(bufArr[i]) < 0) {
				char com[100];
				sprintf(com, "echo 'Error cleaning up %s' >> error.log", bufArr[i]);
				system(com);
			} else {
				numRemoved++;
			}
		}
	}

	char com2[100];
	sprintf(com2, "echo 'Clean up completed successfully, time tag: %llu' >> activity.log", curr_time_tag);
	system(com2);
}

void checkForFullDrive() {
	if (getImgSpace() > CLEANUP_THRESHHOLD)
		cleanUp();
}

void * deletionThread(void*) {
	while (1) {
		if (last_full_check == 0 || get_nanos() - last_full_check > TIMEOUT) {
			checkForFullDrive();
			last_full_check = get_nanos();
		}
		Q_LOCK();
		if (q.empty()) {
			Q_UNLOCK();
			sleep(1);
		} else {
			std::queue<deletion_entry_t> tmp_q;
			while (!q.empty()) {
				deletion_entry_t de = q.front();
				q.pop();
				Q_UNLOCK();
				int numRemoved = 0;
				if (remove(de.fname) < 0) {
					if (get_nanos() - de.entry_time > TIMEOUT) {
						char com[100];
						sprintf(com, "echo 'Error deleting %s' >> error.log", de.fname);
						system(com);
						free(de.fname);
					} else {
						/*Q_LOCK();
						q.push(de);
						Q_UNLOCK();*/
						tmp_q.push(de);
						// leaks in image count
					}
				} else {
					free(de.fname);
					numRemoved++;
				}
				Q_LOCK();
			}
			Q_UNLOCK();

			Q_LOCK();
			while (!tmp_q.empty()) {
				q.push(tmp_q.front());
				tmp_q.pop();
			}
			Q_UNLOCK();
		}
	}
}
