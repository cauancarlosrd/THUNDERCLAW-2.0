/*
     Q0470
     AUTOR:   BrincandoComIdeias
     LINK:    https://www.youtube.com/brincandocomideias ; https://cursodearduino.net/
     COMPRE:  https://www.arducore.com.br/
     SKETCH:  PWM I2C
     DATA:    04/06/2019
*/

// INCLUSÃO DE BIBLIOTECAS
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// INSTANCIANDO OBJETOS
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// DECLARAÇÃO DE FUNÇÕES
void writeServos(int posicao, int tempo);
void beginServos();

void setup() {
  beginServos(); // INCIA OS SERVOS
  delay(300);
}

void loop() {

  for ( byte i = 0 ; i < 5 ; i++ ) {
    for ( byte nS = 0; nS <= 15; nS++) {
      writeServos(nS, 170);
      delay(50);
    }
    delay(50);

    for ( byte nS = 0; nS <= 15; nS++) {
      writeServos(nS, 10);
      delay(50);
    }
    delay(50);
  }

  delay(300);

  for ( byte i = 0 ; i < 5; i++) {

    for ( byte nS = 0; nS <= 15; nS++) {
      writeServos(nS, 170);
      delay(30);
    }
    delay(30);

    for ( byte nS = 0; nS <= 15; nS++) {
      writeServos(nS, 10);
      delay(30);
    }
    delay(30);

  }

  delay(300);

}

// IMPLEMENTO DE FUNÇÕES
void writeServos(int nServo, int posicao) {
#define SERVOMIN  205 // VALOR PARA UM PULSO MAIOR QUE 1 mS
#define SERVOMAX  409 // VALOR PARA UM PULSO MENOR QUE 2 mS

  int pos = map ( posicao , 0 , 180 , SERVOMIN, SERVOMAX);
  pwm.setPWM(nServo , 0, pos);
}

void beginServos() {

#define Frequencia 50 // VALOR DA FREQUENCIA DO SERVO 

  pwm.begin(); // INICIA O OBJETO PWM
  pwm.setPWMFreq(Frequencia); // DEFINE A FREQUENCIA DE TRABALHO DO SERVO
}
