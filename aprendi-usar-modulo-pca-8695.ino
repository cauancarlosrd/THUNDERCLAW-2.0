// aprendendo a utilizar o módulo PWM PCA9685. VAMBORA!!!!!!!!!!! VAI DAR CERTO!!!!!!! 

//inclusão de bibliotecas
#include <Wire.h> // usar I2C
#include <Adafruit_PWMServoDriver.h> // usar 

// criação do objeto com todas as funções e propriedades para utilizar e código do módulo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// definindo pulsos máximos dos servos e frequência (taxa de atualização)
#define SERVOMIN  130 // mínimos e máximos configurados para girar 180 graus certinho
#define SERVOMAX  610
#define SERVO_FREQ 60 // largura de período PWM

// setup
void setup() {

  pwm.begin(); // inicializando módulo PWM

  pwm.setPWMFreq(SERVO_FREQ); //definindo frequencia
}

void loop() {

  for(int i = 0; i <= 5; i++){                               // cada um dos 6 servo motores, de 0 a 5
    pwm.setPWM(i, 0, SERVOMAX);                              //girar da posição atual até o máximo(sentido horário)
    delay(1000);                                             // esperar 1 segundo

    for (int pulse = SERVOMAX; pulse > SERVOMIN; pulse--) {  
      pwm.setPWM(i, 0, pulse);                               // girar da posição atual até o mínimo(sentido anti-horário) por meio de um laço de repetição
    }
    delay(1000);                                             // esperar 1 segundo
  }

}

