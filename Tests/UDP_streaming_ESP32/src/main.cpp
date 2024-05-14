#include <Arduino.h>

#include "Wifi.h"
//#include "AsyncUDP.h"
#include "HardwareSerial.h"

#include <WiFiUdp.h>

const char* ssid = "Vincent";
const char* pass = "azertyuiopq";
const uint16_t PORT = 8080;

const char * udpAddress = "192.168.69.184";


//WiFiServer server(80);
WiFiUDP    udp;

void setup() {

  Serial.begin(115200);

  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  //  WiFi.setSleep(false);

  // WiFi.localIP()

  while (WiFi.status() != WL_CONNECTED){
    delay((uint32_t)500);
  }
  Serial.println("Connected to wifi");
  Serial.println(WiFi.localIP());

  //udp.begin(WiFi.localIP(), PORT);
  udp.beginPacket(udpAddress, PORT);
}

float sinwave = 0;
int a = 0;
int b = 0;



void loop(){
  int total_time = 0;   
  for(int i = 0; i < 1000; i ++){
    sinwave = sin(2 * 3.141592654 * i /1000);
    delay(10); 
    
    a = micros();
    udp.printf("%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%d\r\n", sinwave,sinwave,sinwave,sinwave,sinwave,sinwave,sinwave,sinwave,sinwave,sinwave,sinwave, i);
    total_time += micros() - a;
    //udp.endPacket();
  }

  Serial.println(total_time/1000);
  


  
}