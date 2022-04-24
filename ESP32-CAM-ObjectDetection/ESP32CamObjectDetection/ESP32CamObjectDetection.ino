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
#include "camera_wrap.h"

#define DEBUG
// #define SAVE_IMG

const char* ssid = "Juline_2.4G"; //replace with your wifi ssid
const char* password = "JBDL1234"; //replace with your wifi password
//holds the current upload
int cameraInitState = -1;
uint8_t* jpgBuff = new uint8_t[68123];
size_t   jpgLength = 0;
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


const int LED_BUILTIN = 4;
int ledState = LOW;

uint8_t camNo = 0;

void processData(){
  float wifiStrength;
  const int numberPoints = 7;
  int cb = UDPServer.parsePacket();
  String res;
  
  if (cb) {
    UDPServer.read(packetBuffer, RECVLENGTH);
    addrRemote = UDPServer.remoteIP();
    portRemote = UDPServer.remotePort();

    String strPackage = String((const char*)packetBuffer);
#ifdef DEBUG
    Serial.print("receive from: ");
/*    for (int y = 0; y < RECVLENGTH; y++){
      Serial.print(packetBuffer[y]);
      Serial.print("\n");
    }*/
    
    Serial.print(addrRemote);
    Serial.print(":");
    Serial.print(portRemote);
    Serial.print(" <- ");
    Serial.println(strPackage);
#endif
    if(strPackage.equals("whoami")){
      //UDPServer.beginPacket(addrRemote, portRemote);
      sendData("ESP32-CAM");
    } else if(strPackage.equals("getWifi")){
      //UDPServer.beginPacket(addrRemote, portRemote);
      wifiStrength = getStrength(numberPoints);
      sendData("WifiRxPower;" + String(wifiStrength));
    }
  }
  memset(packetBuffer, 0, RECVLENGTH);
}

void sendData(String msg){
  if (addrRemote != NULL and portRemote != NULL){
    UDPServer.beginPacket(addrRemote, portRemote);
    UDPServer.write((const uint8_t*)msg.c_str(),msg.length());
    UDPServer.endPacket();
    Serial.print("sent to ");
    Serial.print(addrRemote);
    Serial.println(":" + String(portRemote) + " -> " + msg);
  } //else {
    //Serial.println("ERROR: remote IP or remote port not defined. Found addrRemote = " + String(addrRemote) + " and portRemote = " + String(portRemote));
  //}
}


// Take measurements of the Wi-Fi strength and return the average result.
int getStrength(int points){
    long rssi = 0;
    long averageRSSI = 0;
    
    for (int i=0;i < points;i++){
        rssi += WiFi.RSSI();
        delay(20);
    }

   averageRSSI = rssi/points;
   return averageRSSI;
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

void setup(void) {
  Serial.begin(115200);
  Serial.print("\n");
  #ifdef DEBUG
  Serial.setDebugOutput(true);
  #endif

  cameraInitState = initCamera();

  Serial.printf("camera init state %d\n", cameraInitState);

  // pinMode(LED_BUILTIN, OUTPUT);

  if(cameraInitState != 0){
    digitalWrite(LED_BUILTIN, HIGH);
    return;
  }

  //WIFI INIT
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
    digitalWrite(27, HIGH);
    delay(250);    
    digitalWrite(27, LOW);
    Serial.print(".");
    delay(250);
    ii++;
    if (ii == 20){
      i = 0;
      ii = 0;
      WiFi.disconnect();
    }
  }  
  
  // digitalWrite(LED_BUILTIN, LOW);
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  UDPServer.begin(UDPPort); 
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

}

void loop(void) {
  float wifiStrength;
  const int numberPoints = 7;
  
  /*if (WiFi.status() == WL_CONNECTED) { 
    wifiStrength = getStrength(numberPoints);
    sendData("WifiRxPower;" + String(wifiStrength));
  }*/
  
  webSocket.loop();
  if(clientConnected == true){
    grabImage(jpgLength, jpgBuff);
    webSocket.sendBIN(camNo, jpgBuff, jpgLength);
    Serial.print("send img: ");
    Serial.println(jpgLength);
  }
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    processData();
  }
}
