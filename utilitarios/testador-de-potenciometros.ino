//TESTADOR DE POTENCIÔMETROS PARA SERVOS
//imprime no Monitor Serial os valores de cada um e a angulatura de 0 a 180°

// define as portas dos potenciometros
#define pot1 A0
#define pot2 A1
#define pot3 A2
#define pot4 A3
#define pot5 A4

// cria as variavies dos angulos de cada motor
int motor1,motor2,motor3,motor4,motor5;

unsigned long mostradorTimer = 1;
const unsigned long intervaloMostrador = 5000;

void setup() {

  //inicia o monitor serial
  Serial.begin(9600); 

  // atribui pinos dos servos
  
}

void loop(){

  // leitura dos potenciometros
  motor1 = map(analogRead(pot1),0,1023,0,180);
  motor2 = map(analogRead(pot2),0,1023,0,180);
  motor3 = map(analogRead(pot3),0,1023,0,180);
  motor4 = map(analogRead(pot4),0,1023,0,180);
  motor5 = map(analogRead(pot5),0,1023,0,180);


   if ((millis() - mostradorTimer) >= intervaloMostrador) {

  // envio para o monitor serial do posicionamentos dos motores
  Serial.println("**********************************************");
  
  Serial.print("Pot1:");
  Serial.print(analogRead(pot1));
  Serial.print(" Angulo Motor1:");
  Serial.println(motor1);

  Serial.print("Pot2:");
  Serial.print(analogRead(pot2));
  Serial.print(" Angulo Motor2:");
  Serial.println(motor2);

  Serial.print("Pot3:");
  Serial.print(analogRead(pot3));
  Serial.print(" Angulo Motor3:");
  Serial.println(motor3);

  Serial.print("Pot4:");
  Serial.print(analogRead(pot4));
  Serial.print(" Angulo Motor4:");
  Serial.println(motor4);

  Serial.print("Pot5:");
  Serial.print(analogRead(pot5));
  Serial.print(" Angulo Motor5:");
  Serial.println(motor5);

  mostradorTimer = millis();
  }

  // tempo de espera para recomeçar
  delay(100);

}
