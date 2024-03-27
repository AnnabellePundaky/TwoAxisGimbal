// Delay between printouts, in ms
#define PRINT_DELAY 100

// Simulated distance sweeps between DISTANCE_MIN and DISTANCE_MAX mm, with step size of DISTANCE_DELTA and some added noise
#define DISTANCE_MIN 10
#define DISTANCE_MAX 1000
#define DISTANCE_DELTA 20

unsigned long curr_ms;        // Running counter of total ms elapsed

int ms;
int seconds;
int minutes;

float distance_nominal = DISTANCE_MIN;    // mm
float distance_delta = DISTANCE_DELTA;    // mm
float distance;                           // With simulated random noise

float sample_rate;

void setup() {
  Serial.begin(115200);
}

void loop() {
  curr_ms = millis();
  
  ms = (int) (curr_ms % 1000);
  seconds = (int) (curr_ms / 1000.0) % 60;
  minutes = (int) (curr_ms / 1000.0 / 60.0);

  Serial.print(minutes);
  Serial.print(",");

  Serial.print(seconds);
  Serial.print(",");

  Serial.print(ms);
  Serial.print(",");

  // Simulate distance going back and forth between 10mm and 1000mm
  distance_nominal += distance_delta;
  if (distance_nominal >= DISTANCE_MAX || distance_nominal <= DISTANCE_MIN) {
    distance_delta *= -1;
  }
  // Add some random noise to the distance value before printing
  distance = distance_nominal + (random(-100,100) / 10.0);

  Serial.print(distance);
  Serial.print(",");

  // Simulate sample rate in Hz
  sample_rate = 1 / (PRINT_DELAY / 1000.0) + (random(-20, 20) / 100.0);

  Serial.print(sample_rate);
  Serial.print(",");

  Serial.println();
  delay(PRINT_DELAY);
}
