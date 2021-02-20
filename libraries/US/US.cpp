/*
 * Created by Noam
 * Project: The good guard
 * Library name: US (Ultrasonic)
 * Version: debug
 * File: US.cpp
*/


#include "US.h"

US::US(int trig, int echo) {
	/*
	 * Constructor, set trig and echo pins
	*/
	this->trig = trig;
	this->echo = echo;
}

void US::init() {
	/*
	 * set pin modes for US sensor
	*/
	pinMode(trig, OUTPUT);
	pinMode(echo, INPUT);
	this->findP0();
	this->findP0_mm();
}

void US::setSensitivity(int sensitivity) {
	/*
	 * Set the sensitivity for a sensor - in cm
	*/
	this->minDistance = sensitivity;
}

float US::getDistance() {
	/*
	 * Reads the distance in cm
	*/
	float distance = 0.0, duratation = 0.0;
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	digitalWrite(echo, HIGH);
	duratation = pulseIn(echo, HIGH);
	distance = duratation * 0.034 / 2;
	return distance;
}

void US::get10dst() {
	/*
	 * Reads 10 distance calls and saves them into a class variable
	*/
	int time = 50, // Time to wait before each distance call
		i = 0;
	float currentDistance = 0.0;
	for (i = 0; i < NUM_OF_AVG; i++) {
		currentDistance = this->getDistance(); // Just to make it easier to read
		this->distances[i] = currentDistance;
		Serial.print("Distance");
		Serial.print(i);
		Serial.print(": ");
		Serial.println(currentDistance);
		delay(time);
	}
}

void US::findP0() {
	/*
	 * Getting point 0, where the "wall" is - the border of the reading range
	*/
	float average = 0.0, total = 0.0;
	int i = 0;
	this->get10dst();
	for (i = 0; i < NUM_OF_AVG; i++) { // Init array of distances
		total += this->distances[i];
	}
	average = total/NUM_OF_AVG;
	Serial.print("Average: ");
	Serial.println(average);
	this->p0 = average; // Saves it as a class variable instead of returning
}

void US::findP0_mm() {
	/*
	 * Same like findPo, but without the smallest and largest numbers
	 * mm - min max
	*/
	int i = 0, skipped = 0;
	float min = 1000000000000.0, max = 0.0, total = 0.0, average = 0.0;
	float arr[NUM_OF_AVG] = {0.0};
	
	for (i = 0; i < NUM_OF_AVG; i++) {
		arr[i] = this->distances[i];
	}
	for (i = 0; i < NUM_OF_AVG; i++) { // Find min and max
		if (arr[i] > max) {
			max = arr[i];
		}
		if (arr[i] < min) {
			min = arr[i];
		}
	}
	for (i = 0; i < NUM_OF_AVG; i++) { // Remove the min and max from the calculations, and save how many they were - to decrease from the average calculation
		if (arr[i] == min || arr[i] == max) {
			skipped += 1;
			Serial.print("Removed from calculation: ");
			Serial.println(arr[i]);
		} else {
			total += arr[i];
		}
	}
	average = total / (NUM_OF_AVG - skipped);
	this->p0mm = average; // Saves it as a class variable instead of returning
}

// int US::searchObj(float distance, float p0) {
int US::searchObj(float distance) {
	/*
	 * Parameters:
		distance: reading of distance
		p0: point 0, to know the max range
	 * Returns a number to indicate whether there is an object found or not
	*/
	int sensitivity = this->minDistance;
	float p = this->p0mm;
	if (distance <= p-sensitivity) { // Distance of object is in good range
		this->streak+=1;
	} else {
		this->streak = 0; // Reset count
	}
	
	if (this->streak == STREAK_REQ) { // Streak reached
		return STREAK_OK; // Just a random number, not importants
	}
	return this->streak; // Returns the current streak, or STREAK_OK (should be 88) in case it's enough
}



/*
 * Get & Set functions
 * --------------------
*/

void US::setStreak(int val) {
	/*
	 * Set the streak of good reads
	*/
	this->streak = val;
}

float US::getP0() {
	/*
	 * Get the P0
	*/
	return this->p0;
}

float US::getP0_mm() {
	/*
	 * Get the P0_mm - without the smallest and largest numbers
	*/
	return this->p0mm;
}



/*
 * General comments
 * -----------------
*/


/*
else if (distance > p0) { // Distance is farther than max
	return 2;
} else if (distance <= p0) { // Distance is closer than max
	return 3;
} else { // Shouldn't happen
	return 4;
}
*/