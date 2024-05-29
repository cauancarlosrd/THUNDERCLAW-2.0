#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);

#define butUp     8
#define butP      9
#define butDown  10

#define SERVO_FREQ 60

#define MIN_PULSE_WIDTH        700
#define MAX_PULSE_WIDTH       2350

#define potBase                 A0
#define potBraco1               A1
#define potBraco2               A2
#define potBraco3               A3
#define potGripper              A4

void changeMenu();
void lcdMenu();
void manual_mode();
void posInit();
void thunderclaw();
void vision_mode();
void moveObj();
void moveServo();
void down();
void up();
void take();
void put();
void gripper();
void gbase();
void move3Servos();
void moveMotor();

char menu = 0x01;
char visionStep = 0x01;
char subColor = 0x01;
char subBase = 0x01;
char subMenuVision = 0x01;
char set1 = 0x00, set2 = 0x00;
bool t_butUp, t_butDown, t_butP, t_butM;
bool noRep = 0x00;
bool offPot = 0x00;

//PYTHON
char colors[] = {0, 0, 0}; // RED, GREEN e BLUE, respectivamente
int intColors[] = {0, 0, 0}; // merma coisa

int colorSelected;
int baseSelected;

int bases[] = {140, 240, 335, 430, 520};
int lastBase = 3;

int base = 0;
int braco1 = 1;
int braco2 = 2;
int braco3 = 3;

int pulse_wide;

double c1 = 380;
double c2 = 350;
double c3 = 240;
double c5 = 200;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  for(char i = 8; i < 11; i++) pinMode(i, INPUT_PULLUP);

  t_butUp = 0x00;
  t_butDown = 0x00;
  t_butP = 0x00;

  posInit();
}

void loop() {   
  changeMenu();
  lcdMenu();
}


void changeMenu() {
  if(!digitalRead(butUp)) t_butUp = 0x01;
  //if(!digitalRead(butDown)) t_butDown = 0x01;
      
  if(digitalRead(butUp) && t_butUp) {
    t_butUp = 0x00;
    lcd.clear();
    menu++;
    if(menu > 0x04) menu = 0x01;
  }
   
  /*if(digitalRead(butDown) && t_butDown) {
    t_butDown = 0x00;
    lcd.clear();
    menu--;
    if(menu < 0x01) menu = 0x04;
  }*/
}

void lcdMenu() {
  switch(menu) {
    case 0x01:
      thunderclaw();
      break;
    case 0x02:
      manual_mode();
      break;
    case 0x03:
      auto_mode();
      break;
    case 0x04:
      displayVision();
  }
}

void displayVision(){
  switch(visionStep){
    case 0x01:
      vision_mode();
      break;
    case 0x02:
      selectColor();
      break;
    case 0x03:
      dispColor();
      break;
    case 0x04:
      selectBase();
      break;
    case 0x05:
      dispBase();
      break;
    case 0x06:
      visionMove();
      break;
  }
}
void manual_mode() {
  lcd.setCursor(0,0);
  lcd.print("Modo Manual");
  lcd.setCursor(0,1);
  lcd.print("      OFF       ");

  if(!digitalRead(butP)) t_butP = 0x01;

  if(digitalRead(butP) && t_butP){ // ligar o modo manual
    t_butP = 0x00;
    lcd.setCursor(0,1);
    lcd.print("       ON       ");

    for( ; ; ){ // loop infinito
      moveMotor(potBase, base);
      moveMotor(potBraco1, braco1);
      moveMotor(potBraco2, braco2);
      moveMotor(potBraco3, braco3);
      if(!digitalRead(butP)) t_butP = 0x01;
      if(digitalRead(butP) && t_butP){
        t_butP = 0x00;
        lcd.setCursor(0, 1);
        lcd.print("      OFF       ");
        posInit();
        break;
      }
    }
  }
}

