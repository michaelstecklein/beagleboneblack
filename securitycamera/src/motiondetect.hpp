#ifndef _motiondetect_h_
#define _motiondetect_h_


#define RAW_CAPTURE_DIR	"media"

extern volatile unsigned long long curr_time_tag;

long get_nanos(void);

void* MD_main(void*);

void MD_createBuffers(int len);

void MD_detectMotion(void* newBuff);


#endif
