// CONTINUAR: CRIAR VARIÁVEIS DIFERENTES PARA CADA MODO OU ARRANJAR UM JEITO DE LIMPAR AS VARIÁVEIS NA TROCA DE MENU. 
// PRÓXIMO OBJETIVO: FAZER FUNCIONAR O MODO MANUAL COM O POTENCIÔMETRO



// --- Bibliotecas Auxiliares ---
#include <LiquidCrystal_I2C.h>                              //Biblioteca para o lcdlay LCD

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// --- Mapeamento de Hardware ---

  #define butUp     8                                     //Botão para selecionar tela acima no digital 12
  #define butDown  11                                     //Botão para selecionar tela abaixo no digital 11
  #define butP      9                                     //Botão de ajuste mais no digital 10
  #define butM     10                                     //Botão de ajuste menos no digital 9
  #define Lamp1     2                                     //Saída para lâmpada 1 no A0 (será usado como digital)
  #define Lamp2     5                                     //Saída para lâmpada 2 no A1 (será usado como digital)

  #define SERVOMIN  130 
  #define SERVOMAX  610 
  #define SERVO_FREQ 60

  #define pot        A3 

// --- Protótipo das Funções Auxiliares ---
  void changeMenu();                                      //Função para modificar o menu atual
  void lcdMenu();                                         //Função para mostrar o menu atual
  void data_hora();                                       //Função do menu1, data e hora
  void temperatura();                                     //Função do menu2, temperatura
  void lights();                                          //Função do menu3, acionamento de lampadas
  void menu4();                                           //Função do menu4
  
  void s_start();
  void s_end();

  void potMode();
  void moveMotor();

// --- Variáveis Globais ---
char menu = 0x04;                                       //Variável para selecionar o menu
char set1 = 0x00, set2 = 0x00;                          //Controle dos LED's. 0x00 equivale a 0, que na tabela ASCII corresponde ao caractere de controle nul(por isso a variável char).
bool t_butUp, t_butDown, t_butP, t_butM;                //Flags para armazenar o estado dos botões
bool noRep = 0x00;
bool offPot = 0x00;

// --- Hardware do LCD ---
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27, 16, 2);




// --- Configurações Iniciais ---
void setup()
{
  lcd.init();                                            //Inicializa LCD 16 x 2
  lcd.backlight();                                       //Liga a backlight do LCD

  //config PWM
  pwm.begin();
  pwm.setPWMFreq(SERVO_FREQ);

  for(char i=8; i<12; i++) pinMode(i, INPUT_PULLUP);     //Entrada para os botões (digitais 9 a 12) com pull-ups internos
  
  pinMode(Lamp1, OUTPUT);                                //Configura saída para lâmpada 1
  pinMode(Lamp2, OUTPUT);                                //Configura saída para lâmpara 2
  
  t_butUp   = 0x00;                                      //limpa flag do botão Up
  t_butDown = 0x00;                                      //limpa flag do botão Down
  t_butP    = 0x00;                                      //limpa flag do botão P
  t_butM    = 0x00;                                      //limpa flag do botão M
  
  digitalWrite(Lamp1, LOW);                              //Lâmpada 1 inicia apagada
  digitalWrite(Lamp2, LOW);                              //Lâmpada 2 inicia apagada


} //end setup


// --- Loop Infinito ---
void loop()
{   
   changeMenu();
   lcdMenu();

} //end loop

// --- Desenvolvimento das Funções Auxiliares ---

void s_end(){
  for(int i = SERVOMIN; i < SERVOMAX; i++)
    for(int j = 0; j <= 5; j++) pwm.setPWM(j, 0, i);
}

void s_start(){
  for(int i = SERVOMAX; i > SERVOMIN; i--)
    for(int j = 0; j <= 5; j++) pwm.setPWM(j, 0, i);
}

void moveMotor(int controlIn, int motorOut)
{
  int pulse_wide, pulse_width, potVal;
  
  potVal = analogRead(controlIn);                                                   //Read value of Potentiometer
  
  pulse_wide = map(potVal, 800, 240, SERVOMIN, SERVOMAX);
  pulse_width = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);                //Map Potentiometer position to Motor
  
  pwm.setPWM(motorOut, 0, pulse_width);
}

void potMode(){
  for(int serv = 0; serv <= 5; serv++){ 
    if(offPot == 0x01) break;
    moveMotor(pot, serv);
  }
}

void changeMenu()                                       //Modifica o menu atual
{
   if(!digitalRead(butUp))   t_butUp   = 0x01;          //Botão Up pressionado? Seta flag
   if(!digitalRead(butDown)) t_butDown = 0x01;          //Botão Down pressionado? Seta flag
      
   if(digitalRead(butUp) && t_butUp)                    //Botão Up solto e flag setada?
   {                                                    //Sim...
      t_butUp = 0x00;                                   //Limpa flag
      
      lcd.clear();                                     //Limpa lcdlay
      menu++;                                           //Incrementa menu
      
      if(menu > 0x04) menu = 0x01;                      //Se menu maior que 4, volta a ser 1
   
   } //end butUp
   
   if(digitalRead(butDown) && t_butDown)                //Botão Down solto e flag setada?
   {                                                    //Sim...
      t_butDown = 0x00;                                 //Limpa flag
      
      lcd.clear();                                     //Limpa lcdlay
      menu--;                                           //Decrementa menu
      
      if(menu < 0x01) menu = 0x04;                      //Se menu menor que 1, volta a ser 4
   
   } //end butDown   

} //end changeMenu

