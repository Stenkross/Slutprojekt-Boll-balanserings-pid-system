/*
  File: PID_Seesaw_Project
  Auther: Sebatian Loe
  Date:  2025-05-04
  Description: Program that balances a ball in the middle of seesaw system with the use of a PID-regulator.
*/

// Include libraries
#include "Adafruit_VL53L0X.h"
#include <Servo.h>

//construct object for servo and sensor 
Servo servo;
Adafruit_VL53L0X laser = Adafruit_VL53L0X();

//Initialize global variables
float error = 0, pid = 0, i = 0, d = 0;
float mapped_val = 67;
float distanse = 180;
float alp = 0.2;
float kp = 5.0;
float ki = 0.1;
float kd = 50.0;
float lasterr = 0;
float flt_varde;

void setup() {
  //Initialize communications
  Serial.begin(115200);

  //Sets pin 10 as output for the servo and lets the servo move to position
  servo.attach(10);
  pinMode(10, OUTPUT);
  servo.write(mapped_val);
  delay(200);


  //Starts the laser, if it dosent work the program stops (fails to Initialize)
  if (!laser.begin()) {
    while (1)
      ;
  }
  Serial.println(F("systemet startar")); 
}


void loop() {
  //Maps the value over to an appropriate angle for the servo
  mapped_val = map(update(), -100, 100, 75, 56);
  servo.write(mapped_val);

  //Small delay for the system to be stable 
  delay(10);
}

/*
  Function: update
    - Reads the distance from the laser and calculates the pid value 

  Parameters 
    - No parameters
  Returns: The calculated PID value
*/

float update() {
  //Hold and reads the value of the laser
  VL53L0X_RangingMeasurementData_t varde;
  laser.rangingTest(&varde, false);

  //Checks if the reading is valid
  if (varde.RangeStatus != 4) {

    //Lowpass filter that reduces the noise 
    flt_varde = 0.5 * varde.RangeMilliMeter + (1 - 0.5) * flt_varde;

    // Calculates the distance from the setpoint and converts it to cm
    error = (flt_varde - distanse) / 10;
  }
  //Calculates the integal and derivitive parts
  d = (error - lasterr);
  i += (error);

  //Calculates the pid output
  pid = (error * kp) + (ki * i) + (kd * d);

  // Limits the pid value so that the servo dosent get hurt
  if (pid > 150) pid = 150;
  if (pid < -150) pid = -150;
  
  //saves current error for the derivitive part
  lasterr = error;

  //Returns the pid value
  return pid;
}