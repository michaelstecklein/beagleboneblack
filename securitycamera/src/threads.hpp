#ifndef _threads_h_
#define _threads_h_

#include <stdint.h>


/*
 * The TAG_NEW tag is tagged to all captures that have yet to been
 * looked at by motionDetect. The newTagCount is used to determine
 * the age of a capture.
 */
#define TAG_NEW		"NEW%i"
extern volatile uint32_t newTagCount;

/*
 * The TAG_NO_UPLOAD tag is identifies captures that need to be uploaded.
 */
#define TAG_NO_UPLOAD	"NOUPLOAD"

/*
 * Captures from the USB webcam.
 */
void * capture(void*);

/*
 * Polls for tagged captures and checks for motion, either untagging
 * or deleting if no motion is detected.
 */
//void * motiondetect(void*);

/*
 * Uploads untagged captures to the cloud.
 */
void * upload(void*);

#endif
