#include <LiquidCrystal.h>

const byte rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// SET TO 1 FOR SERIAL MONITORING
byte debug = 1;

#define ARRAYSIZE 24
String strings[ARRAYSIZE] = { };
String times[ARRAYSIZE] = { };

// SERIALREAD
String lineString;
String msg;
String output;
byte count = 0;

// CLOCK
String timeString;
const int interval = 1000;

byte lines_I[8] = {
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
};
byte lines_IIIII[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
};
byte lines_I_I[8] = {
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
  0b10100,
};
byte lines_I_I_I[8] = {
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
  0b10101,
};

void setup() {

  pinMode(8, OUTPUT);
  digitalWrite(8, HIGH);
  lcd.begin(16, 2);

  lcd.createChar(0, lines_I);
  lcd.createChar(1, lines_I_I);
  lcd.createChar(2, lines_I_I_I);
  lcd.createChar(3, lines_IIIII);

  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.print("ATMega328 ONLINE");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("   v0.1 2022    ");
  delay(2000);
  lcd.clear();
  initialize();
}

void loop() {

  if (debug == 1) {
    Serial.print("strings[");
    Serial.print(count);
    Serial.print("] = ");
    Serial.println(output);
    Serial.print("times[");
    Serial.print(count);
    Serial.print("] = ");
    Serial.println(timeString);
  }
  clockCount();
  delay(100);
}

void clockCount() {

  unsigned int currentMillis = millis();
  unsigned int previousMillis = 0;         // will store last time LED was updated
  byte seconds = 0;
  byte minutes = 0;
  byte hours = 0;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    seconds = currentMillis / 1000;
    minutes = seconds / 60;
    hours = minutes / 60;

    currentMillis %= 1000;
    seconds %= 60;
    minutes %= 60;
    hours %= 24;

    timeString = "";

    if (hours < 10) {
      timeString += "0";
      timeString += hours;
    } else {
      timeString += hours;
    }
    timeString += ":";
    if (minutes < 10) {
      timeString += "0";
      timeString += minutes;
    } else {
      timeString += minutes;
    }
    timeString += ":";
    if (seconds < 10) {
      timeString += "0";
      timeString += seconds;
    } else {
      timeString += seconds;
    }

    readSerial();
  }
}

void readSerial() {

  byte busy = 1;
  byte timeoutSec = 0;
  unsigned int timeoutMsPrev = 0;
  output = "";
  
  while (busy == 1) {    
    while (Serial.available() == 0) {
      unsigned int timeoutMs = millis();
      if (timeoutMs - timeoutMsPrev >= interval) {
        timeoutMsPrev = timeoutMs;
        timeoutMs %= 1000;
        timeoutSec++;
        timeoutSec %= 60;

        if (timeoutSec > 10) {
          digitalWrite(8, LOW); 
        }
      }
    }
    if (Serial.available()) {
      digitalWrite(8, HIGH); 
      
      char c = Serial.read(); //gets one byte from serial buffer
      if (c != ';') {
        output += c;
      } else {
        busy = 0;
      }
    }
  }
  if (count < ARRAYSIZE) {
    strings[count] = output;
    times[count] = timeString;
    count++;
    delay(100);
  } else {
    count = 0;
    strings[count] = output;
    times[count] = timeString;
    delay(100);
    count++;
  }
  lcdPrint(output);
}

void lcdPrint(String input) {

  String empty = "                ";

  byte strLength = input.length();
  lineString = input.substring(0, 1);
  byte line = lineString.toInt();
  msg = input.substring(2, strLength);

  if (debug == 1) {
    Serial.print("input: ");
    Serial.println(input);
    Serial.print("strLength: ");
    Serial.println(strLength);
    Serial.print("lineString: ");
    Serial.println(lineString);
    Serial.print("line: ");
    Serial.println(line);
    Serial.print("msg: ");
    Serial.println(msg);
  }
  lcd.setCursor(0, line);
  lcd.print(empty);
  lcd.setCursor(0, line);
  lcd.print(msg);
}

void initialize() {

  byte delayTime = 30;
  byte writePos = 0;

  lcd.setCursor(0, 0);
  lcd.print("LOADING...");

  while (writePos < 16) {
    lcd.setCursor(writePos, 1);
    lcd.write(byte(0));
    delay(delayTime);
    lcd.setCursor(writePos, 1);
    lcd.write(byte(1));
    delay(delayTime);
    lcd.setCursor(writePos, 1);
    lcd.write(byte(2));
    delay(delayTime);
    writePos++;
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TX-PIN:");
  lcd.setCursor(0, 1);
  lcd.print("SERIAL READY");
  delay(1000);
  
  digitalWrite(8, HIGH);
  
  while (Serial.available() == 0) {
    int timeoutMs = millis();
    byte timeoutSec = timeoutMs / 1000;
    byte timeoutLimit = 20;
    //currentMillis %= 1000;
    if (timeoutSec > timeoutLimit) {
      digitalWrite(8, LOW);
      timeoutMs = 0;
    }
    lcd.setCursor(12, 1);
    lcd.write(byte(3));
    
    delay(500);
    lcd.setCursor(12, 1);
    lcd.write(" ");
    
    delay(500);
  }
  lcd.clear();
}
