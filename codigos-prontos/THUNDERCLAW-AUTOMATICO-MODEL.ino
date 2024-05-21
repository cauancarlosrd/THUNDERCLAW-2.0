#include <Wire.h> // usar I2C
#include <Adafruit_PWMServoDriver.h> 

// criação do objeto com todas as funções e propriedades para utilizar e código do módulo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// definindo pulsos máximos dos servos e frequência (taxa de atualização)
#define SERVOMIN  130 // mínimos e máximos configurados para girar 180 graus certinho
#define SERVOMAX  610
#define SERVO_FREQ 60 // largura de período PWM

int bases[] = {135, 235, 330, 425, 525};

// setup
void setup() {

  pwm.begin(); // inicializando módulo PWM
  pwm.setPWMFreq(SERVO_FREQ); //definindo frequencia

  delay(1000); // delay pra posição inicial
  posInit(); // setar posição inicial
  delay(1500);

  // ATENÇÃO: A GARRA FOI REMONTADA. TALVEZ AS POSIÇÕES DAS FUNÇÕES moveServo VÃO DAR ERRADO. QUEM SABE, CONSTRUA UMA MONTAGEM DA GARRA PADRÃO PARA QUE NÃO SEJA NECESSÁRIO ALTERAR ESTES VALORES NA PRÓXIMA MONTAGEM DA GARRA.
  
  take(3, 2);
  take(2, 1);
  take(1, 5);
  take(5, 4);
  take(4, 3);

  voltar();

  }

void loop(){}

void moveServo(uint8_t n, int init, int end, int del){
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

void posInit(){
  pwm.setPWM(0, 0, 330); //base
  delay(500);
  pwm.setPWM(1, 0, 500); //b1
  delay(200);
  pwm.setPWM(2, 0, 400); //b2
  delay(500);
  pwm.setPWM(3, 0, 400); //b3
  delay(500);
  pwm.setPWM(5, 0, 50);  //gripper
}

void agaichar(){
  moveServo(1, 500, 250, 7);
  delay(500);

  moveServo(3, 400, 200, 9);
  delay(500);
}

void pegar(){
  moveServo(2, 400, 500, 8); //abaixando
  delay(500);

  moveServo(5, 50, 250, 8);  //agarrando
  delay(500);

  moveServo(2, 500, 400, 9); //levantando
  delay(500);
}

void colocar(){
  moveServo(2, 400, 500, 9);    // pondo no chão
  delay(500);

  moveServo(5, 250, 50, 10);    // soltando
  delay(500);

  moveServo(2, 500, 400, 9);    // levantando
  delay(500);

}

void gbase(int binit, int bend){
  moveServo(0, bases[binit-1], bases[bend-1], 20);
  delay(700);
}

void voltar(){
  moveServo(1, 250, 450, 7);
  delay(500);

  moveServo(3, 200, 400, 9);
  delay(500);

}

void take(int obj, int pos){ //valores de base inseridos
  gbase(3, obj);
  agaichar();
  pegar();
  gbase(obj, pos);

  colocar();

  gbase(pos, 3);

}
