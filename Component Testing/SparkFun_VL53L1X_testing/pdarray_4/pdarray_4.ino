/*
  Reading distance from the laser based VL53L1X using a one-shot measurement.
  By: Nathan Seidle
  Revised by: Andy England, Ricardo Ramos
  Pull request by: Joseph Duchesne
  SparkFun Electronics
  Date: January 31st, 2022
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  SparkFun labored with love to create this code. Feel like supporting open source hardware? 
  Buy a board from SparkFun! https://www.sparkfun.com/products/14667

  This example makes a one shot measurement.

  Are you getting weird readings? Be sure the vacuum tape has been removed from the sensor.
*/

#include <Wire.h>
#include "SparkFun_VL53L1X.h" //Click here to get the library: http://librarymanager/All#SparkFun_VL53L1X

//Optional interrupt and shutdown pins.
#define SHUTDOWN_PIN 2
#define INTERRUPT_PIN 3

SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

int distance1, distance2, distance3, distance4;

void setup(void)
{
  Wire.begin();

  Serial.begin(115200);
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");

  // CUSTOM

  // For 8x8
  // 163: upper-left
  // 227: upper-right
  // 92:  lower-left
  // 28:  lower-right
  // distanceSensor.setROI(4, 4, 145);
}

void loop(void)
{
  distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }

  distanceSensor.setROI(8, 8, 163);
  distance1 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

  distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }

  distanceSensor.setROI(8, 8, 227);
  distance2 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

  distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  
  distanceSensor.setROI(8, 8, 92);
  distance3 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

  distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
  while (!distanceSensor.checkForDataReady())
  {
    delay(1);
  }
  distanceSensor.setROI(8, 8, 28);
  distance4 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

  // Serial.print("Distance(mm): ");
  Serial.print(distance1);
  Serial.print(",");
  Serial.print(distance2);
  Serial.print(",");
  Serial.print(distance3);
  Serial.print(",");
  Serial.print(distance4);

  // float distanceInches = distance * 0.0393701;
  // float distanceFeet = distanceInches / 12.0;

  // Serial.print("\tDistance(ft): ");
  // Serial.print(distanceFeet, 2);

  Serial.println();
}
