#include <stdlib.h>
#include <stdio.h>
#include "v4l2camera.hpp"


#define V4L2_LIST_FORMATS		"v4l2-ctl --list-formats-ext"
#define V4L2_LIST_CAM_SPECS		"v4l2-ctl -V"
#define V4L2_SET_WIDTHHEIGHTFORMAT	"v4l2-ctl --set-fmt-video=width=%i,height=%i,pixelformat=0" // pixelformat: 0 - MJPG, 1 - YUYV

void setCameraResolution(int width, int height) {
	char str[200];
	sprintf(str, V4L2_SET_WIDTHHEIGHTFORMAT, width, height);
	system(str);
	//system(V4L2_LIST_FORMATS); system(V4L2_LIST_CAM_SPECS);
}
