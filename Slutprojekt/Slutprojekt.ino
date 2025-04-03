#include "Adafruit_VL53L0X.h"
#include <Servo.h>

Servo servo; 

Adafruit_VL53L0X laser = Adafruit_VL53L0X();
float error = 0;
float maped_value = 0;
float pid = 0;
float p = 0, i = 0, d = 0, u = 0;
float distanse = 170;
float tp = 0.15;
float ti = 0.5; 
float td = 1.0; 

void setup() {
  Serial.begin(115200);
  servo.attach(10);
  pinMode(10, OUTPUT); 
  
  while (!Serial) {
    delay(1);
  }
  if (!laser.begin()) {
    while (1)
      ;
  }
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}

void loop() {

  servo.write();
  update();
  delay(50);
}

int update() {
  VL53L0X_RangingMeasurementData_t varde;
  laser.rangingTest(&varde, false);
  if (varde.RangeStatus != 4) {
    error = (varde.RangeMilliMeter - distanse);
  }

  d = (error - (varde.RangeMilliMeter - 20));
  i += error;
 
pid = (error * tp) + (ti * d) + (td * i); 

  Serial.print("Update:");
  Serial.println(pid); 
}