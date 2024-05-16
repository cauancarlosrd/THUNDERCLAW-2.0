// libraries
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH        700
#define MAX_PULSE_WIDTH       2350
#define FREQUENCY               60
#define pot                     A3

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


void setup() 
{ 
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
    
  Serial.begin(9600);
}
 
 
void moveMotor(int controlIn, int motorOut)
{
  int pulse_wide, pulse_width, potVal;
  
  potVal = analogRead(controlIn);                                                   //Read value of Potentiometer
  
  pulse_wide = map(potVal, 300, 850, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  pulse_width = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);                //Map Potentiometer position to Motor
  
  pwm.setPWM(motorOut, 0, pulse_width);
  
}

void loop() {
  int potVal = analogRead(pot);

  for(int serv = 0; serv <= 5; serv++){
    moveMotor(pot, serv);
  }
}
