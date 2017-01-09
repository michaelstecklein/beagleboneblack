#include <linux/videodev2.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <unistd.h> // for sleep
#include <pthread.h>
#include "opencv2/opencv.hpp"
#include "v4l2camera.hpp"
#include "led.hpp"
#include "motiondetect.hpp"
#include "capture.hpp"



using namespace std;
using namespace cv;


#define CAM_WIDTH	1280/2
#define CAM_HEIGHT	720/2
#define PICS_BTW_COMPARES	5
#define FILENAME_SPRINTF	sprintf(name, "%s/img_%llu_%i.jpeg", RAW_CAPTURE_DIR, curr_time_tag, i);





void handleCompare(void* start, int len);




struct tm lastTime;
int subdivisor = 0;
long long getCurrTimeTag(void) {
	time_t t = time(0); // get curr time
	struct tm * now = localtime(&t);

	if (lastTime.tm_sec==now->tm_sec && lastTime.tm_min==now->tm_min && lastTime.tm_hour==now->tm_hour) {
		subdivisor++;
		if (subdivisor >= 100)
			subdivisor = 0;
	} else {
		subdivisor = 0;
	}

	long long timeTag = 0;
	timeTag += (now->tm_year + 1900) 	* 1000000000000; // 4
	timeTag += (now->tm_mon + 1) 		* 10000000000; // 2
	timeTag += (now->tm_mday)		* 100000000; // 2
	timeTag += (now->tm_hour)		* 1000000; // 2
	timeTag += (now->tm_min)		* 10000; // 2
	timeTag += (now->tm_sec)		* 100; // 2
	timeTag += subdivisor;			    // 2

	lastTime = *now;
	return timeTag;
}




void * capture(void*) {
	
	setCameraResolution(CAM_WIDTH, CAM_HEIGHT);

	mkdir(RAW_CAPTURE_DIR, 0777);


	int fd;
	if((fd = open("/dev/video0", O_RDWR)) < 0){
		perror("open");
		printf("open\n");
		exit(1);
	}
	 
	struct v4l2_capability cap;
	if(ioctl(fd, VIDIOC_QUERYCAP, &cap) < 0){
	    perror("VIDIOC_QUERYCAP");
	    printf("VIDIOC_QUERYCAP\n");
	    exit(1);
	}
	if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) || !(cap.capabilities & V4L2_CAP_STREAMING)){
	    fprintf(stderr, "The device does not handle video capture and streaming.\n");
	    printf("The device does not handle video capture and streaming.\n");
	    exit(1);
	}

	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = CAM_WIDTH;
	format.fmt.pix.height = CAM_HEIGHT;
	 
	if(ioctl(fd, VIDIOC_S_FMT, &format) < 0){
	    perror("VIDIOC_S_FMT");
	    printf("VIDIOC_S_FMT\n");
	    exit(1);
	}


	struct v4l2_requestbuffers bufrequest;
	bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufrequest.memory = V4L2_MEMORY_MMAP;
	bufrequest.count = 1;
	 
	if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0){
	    perror("VIDIOC_REQBUFS");
	    printf("VIDIOC_REQBUFS\n");
	    exit(1);
	}


	struct v4l2_buffer bufferinfo;
	memset(&bufferinfo, 0, sizeof(bufferinfo));
	 
	bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufferinfo.memory = V4L2_MEMORY_MMAP;
	bufferinfo.index = 0;
	 
	if(ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo) < 0){
	    perror("VIDIOC_QUERYBUF");
	    printf("VIDIOC_QUERYBUF\n");
	    exit(1);
	}

	void* buffer_start = mmap(
	    NULL,
	    bufferinfo.length,
	    PROT_READ | PROT_WRITE,
	    MAP_SHARED,
	    fd,
	    bufferinfo.m.offset
	);
	 
	if(buffer_start == MAP_FAILED){
	    perror("mmap");
	    printf("mmap\n");
	    exit(1);
	}
	 
	memset(buffer_start, 0, bufferinfo.length);

	// Activate streaming
	int type = bufferinfo.type;
	if(ioctl(fd, VIDIOC_STREAMON, &type) < 0){
	    perror("VIDIOC_STREAMON");
	    printf("VIDIOC_STREAMON\n");
	    exit(1);
	}

	MD_createBuffers(bufferinfo.length);
	 
	/* Here is where you typically start two loops:
	 * - One which runs for as long as you want to
	 *   capture frames (shoot the video).
	 * - One which iterates over your buffers everytime. */
	long lastT;
	while (1) {
		curr_time_tag = getCurrTimeTag();
		int i;
		for (i = 0; i < PICS_BTW_COMPARES; i++) {
			 
			// Put the buffer in the incoming queue.
			if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
			    perror("VIDIOC_QBUF");
			    printf("VIDIOC_QBUF\n");
			    exit(1);
			}
			 
			// The buffer's waiting in the outgoing queue.
			if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
			    perror("VIDIOC_QBUF");
			    perror("VIDIOC_QBUF\n");
			    exit(1);
			}

			if (i == 0) {
				long mdTime = get_nanos();
				MD_detectMotion(buffer_start);
				//printf("MD_detectMotion time : %i\n", get_nanos()-mdTime);
			}

			char name[80];
			FILENAME_SPRINTF

			int jpgfile;
			if((jpgfile = open(name, O_WRONLY | O_CREAT, 0660)) < 0){
			    perror("open");
			    printf("open\n");
			    exit(1);
			}

			write(jpgfile, buffer_start, bufferinfo.length);
			close(jpgfile);

			printf("Time : %i\n", get_nanos()-lastT);
			lastT = get_nanos();

		}
/*char command[100];
time_t rawtime;
struct tm * timeinfo;
time ( &rawtime );
timeinfo = localtime ( &rawtime );
sprintf(command, "echo 'curr_time_tag: %llu    time: %s' > curr_time_tag.log", curr_time_tag, asctime(timeinfo));
system(command);*/
	}
	 
	// Deactivate streaming
	if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
	    perror("VIDIOC_STREAMOFF");
	    perror("VIDIOC_STREAMOFF\n");
	    exit(1);
	}

	close(fd);
}



