#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "motiondetect.hpp"
#include "filelog.hpp"

#define LOG_FILE_NAME	"files.log"
#define TMP_FILE_NAME	"TEMP_FILE.log"


void incImgCnt(int n); // call before updating files.log
void decImgCnt(int n); // call before updating files.log


void logFile(const char* name) {
	char com[100];
	sprintf(com, "echo '%s' >> %s/%s", name, RAW_CAPTURE_DIR, LOG_FILE_NAME);
	incImgCnt(1);
	system(com);
}

void logImgs(long long timeTag) {
	for (int i = 0; i < 5; i++) {
		char name[80];
		sprintf(name, "%s/img_%llu_%i.jpeg", RAW_CAPTURE_DIR, timeTag, i);
		logFile(name);
	}
}

void topFile(char* buf) {
	if (buf == NULL)
		return;

	FILE *f;
	char line[MAX_LINE_SIZE];

	char filename[100];
	sprintf(filename, "%s/%s", RAW_CAPTURE_DIR, LOG_FILE_NAME);
	f = fopen(filename, "r");

	if (fgets(line, MAX_LINE_SIZE, f) != NULL) {
		line[strlen(line)-1] = 0; // remove newline at end
		int maxLen = strlen(line);
		if (strlen(buf) > strlen(line))
			maxLen = strlen(buf);
		strncpy(buf, line, maxLen);
	} else {
		char com[100];
		sprintf(com, "echo 'End of file reached in topFile: %s' >> error.log", filename);
		system(com);
	}

	fclose(f);
}

void removeNLines(int n) {
	// remove first n lines of log file
	char com[100];
	sprintf(com, "tail -n +%i %s/%s > %s; mv %s %s/%s;", n+1, RAW_CAPTURE_DIR, LOG_FILE_NAME, TMP_FILE_NAME, TMP_FILE_NAME, RAW_CAPTURE_DIR, LOG_FILE_NAME);
	system(com);
}

void popTopFile(char* buf) {
	topFile(buf);
	decImgCnt(1);
	removeNLines(1);
}

void popTopFiles(char bufArr[][MAX_LINE_SIZE], int numFiles) {
	if (bufArr == NULL)
		return;

	FILE *f;
	char line[MAX_LINE_SIZE];

	char filename[100];
	sprintf(filename, "%s/%s", RAW_CAPTURE_DIR, LOG_FILE_NAME);
	f = fopen(filename, "r");

	for (int i = 0; i < numFiles; i++) {
		if (bufArr[i] == NULL)
			return;
		if (fgets(line, MAX_LINE_SIZE, f) != NULL) {
			line[strlen(line)-1] = 0; // remove newline at end
			int maxLen = strlen(line);
			if (strlen(bufArr[i]) > strlen(line))
				maxLen = strlen(bufArr[i]);
			strncpy(bufArr[i], line, maxLen);
		} else {
			char com[100];
			sprintf(com, "echo 'End of file reached: %s' >> error.log", filename);
			system(com);
		}
	}

	fclose(f);

	decImgCnt(numFiles);
	removeNLines(numFiles);
}









volatile int imageCount = -1;


int fileLogLineCnt() {
	int numLines = 0;
	FILE *f;
	char line[MAX_LINE_SIZE];
	char filename[100];
	sprintf(filename, "%s/%s", RAW_CAPTURE_DIR, LOG_FILE_NAME);
	f = fopen(filename, "r");
	if (f == NULL)
		return 0;
	while (fgets(line, MAX_LINE_SIZE, f) != NULL) {
		numLines++;
	}
	fclose(f);
	return numLines;
}

int getImgCount() {
	if (imageCount < 0) {
		imageCount = fileLogLineCnt();
	}
	return imageCount;
}


void incImgCnt(int n) {
	if (imageCount < 0) {
		imageCount = fileLogLineCnt();
	}
	imageCount += n;
}

void decImgCnt(int n) {
	if (imageCount < 0) {
		imageCount = fileLogLineCnt();
	}
	imageCount -= n;
}


