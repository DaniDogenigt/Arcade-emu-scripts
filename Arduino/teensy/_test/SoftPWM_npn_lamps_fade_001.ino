#include <SoftPWM.h>

int led = 4;
int lamp = 2;

void setup()
{
  pinMode(led, OUTPUT);
  pinMode(lamp, OUTPUT);
  SoftPWMBegin();
  
  SoftPWMSet(led, 0);
  //SoftPWMSet(lamp, 255);

  SoftPWMSetFadeTime(led, 1000, 2000);
  //SoftPWMSetFadeTime(lamp, 500, 500);

  analogWriteFrequency( lamp, 40000 );
  analogWrite(lamp, 255);
}

void loop()
{
  //SoftPWMSet(lamp, 255);
  //digitalWrite(lamp, HIGH);
  //delay(50);
  //digitalWrite(lamp, LOW);
  SoftPWMSet(led, 255);
  delay(1000);
  //SoftPWMSet(lamp, 0);
  SoftPWMSet(led, 0);
  delay(2000);
}
