#include <Wire.h> // usar I2C
#include <Adafruit_PWMServoDriver.h> 

#include <LiquidCrystal_I2C.h>

// criação do objeto com todas as funções e propriedades para utilizar e código do módulo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// definindo pulsos máximos dos servos e frequência (taxa de atualização)
#define SERVOMIN  130 // mínimos e máximos configurados para girar 180 graus certinho
#define SERVOMAX  610
#define SERVO_FREQ 60 // largura de período PWM

int bases[] = {140, 240, 335, 430, 520};
double c1 = 380;
double c2 = 350;
double c3 = 240;
double c5 = 200;

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// setup
void setup() {

  // DESCOMENTAR AQUI!!!!!!!!!!!!!!!!
  /*lcd.init();
  lcd.backlight();
  lcd.clear();*/

  pwm.begin(); // inicializando módulo PWM
  pwm.setPWMFreq(SERVO_FREQ); //definindo frequencia

  //lcd.setCursor(0,0);
  //lcd.print("THUNDERCLAW 2.0 ");

  posInit(); // setar posição inicial
  delay(2000);


  /* pwm.setPWM(1, 0, 160);
    delay(300);
  pwm.setPWM(3, 0, 130);
    delay(300);
  pwm.setPWM(2, 0, 480); */
  //delay(1500);

  lcd.setCursor(0,1);


  /*for(double i = 0; i <= 90; i++){
    pwm.setPWM(1, 0, 160-(i/6));
    pwm.setPWM(2, 0, 480-i);
    pwm.setPWM(3, 0, 130+(i/1.3));
    delay(9);
  }*/
  
  }



void loop(){

  for(int i = 0; i <= 100; i++){ // descer
    pwm.setPWM(1, 0, c1);
    pwm.setPWM(2, 0, c2);
    pwm.setPWM(3, 0, c3);
    c1 -= 2.2;
    c2 += 1.3;
    c3 -= 1.1;
    delay(8);
  }

  gripper(0);

  delay(500);


  // POR LÁ
    for(double i = 0; i <= 60; i++){
        pwm.setPWM(2, 0, 480-i);
        pwm.setPWM(3, 0, 130+(i/1.3));
        pwm.setPWM(1, 0, 160-(i/6));
        delay(9);
      }

    //CATAR
    delay(300);
    gripper(1);
    delay(300);
    pwm.setPWM(1, 0, 155);
    delay(300);


    // voltando
    for(double i = 60; i >= 0; i--){
      pwm.setPWM(2, 0, 480-i);
      pwm.setPWM(3, 0, 130+(i/1.3));
      pwm.setPWM(1, 0, 170-(i/6));
      delay(7);
    }
    delay(700);

  // SUBINDO
    for(int i = 100; i >= 0; i--){
      pwm.setPWM(1, 0, c1);
      pwm.setPWM(2, 0, c2);
      pwm.setPWM(3, 0, c3);
      c1 += 2.2;
      c2 -= 1.3;
      c3 += 1.1;
      delay(20);
    }
      delay(500);


  // GIRANDO
   gbase(3, 2);


  // DESCENDO
    for(int i = 0; i <= 100; i++){ // descendo
      pwm.setPWM(1, 0, c1);
      pwm.setPWM(2, 0, c2);
      pwm.setPWM(3, 0, c3);
      c1 -= 2.2;
      c2 += 1.3;
      c3 -= 1.1;
      delay(20);
    }


  //POR DE VOLTA
    // indo
    for(double i = 0; i <= 60; i++){
      pwm.setPWM(2, 0, 480-i);
      pwm.setPWM(3, 0, 130+(i/1.3));
      pwm.setPWM(1, 0, 170-(i/6));
      delay(9);
    }
    delay(500);

    pwm.setPWM(1, 0, 150);
    delay(500);
    
    // CATAR
    gripper(0);
    delay(500);
    pwm.setPWM(1, 0, 155);
    delay(500);


    // voltando
    for(double i = 60; i >= 0; i--){
      pwm.setPWM(2, 0, 480-i);
      pwm.setPWM(3, 0, 130+(i/1.3));
      pwm.setPWM(1, 0, 170-(i/6));
      delay(7);
    }

    delay(500);

  // SUBINDO
    for(int i = 100; i >= 0; i--){
      pwm.setPWM(1, 0, c1);
      pwm.setPWM(2, 0, c2);
      pwm.setPWM(3, 0, c3);
      c1 += 2.2;
      c2 -= 1.3;
      c3 += 1.1;
      delay(8);
    }
    delay(500);

  gbase(2, 3);

  delay(1000);


  //agaichar();
  // take(1, 5);
  /*take(3, 1);
  take(2, 4);
  take(1, 2);
  take(5, 3);
  take(2, 5);
  take(3, 1);
  take(5, 3);
  take(4, 2);*/

  //voltar();


  /*pwm.setPWM(1, 0, 160); //DESCENDOOO
  pwm.setPWM(2, 0, 480);
  pwm.setPWM(3, 0, 110);
  delay(500);
  pwm.setPWM(1, 0, 160); 
  pwm.setPWM(2, 0, 400);
  pwm.setPWM(3, 0, 190);
  delay(500);*/
  



// IR PEGAR
  // indo
  /*       for(double i = 0; i <= 60; i++){
    pwm.setPWM(2, 0, 480-i);
    pwm.setPWM(3, 0, 130+(i/1.3));
    pwm.setPWM(1, 0, 160-(i/6));
    delay(9);
  }

  //CATAR
  delay(700);
  pwm.setPWM(5, 0, 180);
  delay(500);

  pwm.setPWM(1, 0, 155);
  delay(500);


  // voltando
  for(double i = 60; i >= 0; i--){
    pwm.setPWM(2, 0, 480-i);
    pwm.setPWM(3, 0, 130+(i/1.3));
    pwm.setPWM(1, 0, 170-(i/6));
    delay(7);
  }
  delay(700);

//POR DE VOLTA
  // indo
  for(double i = 0; i <= 60; i++){
    pwm.setPWM(2, 0, 480-i);
    pwm.setPWM(3, 0, 130+(i/1.3));
    pwm.setPWM(1, 0, 170-(i/6));
    delay(9);
  }
  delay(700);

  pwm.setPWM(1, 0, 150);
  delay(500);
  
  // CATAR
  pwm.setPWM(5, 0, 130);
  delay(500);

  pwm.setPWM(1, 0, 155);
  delay(500);


  // voltando
  for(double i = 60; i >= 0; i--){
    pwm.setPWM(2, 0, 480-i);
    pwm.setPWM(3, 0, 130+(i/1.3));
    pwm.setPWM(1, 0, 170-(i/6));
    delay(7);
  }
  delay(700);            */
}








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

