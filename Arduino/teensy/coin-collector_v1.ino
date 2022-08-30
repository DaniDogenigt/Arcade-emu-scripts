// #include <Keyboard.h> // Llibrary for sending keystrokes (not needed for Teensyduino).

int signalValue = 0; // For storing value from digital input
int state; // Current state
int lastState = 0; // Last state

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

}

void loop() {

  signalValue = digitalRead(2); // Read analog value from coin selector

  if (signalValue == 0) {

    state = 1; // State is 1 as we're high

  } else {

    state = 0;
    
    

    if (update == 0) {

      if ((millis() - updateDebounceTime) > updateDebounceDelay) {

        coinValue = 0; // Reset current coin value

        update = 1; // Make sure we don't run again, till next coin

      }

    }

  }

  if (state != lastState) {

    // Process new signal

    if (state == 1) {

      digitalWrite(13, HIGH); // Turn status LED on to show signal

      coinValue = 1; // Update coin value

      updateDebounceTime = millis(); // Update last time we processed a signal

      update = 0; // Time to send a update now?

      Keyboard.press(KEYPAD_PLUS);
      delay(100);
      Keyboard.release(KEYPAD_PLUS);
    } else {

      digitalWrite(13, LOW);  // Turn status LED off
      
      
    }

    lastState = state; // Update last state
   
  }

  delay(20);

}
