#include <Arduino.h>
#include "TeensyThreads.h"

// For Coin Acceptor:
int signalValue = 0; // For storing value from analog input
int state; // Current state
int lastState = 0; // Last state
int balance = 0; // Pence
int coinValue = 0; // Curent coin value
int update = 1; // Used for sending an update
long updateDebounceTime = 0; // The last time we sent an update
long updateDebounceDelay = 50; // Update 500ms after last singal pulse

// For Serial Comm:
String nom = "Arduino";
String msg;
int value = 0;

// For LED blinker:
int count = 0;
volatile int blinkcode = 0;
// --------------------------------------
void setup() {
  pinMode(13, OUTPUT); // Status LED
  pinMode(2, INPUT_PULLUP);
  //pinMode(LED, OUTPUT);
  Serial.begin(9600);
  delay(2000);
  
  threads.addThread(blinkthread);
  threads.addThread(checkCoin);
}

void loop() {

  // SERIAL :
  readSerialPort();
   
  if (msg != "") {
     sendData();
  }
  
  // LED BLINKER:
  //count = 3;
  count = value;
  blinkcode = count;
  delay(2000);

}

void sendData() {
  //write data
  Serial.print(nom);
  Serial.print(" received : ");
  Serial.print(msg);

  delay(100);    
}

void readSerialPort() {
  msg = "";
  if (Serial.available()) {
    delay(100);
    while (Serial.available() > 0) {
        //int inChar = Serial.read();
        msg += (char)Serial.read();
        delay(500);
        //if (isDigit(inChar)) {
        //msg += (char)inChar;
      //}
      //if (inChar == '\n') {
      value = msg.toInt();
      Serial.print("String: ");
      Serial.println(msg);
      Serial.print("INT: ");
      Serial.println(value);
      // clear the string for new input:
      msg = "";
    //}
    
    }
    Serial.flush();
  }
}

void checkCoin() {
 while(1) {
  
  signalValue = digitalRead(2); // Read analog value from coin selector

  if (signalValue == 0) {
    state = 1; // State is 1 as we're high
  } else {
    state = 0;
    // Should we send a balance update
    if (update == 0) {
      if ((millis() - updateDebounceTime) > updateDebounceDelay) {
        coinValue = 0; // Reset current coin value
        update = 1; // Make sure we don't run again, till next coin
      }
    }
  }
  if (state != lastState) {
    if (state == 1) {
      coinValue = 1; // + signalCostFactor; // Update coin value
      updateDebounceTime = millis(); // Update last time we processed a signal
      update = 0; // Time to send a update now?
      Serial.print("Coin Inserted ");
      threads.delay(500);
      //Keyboard.press(KEYPAD_PLUS);
      Keyboard.press(KEY_C);
      digitalWrite(13, HIGH); // Turn status LED on to show signal
      threads.delay(500);
      //Keyboard.release(KEYPAD_PLUS);
      Keyboard.release(KEY_C);
      digitalWrite(13, LOW);
    } else {

      digitalWrite(13, LOW);  // Turn status LED off
    }
    lastState = state; // Update last state
   }
   threads.yield();
 }
}


void blinkthread() {
  while(1) {
    if (blinkcode) {
      for (int i=0; i<blinkcode; i++) {
        digitalWrite(13, HIGH);
        threads.delay(50);
        digitalWrite(13, LOW);
        threads.delay(50);
      }
      blinkcode = 0;
    }
    threads.yield();
  }
}
