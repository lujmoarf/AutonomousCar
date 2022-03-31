/*
BSD 2-Clause License
Copyright (c) 2020, longpth
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <WebSocketsServer.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <SparkFun_TB6612.h>

#define DEBUG
// #define SAVE_IMG

const char* ssid = "Juline_2.4G"; //replace with your wifi ssid
const char* password = "JBDL1234"; //replace with your wifi password
//holds the current upload
//int cameraInitState = -1;
//uint8_t* jpgBuff = new uint8_t[68123];
//size_t   jpgLength = 0;
//Creating UDP Listener Object. 
WiFiUDP UDPServer;
unsigned int UDPPort = 6868;
IPAddress addrRemote;
int portRemote;

// Use WiFiClient class to create TCP connections
WiFiClient tcpClient;
bool clientConnected = false;

WebSocketsServer webSocket = WebSocketsServer(86);

const int RECVLENGTH = 8;
byte packetBuffer[RECVLENGTH];

unsigned long previousMillis = 0;
unsigned long previousMillisServo = 0;
const unsigned long interval = 30;
const unsigned long intervalServo = 100;

bool bStream = false;
int debugCnt=0;

bool reqLeft = false;
bool reqRight = false;
bool reqFw = false;
bool reqBw = false;
bool motor_stop = true;
bool old_motor_stop = true;
/**********************************************
 * MOTOR DEFINITION
 **********************************************/
// Motor A
int motor1Pin1 = 5;
int motor1Pin2 = 18;
int enable1Pin = 19; 

// Motor B
int motor2Pin1 = 17;
int motor2Pin2 = 16;
int enable2Pin = 4;

int standbyPin = 2;


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

// Setting PWM properties for motor control
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 250;

// defined direction
// if direction = 0 -> forward
// if direction = 1 -> backward
int direction = 0;
int old_direction = 0;

//if turn == 0 // no turn
//if turn == 1 // right turn
//if turn ==2 // left turn
int turn = 0;
int old_turn = 0;
String car_status = "";
String old_car_status = "";

/******************************************************
 * Robot  config
 ******************************************************/
// Constant for steps in disk
const float stepcount = 20.00;  // 20 Slots in disk, change if different
// Constant for wheel diameter
const float wheeldiameter = 66.10; // Wheel diameter in millimeters

/******************************************************
 * Speed sensor config
 ******************************************************/
int PIN_speedsensorleft = 23;
int PIN_speedsensorright = 22;
volatile int count_right = 0;
volatile int count_left = 0;

volatile int distance = 0;

/******************************************************
 * Servo config
 ******************************************************/
const int PIN_SERVO_YAW   = 25;
const int PIN_SERVO_PITCH = 26;
const int SERVO_RESOLUTION = 16;

int posYaw = 90;
int posPitch = 110;
int delta = 1;
const int angleMax = 180;
uint8_t camNo = 0;

/***************************************************
 * Pin for voltage monitoring
 */
const int PIN_voltage = 34;

void processData(){
  int cb = UDPServer.parsePacket();
  if (cb) {
    UDPServer.read(packetBuffer, RECVLENGTH);
    addrRemote = UDPServer.remoteIP();
    portRemote = UDPServer.remotePort();

    String strPackage = String((const char*)packetBuffer);


    Serial.print("receive: ");
//    for (int y = 0; y < RECVLENGTH; y++){
//      Serial.print(packetBuffer[y]);
//       Serial.print("\n");
//    }
    Serial.print(strPackage);
    Serial.print(" from: ");
    Serial.println(addrRemote);


    if(strPackage.equals("whoami")){
      UDPServer.beginPacket(addrRemote, portRemote);
      String res = "ESP32-CAM";
      UDPServer.write((const uint8_t*)res.c_str(),res.length());
      UDPServer.endPacket();
      Serial.println("response");
    }else if(strPackage.equals("fwon")){
      reqFw = true;
    }else if(strPackage.equals("bwon")){
      reqBw = true;
    }else if(strPackage.equals("leon")){
      reqLeft = true;
    }else if(strPackage.equals("rion")){
      reqRight = true;
    }else if(strPackage.equals("fwoff")){
      reqFw = false;
    }else if(strPackage.equals("bwoff")){
      reqBw = false;
    }else if(strPackage.equals("leoff")){
      reqLeft = false;
    }else if(strPackage.equals("rioff")){
      reqRight = false;
    }else if(strPackage.equals("mFWon")){
      //distance = 50;
      direction = 0;
    }else if(strPackage.equals("mBWon")){
      //distance = 50;
      direction = 1;
    }else if(strPackage.equals("stopon")){
      motor_stop = true;
    }else if(strPackage.equals("stopoff")){
      motor_stop = false;
    }
  }
  memset(packetBuffer, 0, RECVLENGTH);
}



