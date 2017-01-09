/*
	TODO:
	- finish upload to GoogleDrive
	- increase robustness of motion detection threshhold, it doesn't seem to always pick up motion... (or change algorithm all together) (maybe don't look at every pixel, but skip several at a time)


	+ program freezes ocassionally (IDK what do to about this, isn't a huge issue...)
*/



#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h> // for sleep
#include <pthread.h>
#include "opencv2/opencv.hpp"
#include "v4l2camera.hpp"
#include "led.hpp"
#include "capture.hpp"
#include "motiondetect.hpp"
#include "deletion.hpp"
#include "filelog.hpp"


using namespace std;
using namespace cv;




int main(int, char**)
{
	turnOffAllLEDs();
	system("rm *.log");

	// maybe should clean up media/ using files.log here?

	pthread_t capture_thread, motiondetect_thread, deletion_thread, upload_thread;

	pthread_create(&capture_thread, NULL, capture, NULL);
	pthread_create(&motiondetect_thread, NULL, MD_main, NULL);
	//pthread_create(&deletion_thread, NULL, deletionThread, NULL);
	//pthread_create(&upload_thread, NULL, upload, NULL);

	// main exits and leaves threads to work
	pthread_exit(NULL);
}