void auto_mode() {
  lcd.setCursor(0,0);
  lcd.print("   Modo AUTO    ");

  if(!digitalRead(butP)) t_butP = 0x01;
  //if(!digitalRead(butM) && noRep != 0x00) t_butM = 0x01;

  if(digitalRead(butP) && t_butP) { // ligar o modo automático
    t_butP = 0x00;
    lcd.setCursor(0,1);
    lcd.print("       ON       ");

    moveObj(3, 5);
    moveObj(5, 3);
    gbase(lastBase, 3);

    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  /*if(digitalRead(butM) && t_butM) { //desligar o modo automático
    t_butM = 0x00;
    lcd.setCursor(0,1);
    //s_start();
    lcd.print("      OFF       ");
    noRep = 0x00;
  }*/
}

void thunderclaw() {
  lcd.setCursor(0,0);
  lcd.print("    THUNDER     "); 
  lcd.setCursor(0,1);
  lcd.print("   CLAW  2.0    ");
}

void vision_mode(){
  lcd.setCursor(0, 0);
  lcd.print("   Modo VISAO   ");
  if(!digitalRead(butP)) t_butP = 0x01;
  if(digitalRead(butP) && t_butP){
    t_butP = 0x00;
    lcd.setCursor(0, 0);
    lcd.print(" Processando... ");

    Serial.write(1); // pedindo o azul
    while(!Serial.available()) delay(1); //esperar enquanto não receber a resposta
    if(Serial.available() > 0) colors[2] = Serial.read(); // guardando o valor que o Python enviou na variável correspondente
    lcd.setCursor(0, 1);
    lcd.print("b= ");
    lcd.print(colors[2]);

    Serial.write(2); // pedindo o vermelho
    while(!Serial.available()) delay(1); //esperar enquanto não receber a resposta
    if(Serial.available() > 0) colors[0] = Serial.read(); // guardando o valor que o Python enviou na variável correspondente
    lcd.print(" r= ");
    lcd.print(colors[0]);

    Serial.write(3); // pedindo o verde
    while(!Serial.available()) delay(1); //esperar enquanto não receber a resposta
    if(Serial.available() > 0) colors[1] = Serial.read(); // guardando o valor que o Python enviou na variável correspondente
    lcd.print(" g= ");
    lcd.print(colors[1]);

    for(int i = 0; i <= 2; i++) intColors[i] = colors[i] - '0'; //pegando os chars e transformando eles em inteiros, jogando na outra array

    delay(2000);

    visionStep = 0x02;

  }
}

void selectColor(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Selecionar COR ");

  visionStep = 0x03;
}

void dispColor(){

  if(!digitalRead(butP)) t_butP = 0x01;

  if(digitalRead(butP) && t_butP){
    t_butP = 0x00;

    subColor++;
    if(subColor > 3) subColor = 0x01;
  }

  switch(subColor){
    case 0x01:
      lcd.setCursor(0, 1);
      lcd.print("    VERMELHO   >");
      readSelect(1);
      break;
    
    case 0x02:
      lcd.setCursor(0, 1);
      lcd.print("     VERDE     >");
      readSelect(2);
      break;
    
    case 0x03:
      lcd.setCursor(0, 1);
      lcd.print("      AZUL     >");
      readSelect(3);
      break;
  }
}

void readSelect(char option){
  if(!digitalRead(butDown)) t_butDown = 0x01;

  if(digitalRead(butDown) && t_butDown){
    t_butDown = 0x00;

    switch(option){
      // escolha do objeto
      case 0x01: 
        colorSelected = 0;
        visionStep = 0x04;
        break;

      case 0x02:
        colorSelected = 1;
        visionStep = 0x04;
        break;

      case 0x03:
        colorSelected = 2;
        visionStep = 0x04;
        break;

      // escolha da base
      case 0x04:
        baseSelected = 1;
        visionStep = 0x06;
        break;

      case 0x05:
        baseSelected = 2;
        visionStep = 0x06;
        break;
      
      case 0x06:
        baseSelected = 3;
        visionStep = 0x06;
        break;
      
      case 0x07:
        baseSelected = 4;
        visionStep = 0x06;
        break;
      
      case 0x08:
        baseSelected = 5;
        visionStep = 0x06;
        break;
    }
  }
}

void selectBase(){ // visionStep 0x04
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Selecionar POS ");

  visionStep = 0x05;
}

void dispBase(){
  if(!digitalRead(butP)) t_butP = 0x01;

  if(digitalRead(butP) && t_butP){
    t_butP = 0x00;

    subBase++;

    if(subBase > 5) subBase = 0x01;
  }

  switch(subBase){
    case 0x01:
      if(colors[0] != '1' && colors[1] != '1' && colors[2] != '1'){
        lcd.setCursor(0, 1);
        lcd.print("     BASE 1     ");
        readSelect(4);
        }
      else subBase++;
      break;

    case 0x02:
      if(colors[0] != '2' && colors[1] != '2' && colors[2] != '2'){
        lcd.setCursor(0, 1);
        lcd.print("     BASE 2     ");
        readSelect(5);
      }
      else subBase++;
      break;

    case 0x03:
      if(colors[0] != '3' && colors[1] != '3' && colors[2] != '3'){
        lcd.setCursor(0, 1);
        lcd.print("     BASE 3     ");
        readSelect(6);
      }
      else subBase++;
      break;

    case 0x04:
      if(colors[0] != '4' && colors[1] != '4' && colors[2] != '4'){
        lcd.setCursor(0, 1);
        lcd.print("     BASE 4     ");
        readSelect(7);
      }
      else subBase ++;
      break;

    case 0x05:
      if(colors[0] != '5' && colors[1] != '5' && colors[2] != '5'){
        lcd.setCursor(0, 1);
        lcd.print("     BASE 5     ");
        readSelect(8);
      }
      else subBase++;
      break;
  }
}

void visionMove(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Movendo...   ");
  moveObj(intColors[colorSelected], baseSelected); //... e, finalmente, realizando sua tão grandiosa tarefa

  for(int i = 0; i <= 2; i++){ //limpando os valores das cores pq mudaram de lugar.
    intColors[i] = 0;
    colors[i] = 0;
  }

  lcd.setCursor(0, 0);
  lcd.print("  Movimentacao  ");
  lcd.setCursor(0, 1);
  lcd.print("   concluída.   ");
  posInit();
  delay(3000);
  lcd.clear();

  visionStep = 0x01; // voltando tudo pro começo.
}
// FUNÇÕES DE MOVIMENTAÇÃO DOS SERVOS
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
  else pwm.setPWM(5, 0, 270);
}

void gbase(int binit, int bend){ // girar base com um delay determinado AQUI
  moveServo(0, bases[binit-1], bases[bend-1], 3);
  delay(300);
  lastBase = bend;
}

void move3Servos(int servo1, int servo2, int servo3, int init1, int init2, int init3, int end1, int end2, int end3, int del){
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
  gbase(lastBase, objInit);
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

void moveMotor(int controlIn, int motorOut) {
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
  pulse_width = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);             //Map Potentiometer position to Motor
  
  pwm.setPWM(motorOut, 0, pulse_width); //com todas as transformações do valor recebido pelo pot, girar o servo 
}
