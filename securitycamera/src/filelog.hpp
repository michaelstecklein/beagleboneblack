#ifndef _filelog_h_
#define _filelog_h_


#define MAX_LINE_SIZE	100

void logImgs(long long timeTag);

//void logFile(const char* name);

//void topFile(char* buf);

//void popTopFile(char* buf);

void popTopFiles(char bufArr[][MAX_LINE_SIZE], int numFiles);


#define IMG_SIZE	615 // kilibytes

int getImgCount(void);

#endif
