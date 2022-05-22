// DavidMaitland.me 2015
// See LICENSE file for licensing details

// #include <Keyboard.h> // The main library for sending keystrokes.

// int coinSelector = 2; // Analog input pin that the coin selector uses
const int signalCostFactor = 5; // Each signal pulse is worth 5p

int signalValue = 0; // For storing value from analog input

int state; // Current state
int lastState = 0; // Last state

int balance = 0; // Pence
int coinValue = 0; // Curent coin value

int update = 1; // Used for sending an update

long updateDebounceTime = 0; // The last time we sent an update
long updateDebounceDelay = 50; // Update 500ms after last singal pulse

void setup() {
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT); // Status LED

  Serial.begin(9600); // Setup serial at 9600 baud

  delay(2000); // Don't start main loop until we're sure that the coin selector has started

  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
  //digitalWrite(13, HIGH);
  //delay(20);
  //digitalWrite(13, LOW);
  //delay(20);
  //digitalWrite(13, HIGH);
  //delay(20);
  //digitalWrite(13, LOW);
  //delay(20);
 // Serial.println("Ready..");

}

void loop() {

  signalValue = digitalRead(2); // Read analog value from coin selector

  if (signalValue == 0) {

    state = 1; // State is 1 as we're high

  } else {

    state = 0;

    // Should we send a balance update

    if (update == 0) {

      if ((millis() - updateDebounceTime) > updateDebounceDelay) {

        //Serial.print("Coin Value: ");
        //Serial.println(coinValue); // WARNING: The coin value will be wrong if coins are inserted within the updateDebounceDelay, adjust the delay and test

        coinValue = 0; // Reset current coin value

        update = 1; // Make sure we don't run again, till next coin

      }

    }

  }

  if (state != lastState) {

    // Process new signal

    if (state == 1) {

      digitalWrite(13, HIGH); // Turn status LED on to show signal

      // balance = balance + 1; // + signalCostFactor; // Update balance

      coinValue = 1; // + signalCostFactor; // Update coin value

      updateDebounceTime = millis(); // Update last time we processed a signal

      update = 0; // Time to send a update now?

      Keyboard.press(KEYPAD_PLUS);
      delay(100);
      Keyboard.release(KEYPAD_PLUS);
    } else {

      digitalWrite(13, LOW);  // Turn status LED off
      
      
    }

    lastState = state; // Update last state
    
    //digitalWrite(13, HIGH);
    //delay(20);
    //digitalWrite(13, LOW);
    //delay(20);
    //digitalWrite(13, HIGH);
    //delay(20);
    //digitalWrite(13, LOW);
    //delay(20);
    //digitalWrite(13, HIGH);
    //delay(20);
    //digitalWrite(13, LOW);
    //delay(20);
  }

  delay(20);

}