void gripper(bool state){
  if (state == 0) pwm.setPWM(5, 0, 110);
  else pwm.setPWM(5, 0, 180);
}

void posInit(){ // posição inicial. Serve também pra girar tudo
  pwm.setPWM(0, 0, bases[2]); //base na posição 3, a central
  delay(300);
  pwm.setPWM(1, 0, 380); //b1
  delay(300);
  pwm.setPWM(2, 0, 350); //b2
  delay(300);
  pwm.setPWM(3, 0, 240); //b3
  delay(300);
  pwm.setPWM(5, 0, 200);  //gripper
}


void voltar(){ //voltar pro posInit
  // vao estar nas posições 250 e 200 o 1 e o 3, respectivamente
  for(int i = 0; i <= 200; i++){
    pwm.setPWM(1, 0, 250+i);
    pwm.setPWM(3, 0, 200+i);
    delay(7);
  }
  delay(500);
}

void pegar(){ // abaixar, pegar o objeto, voltar pra posInit
  moveServo(2, 400, 500, 8); //abaixando
  delay(300);

  moveServo(5, 50, 190, 1);  //agarrando
  delay(500);

  moveServo(2, 500, 400, 9); //levantando
  delay(500);
}

void colocar(){ // abaixar, soltar objeto, voltar pra posInit
  moveServo(2, 400, 500, 9);    // pondo no chão
  delay(300);

  moveServo(5, 190, 50, 1);    // soltando
  delay(500);

  moveServo(2, 500, 400, 9);    // levantando
  delay(500);


}

void gbase(int binit, int bend){ // girar base com um delay determinado AQUI

  moveServo(0, bases[binit-1], bases[bend-1], 5);
  delay(1000);

}

void take(int obj, int pos){ // posição inicial, posição final e levar de um pro outro
  lcd.print(obj);
  lcd.print(" para ");
  lcd.print(pos);
  gbase(3, obj);
  pegar();
  gbase(obj, pos);

  colocar();
  lcd.clear();
  gbase(pos, 3);
  
}

/*void agaichar(){ // inutilizado. transformou-se no posInit
   //for(int i = 200; i >= 0; i--){
    //pwm.setPWM(1, 0, (250+i));
    //pwm.setPWM(3, 0, (200+i));
    //delay(7);
  //} 

  pwm.setPWM(1, 0, 380);
  pwm.setPWM(2, 0, 350);
  pwm.setPWM(3, 0, 400);

  delay(300);
}*/
