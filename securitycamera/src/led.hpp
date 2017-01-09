#ifndef _led_h_
#define _led_h_

#include <iostream>
#include <fstream>
#include <stdbool.h>

#define LED0_PATH	"/sys/class/leds/beaglebone:green:usr0"
#define LED1_PATH	"/sys/class/leds/beaglebone:green:usr1"
#define LED2_PATH	"/sys/class/leds/beaglebone:green:usr2"
#define LED3_PATH	"/sys/class/leds/beaglebone:green:usr3"


extern bool LED0_ON;
extern bool LED1_ON;
extern bool LED2_ON;
extern bool LED3_ON;


inline void turnOffLED(int ledNum) {
	std::fstream fs1;
	std::fstream fs2;
	switch (ledNum) {
	case 0:
		fs1.open(LED0_PATH"/trigger", std::fstream::out);
		fs2.open(LED0_PATH"/brightness", std::fstream::out);
		LED0_ON = false;
		break;
	case 1:
		fs1.open(LED1_PATH"/trigger", std::fstream::out);
		fs2.open(LED1_PATH"/brightness", std::fstream::out);
		LED1_ON = false;
		break;
	case 2:
		fs1.open(LED2_PATH"/trigger", std::fstream::out);
		fs2.open(LED2_PATH"/brightness", std::fstream::out);
		LED2_ON = false;
		break;
	case 3:
		fs1.open(LED3_PATH"/trigger", std::fstream::out);
		fs2.open(LED3_PATH"/brightness", std::fstream::out);
		LED3_ON = false;
		break;
	default:
		return;
	}
	fs1 << "none";
	fs2 << "0";
	fs1.close();
	fs2.close();
}

inline void turnOnLED(int ledNum) {
	std::fstream fs;
	switch (ledNum) {
	case 0:
		fs.open(LED0_PATH"/brightness", std::fstream::out);
		LED0_ON = true;
		break;
	case 1:
		fs.open(LED1_PATH"/brightness", std::fstream::out);
		LED1_ON = true;
		break;
	case 2:
		fs.open(LED2_PATH"/brightness", std::fstream::out);
		LED2_ON = true;
		break;
	case 3:
		fs.open(LED3_PATH"/brightness", std::fstream::out);
		LED3_ON = true;
		break;
	default:
		return;
	}
	fs << "1";
	fs.close();
}

inline void turnOffLED0()	{ turnOffLED(0); }
inline void turnOnLED0()	{ turnOnLED(0); }

inline void turnOffLED1()	{ turnOffLED(1); }
inline void turnOnLED1()	{ turnOnLED(1); }

inline void turnOffLED2()	{ turnOffLED(2); }
inline void turnOnLED2()	{ turnOnLED(2); }

inline void turnOffLED3()	{ turnOffLED(3); }
inline void turnOnLED3()	{ turnOnLED(3); }

inline void toggleLED0() {
	if (LED0_ON)
		turnOffLED0();
	else
		turnOnLED0();
}

inline void toggleLED1() {
	if (LED1_ON)
		turnOffLED1();
	else
		turnOnLED1();
}

inline void toggleLED2() {
	if (LED2_ON)
		turnOffLED2();
	else
		turnOnLED2();
}

inline void toggleLED3() {
	if (LED3_ON)
		turnOffLED3();
	else
		turnOnLED3();
}

inline void turnOffAllLEDs() {
	turnOffLED(0);
	turnOffLED(1);
	turnOffLED(2);
	turnOffLED(3);
}

inline void turnOnAllLEDs() {
	turnOnLED(0);
	turnOnLED(1);
	turnOnLED(2);
	turnOnLED(3);
}

#endif
