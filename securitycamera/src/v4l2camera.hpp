/*
	Camera Specs: 

	Sensor:OV9712 
	Picture format "YUY2 and MJPEG optional"/USB Protocal USB2.0 HS/FS 
	Auto exposure AEC Support/Auto white blance AEB Support 
	Performance: 
	1280 (H) x 720 (V) pixels MJPEG 30fps YUY2 10fps 
	960 (H) x 720 (V) pixels MJPEG 30fps YUY2 15fps 
	800 (H) x 600 (V) pixels MJPEG 30fps YUY2 20fps 
	640 (H) x 480 (V) pixels MJPEG 30fps YUY2 30fps 
	Adjustable parameters Brightness/Contrast/Color saturation /Definition/Gamma/WB 
	with IR Cut and IR board for IR night vision. 
	Support OS "WinXP/Vista/WIN7/WIN8/Linux with UVC/Mac-OS X 10.4.8 or later/Wince with UVC/Android 4.0 or above"
*/

#ifndef _v4l2camera_h_
#define _v4l2camera_h_

void setCameraResolution(int width, int height);

#endif
