int pwmChannel = 0; // Selects channel 0
int frequence = 1000000; // PWM frequency of 1 KHz
int resolution = 8; // 8-bit resolution, 256 possible values
int pwmPin = 23;

void setup() {
  // put your setup code here, to run once:
  // pinMode(15, OUTPUT);

  // Configuration of channel 0 with the chosen frequency and resolution
    ledcSetup(pwmChannel, frequence, resolution);

    // Assigns the PWM channel to pin 23
    ledcAttachPin(pwmPin, pwmChannel);

    // Create the selected output voltage
    ledcWrite(pwmChannel, 127); // 1.65 V
}

void loop() {
  // put your main code here, to run repeatedly:
  // digitalWrite(15, HIGH);
  // delayMicroseconds(1);
  // digitalWrite(15, LOW);
  // delayMicroseconds(1);
}
