#include <TimerOne.h>

//#include <TimerOne.h>

#include <Arduino.h>
#include "TeensyThreads.h"
#include <Encoder.h>
//#include <TimerOne.h>
#include <avr/io.h>
//#include <avr/interrupt.h>

int factor = 0;
int motorValue = 0;

int ledPin = 13;
int lampPin = 9; // for small 12V light bulb above coin collector

// For Coin Acceptor:
int coinPin = 0; // Digital Input from Coin Collector
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
int zeroPin = 1; // HIGH when flip-counter is zero
//int brakePin = 2; // HIGH when flip-counter is close to zero
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
const int motorPWMPin = 4; 
int motorArray[]={0, 35, 70, 120, 160, 200, 230, 260, 290, 340, 370};
unsigned long motorTime = 0;
int motorSpeed = 0;
const int speedHigh = 255;
const int speedLow = 200;
int busy = 0;
 int stepCount = 0;
 int stepState = 0;

// For rotary-encoder:
Encoder myEnc(2, 3);
long oldPosition  = 0;
long newPosition = 0;
int count = 0;

// For Serial Comm:
String msg;
int credits = 0;
int lastCredits = 0;
// _______________________________________________________________________

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(coinPin, INPUT_PULLUP);
  pinMode(0, INPUT_PULLUP);
  //pinMode(brakePin, INPUT_PULLUP);
  pinMode(motorPWMPin, OUTPUT);

   // set PWM frequency 
  //analogWriteFrequency( motorPWMPin, 40000 );
  
  for (int i=0; i < ZEROSWITCH; i++) {
    pinMode(zeroPin, INPUT_PULLUP);
    buttonStateFalling[i]=1;
    lastButtonStateFalling[i]=1;
  }
  Timer1.initialize(1000000);
  Timer1.attachInterrupt(motorStep); 

  
  //DONE - Go to zero.
  goToZero();
}

void loop() {
  //checkCoin();
  //interrupts();
  
//  readSerialPort();
//      if (zeroTrue == 0 && credits != 0) {
//        goToZero();
//        delay(500);
//        counter();
//      } else if (zeroTrue == 1) {
//        //sendData();
      counter(); 
//      } 
  
}



void goToZero() {
  zeroTrue = 0;
  Serial.println("goToZero() called...");
  //threads.delay(500);
  while(zeroTrue == 0) {
    //brakeTrue = !digitalRead(zeroPin);
    //int braking = 0;
    motorSpeed = 0;
    //Serial.println(brakeTrue);
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
        brakeTrue = 1;
      } else if ((buttonStateFalling[i] == HIGH) && (lastButtonStateFalling[i] == LOW) && (now - millisPrevious[i] > DEBOUNCE_DELAY)) {
        zeroTrue = 1;
      }
      lastButtonStateFalling[i] = buttonStateFalling[i];
    }
  }
  // Code below executes when breaking out of while loop
  analogWrite(motorPWMPin, 0);
  Serial.println("goToZero() done...");
//  counter();
  threads.yield();
}

void counter() {
  busy = 1;
  motorTime = motorArray[credits];
  Serial.println("counter() called...");
  //int timer = 0;
  
  while (busy == 1) {
    readSerialPort();
    zeroTrue = 0;
    //brakeTrue = !digitalRead(brakePin);
    //motorSpeed = 0;
    //sendData();
    
    //Serial.print("counter() motorTime: ");
    //Serial.println(motorArray[credits]);
//    if (!brakeTrue) {
//      motorSpeed = speedHigh;
//    } else if (brakeTrue) {
      motorSpeed = 255; 
      //Serial.println("braking...");
//    }
    
    //Serial.println(addTime);
    
    if (credits != lastCredits) {
      
    Serial.println(credits);
    Serial.println(lastCredits);
    interrupts();
    //int result = abs ( credits - lastCredits );
//    analogWrite(motorPWMPin, motorSpeed);
    //delay(motorTime);
    //motorStep(5);
//    sendData();
//    
//   motorValue = credits * 35;
////    if (credits > 10 && credits < 40) {
////      motorValue = credits * 33;
////    }
////    else {
////      motorValue = credits * 34;
////    }
//    delay(motorValue);
//    
//    analogWrite(motorPWMPin, 0);
    //checkEncoder();
    
    
    busy = 0;
    
  }
 if (credits != lastCredits && credits == 0) {
   noInterrupts();
   goToZero();
    busy = 0;
  }
    //delay(5000);
    //Serial.println("counter() done..."); 
    //goToZero();
    
}
}

void motorStep() {
 stepState = digitalRead(motorPWMPin);

if (stepState == LOW) {
    stepState = HIGH;
    stepCount = stepCount + 1;  // increase when LED turns on
  } else {
    stepState = LOW;
  }
  digitalWrite(motorPWMPin, stepState);
  //delay(500);
}

void checkEncoder()
{
  long newPosition = myEnc.read();
//  if (newPosition != oldPosition) {
//  }
    count = newPosition - oldPosition;
    Serial.print("Count: "); 
    Serial.println(count);
     
    oldPosition = newPosition;
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
  Serial.println(credits);
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
      threads.delay(500);
      Keyboard.press(KEY_C);
      digitalWrite(ledPin, HIGH); // Turn status LED on to show signal
      threads.delay(500);
      Keyboard.release(KEY_C);
      digitalWrite(ledPin, LOW);
    } else {
      digitalWrite(ledPin, LOW);  // Turn status LED off
    }
    lastState = state; // Update last state
   }
   threads.yield();
 }
}
