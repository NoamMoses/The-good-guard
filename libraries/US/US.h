/*
 * Created by Noam
 * Project: The good guard
 * Library name: US (Ultrasonic)
 * Version: debug
 * File: US.h
*/

#include <Arduino.h>
#define NUM_OF_AVG 10
#define STREAK_OK 88
#define STREAK_REQ 1
#define SENS 5

class US {
	private:
		int echo, // Echo pin
			trig, // Trigger pin
			minDistance, // Distance of object recognitions
			streak; // How many times are passed, need to resed every time there is a different value
		float distances[NUM_OF_AVG];
		float p0;
		float p0mm;
		/* Functions */
		void findP0();
		void findP0_mm();
		void get10dst();
	public:
		US(int echo, int trig);
		float getDistance();
		void init();
		void setSensitivity(int sensitivity);
		int searchObj(float distance);
		/* Get & Set functions */
		void setStreak(int val);
		float getP0();
		float getP0_mm();
};