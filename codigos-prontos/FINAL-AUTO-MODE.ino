/*
  aqui estão todas as funções de movimento do modo automático otimizadas. Não 
  há nada além disso, como um menu de opções ou a telinha.
*/

#include <Wire.h> // usar I2C
#include <Adafruit_PWMServoDriver.h> 


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

#define SERVO_FREQ 60 // largura de período PWM

int bases[] = {140, 240, 335, 430, 520};

double c1 = 380;
double c2 = 350;
double c3 = 240;
double c5 = 200;


void setup() {
  pwm.begin(); // inicializando módulo PWM
  pwm.setPWMFreq(SERVO_FREQ); //definindo frequência

  posInit(); // setar posição inicial
  delay(1500); // esperar um cadin
}

void loop() {
  moveObj(3, 5);
  moveObj(5, 1);
  moveObj(1, 4);
  moveObj(4, 2);
  moveObj(2, 4);
  moveObj(4, 3);
}

void moveServo(uint8_t n, int init, int end, int del){ //move um servo de cada vez
  if(init > end){
    for (int pulse = init; pulse > end; pulse--){
      pwm.setPWM(n, 0, pulse);
      delay(del);
    }
  }

  else if(init < end){
    for (int pulse = init; pulse < end; pulse++){
      pwm.setPWM(n, 0, pulse);
      delay(del);
    }
  }
}


void down(){
  move3Servos(1, 2, 3, 380, 350, 240, 170, 480, 130, 15);
  delay(200);
}

void up(){
  move3Servos(1, 2, 3, 170, 480, 130, 380, 350, 240, 15);
  delay(200);
}

void take(){ //pegar
  gripper(0); //abrir gripper

  // ir frente
  move3Servos(1, 2, 3, 170, 480, 130, 150, 420, 175, 3);
  delay(100);

  // pegar obj e levantar
  gripper(1);
  delay(300);
  pwm.setPWM(1, 0, 170);
  delay(300);

  // ir trás
  move3Servos(1, 2, 3, 170, 420, 175, 170, 480, 130, 3);
  delay(100);
}

void put(){ // colocar
  // ir frente
  move3Servos(1, 2, 3, 170, 480, 130, 170, 420, 175, 5);
  delay(500);
  
  // descer e soltar obj
  pwm.setPWM(1, 0, 155);
  delay(300);
  gripper(0);
  delay(300);

  // ir trás
  move3Servos(1, 2, 3, 155, 420, 175, 160, 480, 130, 2);
  delay(100);
}

void gripper(bool state){ // abrir ou fechar a garra
  if (state == 0) pwm.setPWM(5, 0, 130);
  else pwm.setPWM(5, 0, 260);
}

void gbase(int binit, int bend){ // girar base com um delay determinado AQUI
  moveServo(0, bases[binit-1], bases[bend-1], 3);
  delay(300);
}

void move3Servos(int servo1, int servo2, int servo3, int init1, int init2, int init3, int end1, int end2, int end3, int del){
  //função antiga
    /*int servos[] = {servo1, servo2, servo3};
    int inits[] = {init1, init2, init3};
    int ends[] = {end1, end2, end3};

    for(int j = 0; j <= 100; j++){
      for(int i = 0; i <= 2; i++){
        if(inits[i] < ends[i]) 
          pwm.setPWM(servos[i], 0, (inits[i] + (((ends[i] - inits[i])/100)*j)));

        else if(inits[i] > ends[i])
          pwm.setPWM(servos[i], 0, (inits[i] - (((inits[i] - ends[i])/100)*j)));
      }
      delay(del);
    }*/

  //função nova
    int servos[] = {servo1, servo2, servo3};
    int inits[] = {init1, init2, init3};
    int ends[] = {end1, end2, end3};

    for (int j = 0; j <= 100; j++) {
      for (int i = 0; i <= 2; i++) {
        int newPosition;
        if (inits[i] < ends[i]) {
          newPosition = inits[i] + (((float)(ends[i] - inits[i]) / 100.0) * j);
        } else if (inits[i] > ends[i]) {
          newPosition = inits[i] - (((float)(inits[i] - ends[i]) / 100.0) * j);
        } else {
          newPosition = inits[i];
        }
        pwm.setPWM(servos[i], 0, newPosition);
      }
      delay(del);
    }
}


void moveObj(int objInit, int objEnd){
  down();
  take();
  up();
  gbase(objInit, objEnd);
  down();
  put();
  up();
}

void posInit(){
  pwm.setPWM(0, 0, bases[2]); //base na posição 3, a central
  delay(300);
  pwm.setPWM(1, 0, 380); //b1
  delay(300);
  pwm.setPWM(2, 0, 350); //b2
  delay(300);
  pwm.setPWM(3, 0, 240); //b3
  delay(300);
  gripper(0); // gripper aberto
}
