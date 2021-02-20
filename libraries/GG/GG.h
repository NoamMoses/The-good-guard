/*
 * Created by Noam
 * Project: The good guard
 * Library name: GG (Good Guard)
 * Version: debug
 * File: GG.h
*/

#include <Arduino.h>
#include <Servo.h>
#define VERSION "1.0"

class GG {
	private:
		int echo;
	public:
		GG();
		
		/* Sensor object time recognised functions */
		int t1, // Point in time when sensor 1 found an Object
			t2; // Same for sensor 2
		int getT1();
		int getT2();
		void setT1(int val);
		void setT2(int val);
		void openGate(Servo motor, int delayTime);
		void printObjFound(String objName, float distance, float p0, float p0mm);
};