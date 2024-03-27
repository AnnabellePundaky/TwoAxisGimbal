enum MICROSTEP_MODES {
  MICROSTEP_FULL,
  MICROSTEP_HALF,
  MICROSTEP_QUARTER,
  MICROSTEP_EIGHTH,
  MICROSTEP_SIXTEENTH
};

// Step and direction pins
const int M1_STEP_PIN = 12;  // M1_STEP
const int M1_DIR_PIN = 13;   // M1_DIR
const int M2_STEP_PIN = 27;  // M2_STEP
const int M2_DIR_PIN = 14;   // M2_DIR

// Microstepping pins
const int M1_MS1_PIN = 26;
const int M1_MS2_PIN = 25;
const int M1_MS3_PIN = 33;
const int M2_MS1_PIN = 32;
const int M2_MS2_PIN = 35;
const int M2_MS3_PIN = 34;

const int steps_per_rev = 1028;

// Lower step time results in higher turning speed
int step_time_us = 1000;

void setup()
{
  Serial.begin(115200);

  pinMode(M1_STEP_PIN, OUTPUT);
  pinMode(M1_DIR_PIN, OUTPUT);
  pinMode(M2_STEP_PIN, OUTPUT);
  pinMode(M2_DIR_PIN, OUTPUT);

  pinMode(M1_MS1_PIN, OUTPUT);
  pinMode(M1_MS2_PIN, OUTPUT);
  pinMode(M1_MS3_PIN, OUTPUT);
  pinMode(M2_MS1_PIN, OUTPUT);
  pinMode(M2_MS2_PIN, OUTPUT);
  pinMode(M2_MS3_PIN, OUTPUT);

  // Set microstep mode
  microstep_mode(MICROSTEP_FULL);
}
void loop()
{
  // MOTOR 1 (first DOF in the base)
  // Set motor 1 direction clockwise and run
  digitalWrite(M1_DIR_PIN, HIGH);
  Serial.println("Spinning M1 clockwise...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(M1_STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);  // 2000
    digitalWrite(M1_STEP_PIN, LOW);
    delayMicroseconds(step_time_us);  // 2000
  }
  delay(500);
  // Set motor 1 direction counterclockwise and run
  digitalWrite(M1_DIR_PIN, LOW);
  Serial.println("Spinning M1 counterclockwise...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(M1_STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);   // 1000
    digitalWrite(M1_STEP_PIN, LOW);
    delayMicroseconds(step_time_us);   // 1000
  }

  delay(500);

  // MOTOR 2 (second dof)
  // Set motor 2 direction clockwise
  digitalWrite(M2_DIR_PIN, HIGH);
  Serial.println("Spinning M2 clockwise...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(M2_STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);  // 2000
    digitalWrite(M2_STEP_PIN, LOW);
    delayMicroseconds(step_time_us);  // 2000
  }
  delay(500);
  // Set motor 2 direction counterclockwise
  digitalWrite(M2_DIR_PIN, LOW);
  Serial.println("Spinning M2 counterclockwise...");
  for(int i = 0; i<steps_per_rev; i++)
  {
    digitalWrite(M2_STEP_PIN, HIGH);
    delayMicroseconds(step_time_us);   // 1000
    digitalWrite(M2_STEP_PIN, LOW);
    delayMicroseconds(step_time_us);   // 1000
  }

  delay(500);
}

/*
MS1	MS2	MS3	Microstep Resolution
Low	Low	Low	Full step
High	Low	Low	Half step
Low	High	Low	Quarter step
High	High	Low	Eighth step
High	High	High	Sixteenth step
*/

void microstep_mode(int microstep_mode) {
  switch (microstep_mode) {
    case MICROSTEP_FULL:
      digitalWrite(M1_MS1_PIN, LOW);
      digitalWrite(M1_MS2_PIN, LOW);
      digitalWrite(M1_MS3_PIN, LOW);
      digitalWrite(M2_MS1_PIN, LOW);
      digitalWrite(M2_MS2_PIN, LOW);
      digitalWrite(M2_MS3_PIN, LOW);
      break;
    case MICROSTEP_HALF:
      digitalWrite(M1_MS1_PIN, HIGH);
      digitalWrite(M1_MS2_PIN, LOW);
      digitalWrite(M1_MS3_PIN, LOW);
      digitalWrite(M2_MS1_PIN, HIGH);
      digitalWrite(M2_MS2_PIN, LOW);
      digitalWrite(M2_MS3_PIN, LOW);
      break;
    case MICROSTEP_QUARTER:
      digitalWrite(M1_MS1_PIN, LOW);
      digitalWrite(M1_MS2_PIN, HIGH);
      digitalWrite(M1_MS3_PIN, LOW);
      digitalWrite(M2_MS1_PIN, LOW);
      digitalWrite(M2_MS2_PIN, HIGH);
      digitalWrite(M2_MS3_PIN, LOW);
      break;
    case MICROSTEP_EIGHTH:
      digitalWrite(M1_MS1_PIN, HIGH);
      digitalWrite(M1_MS2_PIN, HIGH);
      digitalWrite(M1_MS3_PIN, LOW);
      digitalWrite(M2_MS1_PIN, HIGH);
      digitalWrite(M2_MS2_PIN, HIGH);
      digitalWrite(M2_MS3_PIN, LOW);
      break;
    case MICROSTEP_SIXTEENTH:
      digitalWrite(M1_MS1_PIN, HIGH);
      digitalWrite(M1_MS2_PIN, HIGH);
      digitalWrite(M1_MS3_PIN, HIGH);
      digitalWrite(M2_MS1_PIN, HIGH);
      digitalWrite(M2_MS2_PIN, HIGH);
      digitalWrite(M2_MS3_PIN, HIGH);
      break;
  } 
}