void lcdMenu()                                         //Mostra o menu atual
{
    switch(menu)                                        //Controle da variável menu
    {
       case 0x01:                                       //Caso 1
             data_hora();                               //Chama a função do relógio
             
             break;                                     //break
       case 0x02:                                       //Caso 2
             servos();                             //Chama a função do termômetro
             
             break;                                     //break
       case 0x03:                                       //Caso 3
             lights();                                  //Chama função para acionamento de lâmpadas
            
             break;                                     //break
       case 0x04:                                       //Caso 4
             menu4();                                   //Chama função para o menu4
             
             break;                                     //break
    
    } //end switch menu

} //end lcdMenu

void data_hora()                                        //Data e Hora (menu1)
{
    
  lcd.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
  lcd.print("Modo Manual");                           //Imprime mensagem
  lcd.setCursor(0,1);                                 //Posiciona cursor na coluna 1, linha 2
  
  if(!digitalRead(butP)) t_butP = 0x01;
  if(!digitalRead(butM)) t_butM = 0x01;

  if(digitalRead(butP) && t_butP) {
    potMode();
    lcd.setCursor(0,1);
    lcd.print("     Pot ON    ");
  }

  if(digitalRead(butM) && t_butM) {
    offPot = 0x01;
    lcd.setCursor(0,1);
    lcd.print("     Pot OFF   ");
  }
} //end data_hora

void servos(){

  lcd.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
  lcd.print("Servo Motores");                                //Imprime mensagem

  if(!digitalRead(butP) && noRep != 0x01) t_butP = 0x01;
  if(!digitalRead(butM) && noRep != 0x00) t_butM = 0x01;

  if(digitalRead(butP) && t_butP) { // entrar na função de girar os servos para 180°

    t_butP = 0x00;

    lcd.setCursor(0,1);
    s_end();

    lcd.print("    180 graus   ");
    
    noRep = 0x01;

  }

  if(digitalRead(butM) && t_butM) {

    t_butM = 0x00;

    lcd.setCursor(0,1);

    s_start();
    lcd.print("    0 graus     ");

    noRep = 0x00;
  }

} //end servos()
 
/*==============================================================================*/

void lights()                                           //Acionamento de lâmpadas (menu3)
{
  
   lcd.setCursor(0,0);                                 //Posiciona cursor na coluna 1, linha 1
   lcd.print("Ligar LED's");                           //Imprime mensagem do menu 3
  
  
   if(!digitalRead(butP))    t_butP    = 0x01;          //Botão P pressionado? Seta flag
   if(!digitalRead(butM))    t_butM    = 0x01;          //Botão M pressionado? Seta flag
   
   if(digitalRead(butP) && t_butP)                      //Botão P solto e flag setada?
   {                                                    //Sim...
      t_butP = 0x00;                                    //Limpa flag
      
      set1++;                                           //Incrementa set1
      
      if(set1 > 2) set1 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set1)                                      //Controle do set1
      {
          case 0x01:                                    //Caso 1
                lcd.setCursor(0,1);                     //Posiciona cursor na coluna 1, linha 2
                lcd.print("L1 on ");                    //Imprime mensagem
                digitalWrite(Lamp1, HIGH);              //Liga lâmpada 1
                break;                                  //Break
          case 0x02:                                    //Caso 2
                lcd.setCursor(0,1);                     //Posiciona cursor na coluna 1, linha 2
                lcd.print("L1 off");                    //Imprime mensagem
                digitalWrite(Lamp1, LOW);               //Desliga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butP
   
   if(digitalRead(butM) && t_butM)                      //Botão D solto e flag setada?
   {                                                    //Sim...
      t_butM = 0x00;                                    //Limpa flag
      
      set2++;                                           //Incrementa set2
      
      if(set2 > 2) set2 = 0x01;                         //Se maior que 2, volta a ser 1
      
      switch(set2)                                      //Controle do set2
      {
          case 0x01:                                    //Caso 1
                lcd.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                lcd.print("L2 on ");                   //Imprime mensagem
                digitalWrite(Lamp2, HIGH);              //Liga lâmpada 2
                break;                                  //Break
          case 0x02:                                    //Caso 2
                lcd.setCursor(8,1);                    //Posiciona cursor na coluna 8, linha 2
                lcd.print("L2 off");                   //Imprime mensagem
                digitalWrite(Lamp2, LOW);               //Liga lâmpada 1
                break;                                  //Break
      
      } //end switch set1
   
   } //end butM

} //end lights

/*==============================================================================*/

void menu4()                                            //Função genérica para um quarto menu...
{
   lcd.setCursor(0,0);                                 
   lcd.print("    THUNDER     "); 
   lcd.setCursor(0,1);
   lcd.print("     CLAW       ");   


} //end menu4
