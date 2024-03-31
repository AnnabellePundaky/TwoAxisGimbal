#include <Wire.h>
#include "SparkFun_VL53L1X.h"

#include <Arduino.h>

// Serial pins to receive input from Raspberry Pi 4
#define TX2 17
#define RX2 16

// Milliseconds to wait between time-of-flight measurements
#define TOF_INTERVAL 250

// Define serial input request flags
const int CW_X_FLAG = 1;  // Move horizontal-axis motor clockwise
const int CCW_X_FLAG = 2; // Move horizontal-axis motor counterclockwise
const int CW_Y_FLAG = 3;  // Move vertical-axis motor clockwise
const int CCW_Y_FLAG = 4; // Move vertical-axis motor counterclockwise
// Motor steps per request
const int NUM_STEPS_PER_REQ = 5;
int step_delay_us = 500;  // Time to pause after each step [microseconds]

// Motor driver pins
// A4988 pins for the horizontal-axis motor
const int STEP_X_PIN = 12;
const int DIR_X_PIN = 13;
// A4988 pins for the vertical-axis motor
const int STEP_Y_PIN = 27;
const int DIR_Y_PIN = 14;

// Working variables
char rpi_in_byte = ' ';
int rpi_in_num;

// Function prototypes
int get_rpi_serial_input_as_int();
void moveMotor(int stepPin, int steps);

// Distance sensor
SFEVL53L1X distanceSensor;
int distance;

// Timing variables
unsigned long prev_time = 0;
unsigned long curr_time;


void setup() {
  Wire.begin();

  Serial.begin(9600); // Initialize Serial for monitoring
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2); // Initialize Serial2 for UART communication

  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");
  distanceSensor.setDistanceModeLong();

  // Initialize motor pins as outputs
  pinMode(STEP_X_PIN, OUTPUT);
  pinMode(DIR_X_PIN, OUTPUT);
  pinMode(STEP_Y_PIN, OUTPUT);
  pinMode(DIR_Y_PIN, OUTPUT);
}


void loop() {
  if (Serial.available()) {
    char input = Serial.read();

    if (input == '#') {
      while (true) {
        if (Serial2.available()) {
          rpi_in_num = get_rpi_serial_input_as_int();
          // Serial.print("Received command:  ");
          // Serial.print(in_num);
          if (rpi_in_num == CW_X_FLAG) {   // CW and CCW are reversed but tracks properly.
            // Select motor and direction based on command
            bool direction = 0;
            digitalWrite(DIR_X_PIN, direction);
            moveMotor(STEP_X_PIN, NUM_STEPS_PER_REQ);
            // Serial.println("Move Horizontal-axis CW");
          }
          else if( rpi_in_num == CCW_X_FLAG ) {
            // Select motor and direction based on command
            bool direction = 1;
            digitalWrite(DIR_X_PIN, direction);
            moveMotor(STEP_X_PIN, NUM_STEPS_PER_REQ);
            // Serial.println("Move Horizontal-axis CCW");
          }
          else if (rpi_in_num == CW_Y_FLAG) {
            // Select motor and direction based on command
            bool direction = 0;
            digitalWrite(DIR_Y_PIN, direction);
            moveMotor(STEP_Y_PIN, NUM_STEPS_PER_REQ);
            //  Serial.println("Move Vertical-axis CW");
          }
          else if (rpi_in_num == CCW_Y_FLAG) {
            // Select motor and direction based on command
            bool direction = 1;
            digitalWrite(DIR_Y_PIN, direction);
            moveMotor(STEP_Y_PIN, NUM_STEPS_PER_REQ);
            //  Serial.println("Move Vertical-axis CCW");
          }
          // else {
          //   Serial.println("Unknown Command");
          // }
        }
        
        curr_time = millis();
        if (curr_time - prev_time >= TOF_INTERVAL) {
          prev_time = curr_time;
          distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
          while (!distanceSensor.checkForDataReady()) {
            delayMicroseconds(1);
          }
          distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
          // distanceSensor.clearInterrupt();
          // distanceSensor.stopRanging();
          Serial.print(distance);
          Serial.println();
        }
      }
    }

    else if (input == '$') {
      while (true) {
        Serial.println("Mode 2: Sensor data 2");
        delay(1000);
      }
    }
  }
}


int get_rpi_serial_input_as_int() {
  rpi_in_byte = Serial2.read();
  return int(rpi_in_byte - '0');  // Convert ASCII character to int.
}


void moveMotor(int stepPin, int steps) {
  for(int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(step_delay_us);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(step_delay_us);
  }
}
