// Delay between printouts, in ms
#define PRINT_DELAY 100

unsigned long curr_ms, ms_end;
int ms;
int seconds;
int minutes;

float distance_nominal = 10;  // mm
float distance_delta = 20;    // mm
float distance;               // With simulated random noise

float sample_rate;

void setup() {
  Serial.begin(115200);
}

void loop() {
  curr_ms = millis();
  
  ms = (int) (curr_ms % 1000);
  seconds = (int) (curr_ms / 1000.0) % 60;
  minutes = (int) (curr_ms / 1000.0 / 60.0);

  // Simulate sample rate in Hz
  sample_rate = 1 / (PRINT_DELAY / 1000.0) + (random(-20, 20) / 100.0);

  Serial.print(minutes);
  Serial.print(",");

  Serial.print(seconds);
  Serial.print(",");

  Serial.print(ms);
  Serial.print(",");

  // Simulate distance going back and forth between 10mm and 1000mm
  distance_nominal += distance_delta;
  if (distance_nominal >= 1000 || distance_nominal <= 10) {
    distance_delta *= -1;
  }
  distance = distance_nominal + (random(-100,100) / 10.0);

  Serial.print(distance);
  Serial.print(",");

  Serial.print(sample_rate);
  Serial.print(",");

  Serial.println();
  delay(PRINT_DELAY);
}
