#include <stdio.h>
#include "opencv2/opencv.hpp"
#include "led.hpp"
#include "motiondetect.hpp"
#include "deletion.hpp"
#include "filelog.hpp"
using namespace cv;

volatile unsigned long long curr_time_tag = 0;

#define VARIANCE_THRESHHOLD	30


void* thisBuf;
volatile int buff_len = -1;
volatile int ready_flag = -2; // +/-2 means first pass hasn't occured, -1 means not ready, 1 ready

long get_nanos(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

double compare(Mat newMat, Mat oldMat) {
	int totalDiffs = 0;
	double numDiffs = 0;
	double totalDiffsSqr = 0;
	int i = 0;
	if (newMat.rows != oldMat.rows || newMat.cols != oldMat.cols || newMat.step != oldMat.step)
		return -1;
	uint8_t* newData = newMat.data;
	uint8_t* oldData = oldMat.data;
	int width = newMat.cols;
	int height = newMat.rows;
	int stride = newMat.step;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			char newVal = newData[i*stride + j];
			char oldVal = oldData[i*stride + j];
			numDiffs++;
			int diff = newVal - oldVal;
			if (diff < 0)
				totalDiffs -= diff;
			else
				totalDiffs += diff;
			totalDiffsSqr += diff * diff;
		}
	}
	double variance = (totalDiffsSqr - (totalDiffs*totalDiffs)/numDiffs) / numDiffs; // Var = Mean(x^2) - (Mean(x))^2
	printf("Var: %f\n", variance);
	if (variance > VARIANCE_THRESHHOLD) {
		printf("MOTION DETECTED!\n");
		toggleLED0();
	}
	return variance - VARIANCE_THRESHHOLD;
}


void MD_createBuffers(int len) {
	thisBuf = malloc(len);
	buff_len = len;
}


void MD_detectMotion(void* newBuf) {
	memcpy(thisBuf, newBuf, buff_len);
	while (ready_flag > 0) {} // block if last compare isn't done
	ready_flag *= -1;
}


Mat decodedOld;
Mat rawNew;
Mat decodedNew;

void analyzeForMotion() {
	// Mat's create error : Corrupt JPEG data: XX extraneous bytes before marker 0xXX
	//	there's nothing I can do about it
	rawNew = Mat (1, buff_len, CV_8UC1, (void*)thisBuf);
	decodedNew  =  imdecode(rawNew, 1);
	if (ready_flag == 2) { // first time
		deleteImgs(curr_time_tag);
		ready_flag--;
	} else if (compare(decodedNew, decodedOld) < 0) {
		deleteImgs(curr_time_tag);
	} else {
		logImgs(curr_time_tag);
	}
	decodedOld = decodedNew;
}





void* MD_main(void*) {

	// block while buffers aren't created
	while (buff_len < 0) {}

	while (1) {
		// block waiting for ready flag
		while (ready_flag < 0) {}
		
		analyzeForMotion();
		ready_flag *= -1;
	}
}
