void setup_motor(){
  /****************************************************************************
  // SETUP THE MOTORS
  *****************************************************************************/
  // these constants are used to allow you to make your motor configuration 
  // line up with function names like forward.  Value can be 1 or -1
  const int offsetA = 1;
  const int offsetB = 1;
  
  // Initializing motors.  The library will allow you to initialize as many
  // motors as you have memory for.  If you are using functions like forward
  // that take 2 motors as arguements you can either write new functions or
  // call the function more than once.
  Motor motor1 = Motor(motor1Pin1, motor1Pin2, enable1Pin, offsetA, standbyPin);
  Motor motor2 = Motor(motor2Pin1, motor2Pin2, enable2Pin, offsetB, standbyPin);
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
  ////motor1
  //ledcWrite(pwmChannel, dutyCycle);
  //digitalWrite(motor1Pin1, HIGH);
  //digitalWrite(motor1Pin2, LOW);

  ////motor2
  //ledcWrite(pwmChannel, dutyCycle);
  //digitalWrite(motor2Pin1, HIGH);
  //digitalWrite(motor2Pin2, LOW);

  forward(motor1, motor2, 150);

  direction = 0;
  
//  Serial.print("forward with duty cycle: ");
//  Serial.println(dutyCycle);
}

void stop_motors() {
  //// Stop the DC motor
  //// Serial.println("Motor stopped");
  //digitalWrite(motor1Pin1, LOW);
  //digitalWrite(motor1Pin2, LOW);

  //digitalWrite(motor2Pin1, LOW);
  //digitalWrite(motor2Pin2, LOW);

  brake(motor1, motor2);
}

void move_backward() {
  //// Move the DC motor backward
  ////motor1
  //ledcWrite(pwmChannel, dutyCycle);
  //digitalWrite(motor1Pin1, LOW);
  //digitalWrite(motor1Pin2, HIGH);
  
  ////motor2
  //ledcWrite(pwmChannel, dutyCycle);
  //digitalWrite(motor2Pin1, LOW);
  //digitalWrite(motor2Pin2, HIGH);  

  back(motor1, motor2, -150);
  direction = 1;
  
  //Serial.print("Backward with duty cycle: ");
  //Serial.println(dutyCycle);
}

void turn_forward_left() { 
  left(motor1, motor2, 150);
}

void turn_backward_left() {
  left(motor1, motor2, -150);
}

void turn_forward_right() {
  right(motor1, motor2, 150);
}

void turn_backward_right() {
  right(motor1, motor2, -150);
}
