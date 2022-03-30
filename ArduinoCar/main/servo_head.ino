void servoWrite(uint8_t channel, uint8_t angle) {
  // regarding the datasheet of sg90 servo, pwm period is 20 ms and duty is 1->2ms
  uint32_t maxDuty = (pow(2,SERVO_RESOLUTION)-1)/10; 
  uint32_t minDuty = (pow(2,SERVO_RESOLUTION)-1)/20; 
  uint32_t duty = (maxDuty-minDuty)*angle/180 + minDuty;
  ledcWrite(channel, duty);
}

void controlServos(){
  if(reqFw){
    if(posPitch<160){
      posPitch += 1;
    }
  }
  if(reqBw){
    if(posPitch>0){
      posPitch -= 1;
    }
  }
  if(reqLeft){
    if(posYaw<180){
      posYaw += 1;
    }
  }
  if(reqRight){
    if(posYaw>0){
      posYaw -= 1;
    }
  }
  
  servoWrite(2,posPitch);
  servoWrite(4,posYaw);
}
