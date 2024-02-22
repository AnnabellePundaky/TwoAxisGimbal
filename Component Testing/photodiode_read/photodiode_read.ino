const int PD_PIN = 34;

int pdVal = 0;

bool dirHigh;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  pdVal = analogRead(PD_PIN);
  Serial.println(pdVal);
  delay(500);

}