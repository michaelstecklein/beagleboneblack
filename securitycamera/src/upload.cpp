#include <time.h>
#include "led.hpp"


void * upload(void*) {

	struct timespec time, time_ret;
	time.tv_sec = 0;
	time.tv_nsec = 100000000L; // 1 sec

	while (1) {
		
		/*turnOnLED2();	
		nanosleep(&time, &time_ret);
		turnOffLED2();
		nanosleep(&time, &time_ret);*/

	}
	
}
