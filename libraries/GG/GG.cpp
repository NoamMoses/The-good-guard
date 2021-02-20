/*
 * Created by Noam
 * Project: The good guard
 * Library name: GG (Good Guard)
 * Version: debug
 * File: GG.cpp
*/

#include "GG.h"
#include <Servo.h>

GG::GG() {
	
}

/*
 * Get & Set functions for time - when an object passed the sensor
*/
int GG::getT1() {
	return this->t1;
}

int GG::getT2() {
	return this->t2;
}

void GG::setT1(int val) {
	this->t1 = val;
}

void GG::setT2(int val) {
	this->t2 = val;
}


void GG::openGate(Servo motor, int delayTime) {
	/*
	 * Parameters:
		motor - the servo motor, gate
		delayTime - how fast to open the gate. the lower the number, the faster the gate will open
	 * Opens the gate with a small delay, set by the parameter
	*/
	int i = 0, min = 0, max = 90;
	for (i = 0; i < 90; i+=10) { // Opens the gate 10 degrees at a time - to make it fast enough. Should be fine with a fixed value
		motor.write(i);
		delay(delayTime);
	}
}

void GG::printObjFound(String objName, float distance, float p0, float p0mm) {
	/*
	 * Just a helper function, to save lines in the main function (Arduino code)
	*/
	char txt[200] = {0}; // Buffer
	sprintf(txt, "%s has been found, at %s and p0 is %s and p0_mm is %s\n",
		String(objName).c_str(),
		String(distance).c_str(),
		String(p0).c_str(),
		String(p0mm).c_str());
	Serial.println(txt);
}









/*
Serial.print("Object1 has been found, at ");
Serial.print(distance1);
Serial.print(" and p0 is ");
Serial.print(v1.getP0());
Serial.print(" and p0_mm is ");
Serial.println(v1.getP0_mm());
*/
