#include <Arduino.h>

#include <Wire.h>
#include "SparkFun_VL53L1X.h"


#define MENU_MODE             0
#define MAIN_GIMBAL_MODE      1   // Mode 1: Main gimbal demo        (Python script sends '#')
#define PHOTODIODE_ARRAY_MODE 2   // Mode 2: Photodiode array demo   (Python script sends '$')
int mode = MENU_MODE;
char wait_for_python_char;        // Either '#' or '$', sent by Python script

// Motor driver pins
// A4988 pins for the horizontal-axis motor
const int STEP_X_PIN = 12;
const int DIR_X_PIN = 13;
// A4988 pins for the vertical-axis motor
const int STEP_Y_PIN = 27;
const int DIR_Y_PIN = 14;

// Serial pins to receive input from Raspberry Pi 4
#define TX2 17
#define RX2 16

// Define serial input request flags
const int CW_X_FLAG = 1;  // Move horizontal-axis motor clockwise
const int CCW_X_FLAG = 2; // Move horizontal-axis motor counterclockwise
const int CW_Y_FLAG = 3;  // Move vertical-axis motor clockwise
const int CCW_Y_FLAG = 4; // Move vertical-axis motor counterclockwise
// Motor steps per request
const int NUM_STEPS_PER_REQ = 5;
int STEP_DELAY_US = 500; // Time in microseconds to pause after each step // WE ALSO NEED TO adjust this, THIS WILL COME FROM TESTING THE MOTORS, IF WE TEST THE MOTORS AND IT MOVES RELIABILY WITHOUT SKIPPING STEPS WE CAN SLOWLY REDUCE THIS

// Working variables
char rpi_in_byte = ' ';
int rpi_in_num;

// Distance sensor
int distance1, distance2, distance3, distance4;
SFEVL53L1X distanceSensor;
//Uncomment the following line to use the optional shutdown and interrupt pins.
//SFEVL53L1X distanceSensor(Wire, SHUTDOWN_PIN, INTERRUPT_PIN);

// Function prototypes
int get_rpi_serial_input_as_int();
void moveMotor(int stepPin, int steps);


void setup() {
  mode = MENU_MODE;   // Reset mode to MENU_MODE every time board is reset
  Wire.begin(); // Enable I2C for VL53L1X input 
  Serial.begin(115200); // Initialize Serial for monitoring.
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2); // Initialize Serial2 for UART communication
  // Initialize motor pins as outputs
  pinMode(STEP_X_PIN, OUTPUT);
  pinMode(DIR_X_PIN, OUTPUT);
  pinMode(STEP_Y_PIN, OUTPUT);
  pinMode(DIR_Y_PIN, OUTPUT);

  // Initialize distance sensor
  if (distanceSensor.begin() != 0) //Begin returns 0 on a good init
  {
    Serial.println("Sensor failed to begin. Please check wiring. Freezing...");
    while (1)
      ;
  }
  Serial.println("Sensor online!");
  distanceSensor.setDistanceModeLong();

  // Send char to Python
  Serial.println("@");
  Serial.println("<ESP32 is beginning main loop>");
}


void loop() {
  switch (mode) {
    case MENU_MODE:
      // Wait for character from Python before beginning
      while(1) {
        if (Serial.available() > 0) {
          wait_for_python_char = Serial.read();
          if (wait_for_python_char = '#') {
            mode = MAIN_GIMBAL_MODE;
            Serial.println("RECEIVED MODE 1 CHAR FROM PYTHON");
            break;
          }
          else if (wait_for_python_char = '$') {
            mode = PHOTODIODE_ARRAY_MODE;
            Serial.println("RECEIVED MODE 2 CHAR FROM PYTHON");
            break;
          }
        }
      }
      break;
    case MAIN_GIMBAL_MODE:
      // Check for a request from Raspberry Pi, move appropriate motor based on command
      if (Serial2.available()) {
        rpi_in_num = get_rpi_serial_input_as_int();
        Serial.print("Received command:  ");
        Serial.print(rpi_in_num);
        if (rpi_in_num == CW_X_FLAG) {   // CW and CCW are reversed but tracks properly.
          // Select motor and direction based on command
          bool direction = 0;
          digitalWrite(DIR_X_PIN, direction);
          moveMotor(STEP_X_PIN, NUM_STEPS_PER_REQ);
          Serial.println("Move Horizontal-axis CW");
        }
        else if( rpi_in_num == CCW_X_FLAG ) {
          // Select motor and direction based on command
          bool direction = 1;
          digitalWrite(DIR_X_PIN, direction);
          moveMotor(STEP_X_PIN, NUM_STEPS_PER_REQ);
          Serial.println("Move Horizontal-axis CCW");
        }
        else if (rpi_in_num == CW_Y_FLAG) {
          // Select motor and direction based on command
          bool direction = 1;
          digitalWrite(DIR_Y_PIN, direction);
          moveMotor(STEP_Y_PIN, NUM_STEPS_PER_REQ);
          Serial.println("Move Vertical-axis CW");
        }
        else if (rpi_in_num == CCW_Y_FLAG) {
          // Select motor and direction based on command
          bool direction = 0;
          digitalWrite(DIR_Y_PIN, direction);
          moveMotor(STEP_Y_PIN, NUM_STEPS_PER_REQ);
          Serial.println("Move Vertical-axis CCW");
        }
        else {
          // Serial.println("Unknown Command");
          Serial.println();
        }
      }
      Serial.println("Cool");
      delay(200);
      break;
    case PHOTODIODE_ARRAY_MODE:
      distanceSensor.setROI(8, 8, 163);
      distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
      while (!distanceSensor.checkForDataReady()) {
        // delay(1);
        delayMicroseconds(1);
      }
      distance1 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

      distanceSensor.setROI(8, 8, 227);
      distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
      while (!distanceSensor.checkForDataReady()) {
        delayMicroseconds(1);
      }
      distance2 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

      distanceSensor.setROI(8, 8, 92);
      distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
      while (!distanceSensor.checkForDataReady()) {
        delayMicroseconds(1);
      }
      distance3 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

      distanceSensor.setROI(8, 8, 28);
      distanceSensor.startOneshotRanging(); //Write configuration bytes to initiate measurement
      while (!distanceSensor.checkForDataReady()) {
        delayMicroseconds(1);
      }
      distance4 = distanceSensor.getDistance(); //Get the result of the measurement from the sensor

      Serial.print(distance1);
      Serial.print(",");
      Serial.print(distance2);
      Serial.print(",");
      Serial.print(distance3);
      Serial.print(",");
      Serial.print(distance4);
      Serial.println();
      break;
  }
}

int get_rpi_serial_input_as_int() {
  rpi_in_byte = Serial2.read();
  return int(rpi_in_byte - '0');  // Convert ASCII character to int.
}

void moveMotor(int stepPin, int steps) {
  for(int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(STEP_DELAY_US);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(STEP_DELAY_US);
  }
}
