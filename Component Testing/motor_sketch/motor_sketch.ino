const int STEP_PIN = 12;
const int DIR_PIN = 13;

// Microstepping pins
const int MS1_PIN = 32;
const int MS2_PIN = 33;
const int MS3_PIN = 25;

const int steps_per_rev = 1028;

int step_time_us = 1000;

void setup()
{
  Serial.begin(115200);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);

  pinMode(MS1_PIN, OUTPUT);
  pinMode(MS2_PIN, OUTPUT);
  pinMode(MS3_PIN, OUTPUT);

  digitalWrite(MS1_PIN, HIGH);
  digitalWrite(MS2_PIN, HIGH);
  digitalWrite(MS3_PIN, HIGH);
}
void loop()
{
  // Set motor direction clockwise
  digitalWrite(DIR_PIN, HIGH);
  Serial.println("Spinning clockwise slowly...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);  // 2000
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(step_time_us);  // 2000
    // delay(500);
  }

  delay(1000);

  // Set motor direction counterclockwise
  digitalWrite(DIR_PIN, LOW);
  Serial.println("Spinning counterclockwise...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);   // 1000
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(step_time_us);   // 1000
    // delay(500);
  }

  delay(1000);
}