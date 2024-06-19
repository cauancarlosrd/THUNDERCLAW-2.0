void moveMotor(int controlIn, int motorOut) {
  int pulse_wide, pulse_width, potVal;
  
  potVal = analogRead(controlIn);                                                   //Read value of Potentiometer
  
  pulse_wide = map(potVal, 300, 850, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);

  switch(motorOut){
    case 0: // base
      pulse_wide = map(potVal, 150, 500, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 1: // braco1
      pulse_wide = map(potVal, 60, 700, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 2: // braco2
      pulse_wide = map(potVal, 0, 600, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
    
    case 3: // braco3
      pulse_wide = map(potVal, 800, 50, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;

    case 5: //gripper
      pulse_wide = map(potVal, 0, 1023, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
      break;
  }
  pulse_width = int(float(pulse_wide) / 1000000 * SERVO_FREQ * 4096);             //Map Potentiometer position to Motor
  
  pwm.setPWM(motorOut, 0, pulse_width); //com todas as transformações do valor recebido pelo pot, girar o servo 
}
