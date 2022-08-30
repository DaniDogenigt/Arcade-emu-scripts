int ledPin = 13;
int lampPin = 9; // for small 12V light bulb above coin collector

// For Coin Acceptor:
int coinPin = 5; // Digital Input from Coin Collector
int signalValue = 0; // For storing value from coin input
int state; // Current state
int lastState = 0; // Last state
//int balance = 0; // Pence
int coinValue = 0; // Curent coin value
int update = 1; // Used for sending an update
long updateDebounceTime = 0; // The last time we sent an update
long updateDebounceDelay = 50; // Update 500ms after last singal pulse

// For Zero-switch:
#define ZEROSWITCH 1
int zeroPin = 2; // HIGH when flip-counter is zero
int brakePin = 4; // HIGH when flip-counter is close to zero
int zeroPinState = 0;
int zeroTrue = 0; // variable for detecting zero-switch rising edge
int brakeTrue = 0;

#define DEBOUNCE_DELAY 120
//char meterTopic[]="meter/0";
int buttonStateFalling[ZEROSWITCH];
int lastButtonStateFalling[ZEROSWITCH];
unsigned long millisPrevious[ZEROSWITCH];

// Motor:
//int motorPowerPin = 2; // Digital Out for turning DC-motor ON/OFF
int motorPWMPin = 3; 
int motorArray[]={0, 1000, 2200, 3500, 4100, 5250, 6300, 7500, 8800, 10500};
unsigned long motorTime = 0;
int motorSpeed = 0;
int speedHigh = 255;
int speedLow = 100;
int busy = 0;

// For Serial Comm:
String msg;
int credits = 0;
int lastCredits = 0;
// _______________________________________________________________________

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(coinPin, INPUT_PULLUP);
  pinMode(brakePin, INPUT_PULLUP);
  
  
  for (int i=0; i < ZEROSWITCH; i++) {
    pinMode(zeroPin, INPUT_PULLUP);
    buttonStateFalling[i]=1;
    lastButtonStateFalling[i]=1;
  }

  goToZero();
}

void loop() {
  //checkCoin();
  readSerialPort();
    if (credits != lastCredits) {
      if (zeroTrue == 0 && credits != 0) {
        goToZero();
        delay(500);
        counter();
      } else if (zeroTrue == 1) {
        //sendData();
        counter(); 
      } else if (zeroTrue == 0 && credits == 0) {
        goToZero();
      }
  }
}



void goToZero() {
  zeroTrue = 0;
  Serial.println("goToZero() called...");
  delay(500);
  while(zeroTrue == 0) {
    brakeTrue = !digitalRead(zeroPin);
    int braking = 0;
    motorSpeed = 0;
    if (!brakeTrue) {
      motorSpeed = speedHigh; 
    } else if (brakeTrue) {
      motorSpeed = speedLow;
    }
    analogWrite(motorPWMPin, motorSpeed);
    unsigned long now=millis();

    for(int i=0 ; i < ZEROSWITCH ; i++) {
      buttonStateFalling[i] = digitalRead(zeroPin);
      if ((buttonStateFalling[i] == LOW) && (lastButtonStateFalling[i] == HIGH)) {
        millisPrevious[i] = now;
      } else if ((buttonStateFalling[i] == HIGH) && (lastButtonStateFalling[i] == LOW) && (now - millisPrevious[i] > DEBOUNCE_DELAY)) {
        zeroTrue = 1;
      }
      lastButtonStateFalling[i] = buttonStateFalling[i];
    }
  }
  // Code below executes when breaking out of while loop
  analogWrite(ledPin, 0);
  Serial.println("goToZero() done...");
  //threads.yield();
}

void counter() {
  busy = 1;
  motorTime = motorArray[credits];
  
  while (busy == 1) {
    zeroTrue = 0;
    brakeTrue = !digitalRead(brakePin);
    motorSpeed = 0;
    sendData();
    Serial.println("counter() called...");
    Serial.print("counter() motorTime: ");
    Serial.println(motorArray[credits]);
    if (!brakeTrue) {
      motorSpeed = speedHigh;
    } else if (brakeTrue) {
      motorSpeed = speedLow; 
      //Serial.println("braking...");
    }
    analogWrite(ledPin, motorSpeed);
    delay(motorTime);
    analogWrite(ledPin, 0);
    busy = 0;
  }
    Serial.println("counter() done..."); 
}

void readSerialPort() {
  msg = "";
  lastCredits = credits;
  if (Serial.available()) {
    delay(100);
    while (Serial.available() > 0) {
        int inChar = Serial.read();
        delay(250);
        if (isDigit(inChar)) {
        msg += (char)inChar;
      }
      if (inChar == '\n') {
      credits = msg.toInt();
      msg = "";
      }
    }
    Serial.flush();
  }
}

void sendData() {
  //Serial.print("String: ");
  //Serial.println(msg);
  Serial.print("credits: ");
  //Serial.println(credits);
  //delay(100);    
}

void checkCoin() {
 while(1) {
  signalValue = digitalRead(coinPin); // Read value from coin selector
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
    if (state == 1) {
      coinValue = 1; // + signalCostFactor; // Update coin value
      updateDebounceTime = millis(); // Update last time we processed a signal
      update = 0; // Time to send a update now?
      Serial.print("Coin Inserted ");
      delay(500);
      //threads.delay(500);
      // Keyboard.press(KEY_C);
      digitalWrite(ledPin, HIGH); // Turn status LED on to show signal
      delay(500);
      //threads.delay(500);
      //Keyboard.release(KEY_C);
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, LOW);  // Turn status LED off
    }
    lastState = state; // Update last state
   }
   //threads.yield();
 }
}
