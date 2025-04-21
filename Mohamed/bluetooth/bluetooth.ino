#include <Dabble.h>
#define RELAY1 22
#define RELAY2 24
struct SpeedFeedback {
  int16_t speedL;
  int16_t speedR;
};

SpeedFeedback speedIn;

enum ParseState {
  WAIT_START,
  WAIT_FEEDBACK,
  WAIT_ACK
};

ParseState state = WAIT_START;
uint8_t buffer[sizeof(SpeedFeedback)];
uint8_t idx = 0;

void setup() {
  Serial.begin(9600);           // For debugging
  Serial1.begin(9600);
  Dabble.begin(9600, 3);        // HM-10 on Serial1 (Mega: TX1/RX1)
  Serial.println("Dabble Gamepad Ready");
  pinMode(RELAY1,OUTPUT);
  pinMode(RELAY2,OUTPUT);
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);
  delay(1000);
  digitalWrite(RELAY1,LOW);
  digitalWrite(RELAY2,LOW);
  delay(1000);
  digitalWrite(RELAY1,HIGH);
  digitalWrite(RELAY2,HIGH);

}

void loop() 
{
  Dabble.processInput();        // Required to update Gamepad states

  // Check which Gamepad button is pressed
  if (GamePad.isUpPressed()) {
    sendForward();
  }
  else if (GamePad.isDownPressed()) 
  {
    sendBackward();
  }
  else if (GamePad.isLeftPressed()) 
  {
    sendLeft();
  }
  else if (GamePad.isRightPressed()) 
  {
    sendRight();
  }
  else if (GamePad.isStartPressed()) 
  {
    // sendSteer();
  }
  else if (GamePad.isSelectPressed()) {
    sendStop();
  } 
  else if (GamePad.isTrianglePressed()) {
    Serial1.println('Z');
    
  }
  else if (GamePad.isCrossPressed()) {
    Serial1.println('X');
  }
  else if (GamePad.isCirclePressed()) {
    Serial1.println('C');
  } else if (GamePad.isSquarePressed()) {
    Serial1.println('V');
  } 
  else {
    sendStop();
  }
    // Process incoming bytes from Uno
  while (Serial1.available()) {
    byte b = Serial1.read();

    switch (state) {
      case WAIT_START:
        if (b == 0xAA) {
          state = WAIT_FEEDBACK;
          idx = 0;
        } else if (b == 0xAC) {
          state = WAIT_ACK;
        }
        break;

      case WAIT_FEEDBACK:
        buffer[idx++] = b;
        if (idx >= sizeof(SpeedFeedback)) {
          memcpy(&speedIn, buffer, sizeof(SpeedFeedback));
          Serial.print("Left Speed: ");
          Serial.print(speedIn.speedL);
          Serial.print(" | Right Speed: ");
          Serial.println(speedIn.speedR);
          state = WAIT_START;
        }
        break;

      case WAIT_ACK:
        Serial.print("ACK received for command: ");
        Serial.println((char)b);
        state = WAIT_START;
        break;
    }
  }
}

// === Motion Functions (Send command to Serial1) ===

void sendForward() {
  Serial1.println('F');
  // Serial1.println('F');
  Serial.println("Sent Forward");
}

void sendBackward() {
  Serial1.println('B');
  Serial.println("Sent Backward");
}

void sendLeft() {
  Serial1.println('L');
  Serial.println("Sent Left");
}

void sendRight() {
  Serial1.println('R');
  Serial.println("Sent Right");
}

void sendStop() {
  Serial1.println('S');
  Serial.println("Sent Stop");
}

void sendSteer() {
  Serial1.println('E');
  Serial.println("Sent Steer");
}