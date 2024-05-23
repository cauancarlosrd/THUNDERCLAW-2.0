#include <Wire.h> // usar I2C
#include <Adafruit_PWMServoDriver.h> 

#include <LiquidCrystal_I2C.h>

// criação do objeto com todas as funções e propriedades para utilizar e código do módulo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// definindo pulsos máximos dos servos e frequência (taxa de atualização)
#define SERVOMIN  130 // mínimos e máximos configurados para girar 180 graus certinho
#define SERVOMAX  610
#define SERVO_FREQ 60 // largura de período PWM

int bases[] = {110, 210, 305, 400, 500};

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

// setup
void setup() {

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pwm.begin(); // inicializando módulo PWM
  pwm.setPWMFreq(SERVO_FREQ); //definindo frequencia

  lcd.setCursor(0,0);

  lcd.print("THUNDERCLAW 2.0 ");
  delay(200);

  posInit(); // setar posição inicial
  delay(1500);

  lcd.setCursor(0,1);
  }

void loop(){
  agaichar();

  take(1, 5);
  take(3, 1);
  take(2, 4);
  take(1, 2);
  take(5, 3);
  take(2, 5);
  take(3, 1);
  take(5, 3);
  take(4, 2);

  voltar();

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

void posInit(){
  pwm.setPWM(0, 0, 330); //base
  delay(500);
  pwm.setPWM(1, 0, 450); //b1
  delay(200);
  pwm.setPWM(2, 0, 400); //b2
  delay(500);
  pwm.setPWM(3, 0, 400); //b3
  delay(500);
  pwm.setPWM(5, 0, 50);  //gripper
}

void agaichar(){
  for(int i = 200; i >= 0; i--){
    pwm.setPWM(1, 0, (250+i));
    pwm.setPWM(3, 0, (200+i));
    delay(5);
  }

  delay(300);
}

void pegar(){
  moveServo(2, 400, 500, 8); //abaixando
  delay(300);

  moveServo(5, 50, 190, 1);  //agarrando
  delay(500);

  moveServo(2, 500, 400, 9); //levantando
  delay(500);
}

void colocar(){
  moveServo(2, 400, 500, 9);    // pondo no chão
  delay(300);

  moveServo(5, 190, 50, 1);    // soltando
  delay(500);

  moveServo(2, 500, 400, 9);    // levantando
  delay(500);


}

void gbase(int binit, int bend){

  moveServo(0, bases[binit-1], bases[bend-1], 7);
  delay(300);

}

void voltar(){
  for(int i = 0; i <= 200; i++){
    pwm.setPWM(1, 0, (450-i));
    pwm.setPWM(3, 0, (400-i));
    delay(5);
  }
  delay(500);
}

void take(int obj, int pos){ //valores de base inseridos
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
