#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X laser = Adafruit_VL53L0X();

float p = 0, i = 0, d = 0, u = 0;

float tp = 2.0;  
float ti = 0.5; 
float td = 2.0; 

void setup() {
  Serial.begin(115200);
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

  update();
  delay(50);
}

int update() {
  VL53L0X_RangingMeasurementData_t varde;
  laser.rangingTest(&varde, false);
  if (varde.RangeStatus != 4) {
    Serial.println(varde.RangeMilliMeter);
  }

  d = (p - varde.RangeMilliMeter)/0.05;
  p = varde.RangeMilliMeter;
  i = 0.05 * p;


  u = tp * (p + i / ti + td * d);
  Serial.println(u);
}
