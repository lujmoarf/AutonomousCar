void setup_motor(){
  /****************************************************************************
  // SETUP THE MOTORS
  *****************************************************************************/
  // sets the pins as outputs motor1:
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);

  // sets the pins as outputs motor2:
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);
    
  // configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enable1Pin, pwmChannel);
  ledcAttachPin(enable2Pin, pwmChannel);
  ledcWrite(pwmChannel, dutyCycle);
}

// Function to convert from centimeters to steps
int CMtoSteps(float cm) {
  int result;  // Final calculation result
  float circumference = (wheeldiameter * 3.14) / 10; // Calculate wheel circumference in cm
  float cm_step = circumference / stepcount;  // CM per Step
  
  float f_result = cm /cm_step;  // Calculate result as a float
  result = (int) f_result; // Convert to an integer (note this is NOT rounded)
  
  return result;  // End and return result
}

void mf(int steps){
  count_right = 0;
  count_left = 0;
  
  while (steps > count_right && steps > count_left){
    move_forward();
    distance = distance - min(count_right, count_left);
    Serial.print("Forward with duty cycle = ");
    Serial.print(dutyCycle);
    Serial.print(" Counter right = ");
    Serial.print(count_right);
    Serial.print(" - Counter left = ");
    Serial.println(count_left);
  }

  stop_motors();
}

void mb(int steps){
  count_right = 0;
  count_left = 0;

  while (steps > count_right && steps > count_left){
    move_backward();
    Serial.print("Backward with duty cycle = ");
    Serial.print(dutyCycle);
    Serial.print(" Counter right = ");
    Serial.print(count_right);
    Serial.print(" - Counter left = ");
    Serial.println(count_left);
  }

  stop_motors();
}


void move_forward() {
  // Move the DC motor forward at maximum speed
  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);

  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);

  direction = 0;
  
//  Serial.print("forward with duty cycle: ");
//  Serial.println(dutyCycle);
}

void stop_motors() {
  // Stop the DC motor
  Serial.println("Motor stopped");
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);

  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}

void move_backward() {
  // Move the DC motor backward at maximum speed

  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  
  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);  

  direction = 1;
  
  //Serial.print("Backward with duty cycle: ");
  //Serial.println(dutyCycle);
}

void turn_forward_left() {
  // STOP M1 and MOVE M2 forward
  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  
  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);  
  
  Serial.print("Turning forward left with duty cycle: ");
  Serial.println(dutyCycle);

  delay(500);
  move_forward();
}

void turn_backward_left() {
  // STOP M1 and MOVE M2 backward

  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  
  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);  
  
  Serial.print("Turning backward left with duty cycle: ");
  Serial.println(dutyCycle);

  delay(500);
  move_backward();
}

void turn_forward_right() {
  // STOP M2 and MOVE M1 forward

  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  
  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);  
  
  Serial.print("Turning forwardRight with duty cycle: ");
  Serial.println(dutyCycle);

  delay(500);
  move_forward();
}

void turn_backward_right() {
  // STOP M2 and MOVE M1 backward

  //motor1
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  
  //motor2
  ledcWrite(pwmChannel, dutyCycle);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);  
  
  Serial.print("Turning backward Right with duty cycle: ");
  Serial.println(dutyCycle);

  delay(500);
  move_backward();
  
}
