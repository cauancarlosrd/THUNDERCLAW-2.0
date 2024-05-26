// MOVER SERVOS DE ACORDO COM POTENCIÔMETROS. POSSÍVEL MELHORIA: 
// PARAR DE USAR LARGURA DE PUSLO E USAR OS VALORES QUE CONHEÇO 
// PARA CONTROLAR OS SERVOS, PODENDO ADAPTAR PARA CADA UM.
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define MIN_PULSE_WIDTH        700
#define MAX_PULSE_WIDTH       2350
#define FREQUENCY               60


#define potBase                 A0
#define potBraco1               A1
#define potBraco2               A2
#define potBraco3               A3
#define potGripper              A4

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int base = 0;
int braco1 = 1;
int braco2 = 2;
int braco3 = 3;
int gripper = 5;

int pulse_wide;

void setup() 
{ 
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
    
  Serial.begin(9600);

  //posInit();
}
 
 
void moveMotor(int controlIn, int motorOut)
{
  int pulse_wide, pulse_width, potVal;
  
  potVal = analogRead(controlIn);                                                   //Read value of Potentiometer
  
  pulse_wide = map(potVal, 300, 850, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);


  switch(motorOut){
    case 0: // base
      pulse_wide = map(potVal, 300, 850, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 1: // braco1
      pulse_wide = map(potVal, 120, 620, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 2: // braco2
      pulse_wide = map(potVal, 430, 990, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 3: // braco3
      pulse_wide = map(potVal, 700, 110, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;

    case 5: //gripper
      pulse_wide = map(potVal, 0, 1023, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
  }
  pulse_width = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);                //Map Potentiometer position to Motor
  
  pwm.setPWM(motorOut, 0, pulse_width); //com todas as transformações do valor recebido pelo pot, girar o servo 
  
}

void loop() {
  moveMotor(potBase, base);
  moveMotor(potBraco1, braco1);
  moveMotor(potBraco2, braco2);
  moveMotor(potBraco3, braco3);
  //moveMotor(gripper, potGripper);


}

void posInit(){ // posição inicial. Serve também pra girar tudo
  pwm.setPWM(0, 0, 335); //base na posição 3, a central
  delay(300);
  pwm.setPWM(1, 0, 380); //b1
  delay(300);
  pwm.setPWM(2, 0, 350); //b2
  delay(300);
  pwm.setPWM(3, 0, 240); //b3
  delay(300);
  pwm.setPWM(5, 0, 200);  //gripper
}
