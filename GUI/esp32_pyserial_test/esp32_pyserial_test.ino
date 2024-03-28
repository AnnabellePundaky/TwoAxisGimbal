// https://forum.arduino.cc/t/serial-input-basics-updated/382007
// https://www.youtube.com/watch?v=ZupqryZjIP8  <- stuff about KeyBoardInterrupt
// Two-part series on Python ESP32 serial communication:
// https://www.youtube.com/watch?v=ZupqryZjIP8
// https://www.youtube.com/watch?v=Jr79o_28tSY


// char receivedChar;
// boolean newData = false;

char wait_for_python_char;

int i = 0;  // Dummy variable for printouts

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Send char to Python
  Serial.println("@");

  Serial.println("<ESP32 is beginning main loop>");
}

void loop() {
  // put your main code here, to run repeatedly:
  // recvOneChar();
  // showNewData();

  // Wait for character from Python before beginning
  while(1) {
    if (Serial.available() > 0) {
      wait_for_python_char = Serial.read();
      if (wait_for_python_char = '#') {
        Serial.println("RECEIVED CHAR FROM PYTHON");
        break;
      }
    }
  }

  // Main loop
  while(1) {
    i = (i + 1) % 1000;
    Serial.print("Test ");
    Serial.println(i);
    delay(100);
  }
}

/*
void recvOneChar() {
  if (Serial.available() > 0) {
    receivedChar = Serial.read();
    newData = true;
  }
}

void showNewData() {
  if (newData == true) {
    Serial.print("This just in ... ");
    Serial.println(receivedChar);
    newData = false;
  }
}
*/