void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch(type) {
      case WStype_DISCONNECTED:
          Serial.printf("[%u] Disconnected!\n", num);
          camNo = num;
          clientConnected = false;
          break;
      case WStype_CONNECTED:
          Serial.printf("[%u] Connected!\n", num);
          clientConnected = true;
          break;
      case WStype_TEXT:
      case WStype_BIN:
      case WStype_ERROR:
      case WStype_FRAGMENT_TEXT_START:
      case WStype_FRAGMENT_BIN_START:
      case WStype_FRAGMENT:
      case WStype_FRAGMENT_FIN:
          Serial.println(type);
          break;
  }
}

void isr_count_right(){
  count_right++;
}

void isr_count_left(){
  count_left++;
}

void setup(void) {
  Serial.begin(115200);
  Serial.print("\n");
  Serial.setDebugOutput(true);
  
//  setup_motor();
/****************************************
 * Setup Speed Sensors
 */
  pinMode(PIN_speedsensorright, INPUT_PULLUP);
  attachInterrupt(PIN_speedsensorright, isr_count_right, RISING);

  pinMode(PIN_speedsensorleft, INPUT_PULLUP);
  attachInterrupt(PIN_speedsensorleft, isr_count_left, RISING);

  //WIFI INIT
  Serial.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  UDPServer.begin(UDPPort); 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // 1. 50hz ==> period = 20ms (sg90 servo require 20ms pulse, duty cycle is 1->2ms: -90=>90degree)
  // 2. resolution = 16, maximum value is 2^16-1=65535
  // From 1 and 2 => -90=>90 degree or 0=>180degree ~ 3276=>6553
  ledcSetup(4, 50, SERVO_RESOLUTION);//channel, freq, resolution
  ledcAttachPin(PIN_SERVO_YAW, 4);// pin, channel

  ledcSetup(2, 50, SERVO_RESOLUTION);//channel, freq, resolution
  ledcAttachPin(PIN_SERVO_PITCH, 2);// pin, channel

  /**********************
   * Move the robot
   */

 //  mf(CMtoSteps(50));
 //  mb(CMtoSteps(50));

  /**********************
   * monitor power setup
   */
  pinMode(PIN_voltage, INPUT);
}

void loop(void) {
  webSocket.loop();
//  if(clientConnected == true){
//      Serial.println("Client connected");
//    grabImage(jpgLength, jpgBuff);
//    webSocket.sendBIN(camNo, jpgBuff, jpgLength);
    // Serial.print("send img: ");
    // Serial.println(jpgLength);
//  }

  int v = analogRead(PIN_voltage);
//  Serial.print("Voltage = ");
//  Serial.println(v);
//  delay(500);
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    processData();
  }
  if (currentMillis - previousMillisServo >= intervalServo) {
    previousMillisServo = currentMillis;
    controlServos();
    
    if (motor_stop == false){
      //mf(distance);
      if (direction == 0){
          if (turn == 0){
            move_forward();
            car_status = "moving forward";
          } else if (turn == 1){
            turn_forward_right();
            car_status = "turn forward right";
          } else if (turn == 2){
            turn_forward_left();
            car_status = "turn forward left";
          }
      } else if (direction == 1){
        if (turn == 0){
          move_backward();
          car_status = "moving backward";
        } else if (turn == 1){
          turn_backward_right();
          car_status = "turn backward right";
        } else if (turn == 2){
          turn_backward_left();
          car_status = "turn backward left";
        }
      }

      
      
    } else {
      stop_motors();
      car_status = "stop";
    }

    if (old_car_status != car_status){
      Serial.println(car_status);
      old_car_status = car_status;
    }

    
  }
}
