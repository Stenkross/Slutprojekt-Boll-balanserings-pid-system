#include "Adafruit_VL53L0X.h"
#include <Servo.h>

Servo servo;

Adafruit_VL53L0X laser = Adafruit_VL53L0X();
float error = 0;
float pid = 0;
float mapped_val = 67;
float i = 0, d = 0;
float distanse = 180;
float filt_pid = 0;
float alp = 0.2;
float kp = 5.0;
float ki = 0.1;
float kd = 50.0;
float lasterr = 0;
float flt_varde;

void setup() {
  Serial.begin(115200);
  servo.attach(10);
  servo.write(mapped_val);
  delay(200);
  pinMode(10, OUTPUT);

  if (!laser.begin()) {
    while (1)
      ;
  }
  Serial.println(F("systemet startar"));
}

void loop() {
  filt_pid = 1 * update() + (1 - 1) * filt_pid;
  mapped_val = map(filt_pid, -100, 100, 75, 56);
  servo.write(mapped_val);
  delay(10);
}

float update() {
  VL53L0X_RangingMeasurementData_t varde;
  laser.rangingTest(&varde, false);

  if (varde.RangeStatus != 4) {
      flt_varde = 0.5 * varde.RangeMilliMeter + (1 - 0.5) * flt_varde;
    error = (flt_varde - distanse) / 10;
  }

  d = (error - lasterr);
  i += (error);
  pid = (error * kp) + (ki * i) + (kd * d);

  if (pid > 150) pid = 150;
  if (pid < -150) pid = -150;

  lasterr = error;

  Serial.print("Error: ");
  Serial.println(error);
  Serial.print("PID: ");
  Serial.println(pid);
  Serial.print("avstånd");
  Serial.println(varde.RangeMilliMeter);
  Serial.println("-------------------");
  return pid;
}