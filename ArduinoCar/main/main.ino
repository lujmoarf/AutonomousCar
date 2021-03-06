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
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>


#define DEBUG
// #define SAVE_IMG

const char* ssid = "Bernie"; //replace with your wifi ssid
const char* password = "bernie0909"; //replace with your wifi password


extern void stop_motors();

//Creating UDP Listener Object. 
WiFiUDP UDPServer;
unsigned int UDPPort = 6868;
IPAddress addrRemote;
int portRemote;

AsyncWebServer server(80);



// Use WiFiClient class to create TCP connections
WiFiClient tcpClient;
bool clientConnected = false;

WebSocketsServer webSocket = WebSocketsServer(86);

const int RECVLENGTH = 8;
byte packetBuffer[RECVLENGTH];

extern volatile unsigned long previousMillis = 0;  
unsigned long previousMillisServo = 0;
const unsigned long interval = 250;
const unsigned long intervalServo = 250;

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
//#define LED 2

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

/**************************************************
 * Procedure for Serial Web Server
 */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}


void processData(){
  int cb = UDPServer.parsePacket();
  if (cb) {
    digitalWrite(27, LOW);
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
    WebSerial.print("receive: ");
    WebSerial.print(strPackage);
    WebSerial.print(" from: ");
    WebSerial.println(addrRemote);

    if(strPackage.equals("whoami")){
      UDPServer.beginPacket(addrRemote, portRemote);
      String res = "ESP32-CAM";
      UDPServer.write((const uint8_t*)res.c_str(),res.length());
      UDPServer.endPacket();
      Serial.println("response");
      WebSerial.println("response");
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
      motor_stop = false;
      direction = 0;
      move_forward();
      previousMillis = millis();
    }else if(strPackage.equals("mBWon")){
      motor_stop = false;
      direction = 1;
      move_backward();
      previousMillis = millis();
    }else if(strPackage.equals("mRGon")){
      motor_stop = false;
      turn = 1;
      if (direction == 0){
        turn_forward_right();
      } else if (direction == 1){
        turn_backward_right();
      } else {
        Serial.print("Error in mRGon: recevied Direction = ");
        Serial.print(direction);
      }
      previousMillis = millis();
    }else if(strPackage.equals("mLFon")){
      motor_stop = false;
      turn = 2;
      if (direction == 0){
        turn_forward_left();
      } else if (direction == 1){
        turn_backward_left();
      } else {
        Serial.print("Error in mRGon: recevied Direction = ");
        Serial.print(direction);
      }      
      previousMillis = millis();     
    }else if(strPackage.equals("stopon")){
      motor_stop = true;
    }
  }
  memset(packetBuffer, 0, RECVLENGTH);
  digitalWrite(27, HIGH);
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
          WebSerial.println(type);
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
  pinMode(27, OUTPUT);

  stop_motors();
  car_status = "stop";
  
  //WIFI INIT
/*  Serial.printf("Connecting to %s\n", ssid);
  if (String(WiFi.SSID()) != String(ssid)) {
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
  }

  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(27, HIGH);
    delay(250);    
    digitalWrite(27, LOW);
    Serial.print(".");
    delay(250);
  }*/

  int i = 0;
  int ii = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (i == 0){
      Serial.printf("Connecting to %s\n", ssid);
      if (String(WiFi.SSID()) != String(ssid)) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
      }
      i++;
    }
//    digitalWrite(27, HIGH);
    delay(250);    
//    digitalWrite(27, LOW);
    Serial.print(".");
    delay(250);
    ii++;
    if (ii == 20){
      i = 0;
      ii = 0;
      WiFi.disconnect();
    }
  }  
  
  
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());  


  digitalWrite(27, HIGH);
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
   * monitor power setup
   */
  pinMode(PIN_voltage, INPUT);

  
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
//  WebSerial.setDebugOutput(true);
  server.begin();
  previousMillis = millis();
}

void loop(void) {
//  digitalWrite(27, HIGH);
  webSocket.loop();
//  if(clientConnected == true){
//      Serial.println("Client connected");
//    grabImage(jpgLength, jpgBuff);
//    webSocket.sendBIN(camNo, jpgBuff, jpgLength);
    // Serial.print("send img: ");
    // Serial.println(jpgLength);
//  }

//  int v = analogRead(PIN_voltage);
//  Serial.print("Voltage = ");
//  Serial.println(v);
//  delay(500);
  
  unsigned long currentMillis = millis();
//  if (currentMillis - previousMillisServo >= intervalServo) {
//    previousMillisServo = currentMillis;
//    controlServos();
//  }

  if (currentMillis - previousMillis < interval) {
    processData();
    } else {
      stop_motors();
      car_status = "stop";
      previousMillis = currentMillis;
      processData();
    }

    if (old_car_status != car_status){
      Serial.println(car_status);
      WebSerial.println(car_status);
      old_car_status = car_status;
    }
}
