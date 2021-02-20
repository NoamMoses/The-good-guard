#include <Servo.h>
#include <US.h>
#include <GG.h>
using namespace std;

/* Init sensors */
US v1(8, 9);
US v2(10, 11);
GG gg;
Servo gate;
float distance1 = 0.0, distance2 = 0.0, // Distances
  timeDiff = 0.0, timeDiffSec = 0.0; // To save the time between the two sensors reads
int objFound1 = 0, objFound2 = 0, obj1 = 0, obj2 = 0; // Indicators of whetther an object was found or not
char txt[200] = {0};

void setup() {
  Serial.begin(9600);
  Serial.println("# Setup has been started");
  printSetup();
  v1.init(); // Calls pinmode functions
  v2.init();
  v1.setSensitivity(SENS); // Recognition sensitivity
  v2.setSensitivity(SENS);
  Serial.println("* US sensors Initalisation has been ompleted");
  gate.attach(7); // Auto value is 90 -  natural, centre
  delay(50); // Small delay, to make sure the servo has time to set itself
  gate.write(0); // Set to 0
  Serial.println("* Servo setup has been completed");
  Serial.println("# Setup has been completed");
  delay(500); // Time between setup and actual program
}

void loop() {
  if (!objFound1) { // Sensor1 still didn't find anything
    distance1 = v1.getDistance();
    obj1 = v1.searchObj(distance1);
    if (obj1 == STREAK_OK) { // Got enough reads, an object was found
      gg.setT1(millis()); // Save time
      gg.printObjFound("Object1", distance1, v1.getP0(), v1.getP0_mm());
      objFound1 = 1;
    } else { // Still searching for an object, either not found, or not enough good reads
      Serial.print("Sensor1 counted: ");
      Serial.println(obj1);
    }
  }
  delay(10);
  if (objFound1) { // Check sensor2 just if sensor1 already found something
    if (!objFound2) {
      distance2 = v2.getDistance(); // Read distances from sensors
      obj2 = v2.searchObj(distance2); // Searching for an object
      if (obj2 == STREAK_OK) {
        gg.setT2(millis());
        gg.printObjFound("Object2", distance2, v2.getP0(), v2.getP0_mm());
        objFound2 = 1;
      } else {
      Serial.print("Sensor2 counted: ");
        Serial.println(obj2);
      }
    }
  }
  if (objFound1 && objFound2) { // Both sensors sensed an object
    timeDiff = gg.getT2() - gg.getT1();
    timeDiffSec = timeDiff/1000;
    sprintf(txt, "Both found.\nTime between sensors: %ss (%sms)", String(timeDiffSec).c_str(), String(timeDiff).c_str());
    Serial.println(txt);
    int servoSpeed = 0;
    if (timeDiffSec <= 1.0) {
      gg.openGate(gate, 100);
    } else {
      gg.openGate(gate, 300);
    }
    /* Gate is fully opened */
    delay(100000000); // Stop the software when the gate opens
  }
  delay(0); // Delay between each loop, should stay 0 unless debugging
}


void printSetup() {
  /*
   * Helper function, to save lines and println calls
  */
  char txt[200] = {0}; // Buffer
  sprintf(txt, "\
* Variables values:\n  ----------------\n\
    • Sensitivity: %d\n\
    • Streak required: %d\n\
    • Version: %s",
SENS, STREAK_REQ, VERSION);
  Serial.println(txt);
}
