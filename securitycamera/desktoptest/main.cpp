#include <stdio.h>
#include "opencv2/opencv.hpp"

using namespace cv;





double compare(Mat newMat, Mat oldMat) {

	if (newMat.rows != oldMat.rows || newMat.cols != oldMat.cols || newMat.step != oldMat.step)
		return -1;
	uint8_t* newData = newMat.data;
	uint8_t* oldData = oldMat.data;
	int width = newMat.cols;
	int height = newMat.rows;
	int stride = newMat.step;

	Mat diffMat = Mat (height, width, CV_8UC1);
	int diffThreshhold = 15;
 
	int numDiffs = 0;
	int totalDiffs = 0;
	int totalDiffsSqr = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			char newVal = newData[i*stride + j];
			char oldVal = oldData[i*stride + j];
			numDiffs++;
			int diff = newVal - oldVal;
			if (diff >= diffThreshhold)
				diffMat.data[i*stride + j] = diff;
			else
				diffMat.data[i*stride + j] = 0;
			if (diff < 0)
				totalDiffs -= diff;
			else
				totalDiffs += diff;
			totalDiffsSqr += diff * diff;
		}
	}

	return 0;
}

int main(void) {

	Mat temp;

Mat image;
 image = imread( "pics/b_me.jpeg", 1 );

 if( !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 Mat gray_image;
 cvtColor( image, gray_image, CV_BGR2GRAY );

 imwrite( "Gray_Image.jpg", gray_image );

}
