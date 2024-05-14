#include <Arduino.h>
#include "Communication.h"
#include "Message.h"
#include "IDS.h"
#include "iostream"
#include "Logger.h"
using namespace std;

Message mess;
Message mess2;
Communication com = Communication(&Serial2);
uint32_t topWarn;
uint32_t currentMillis;
int compteur=0;

#define FREQUENCY 100





void setup() {
  mess = newMessageCoordonnees(Teensy,ESP32,305,480);

  mess2.did=EmptyD;
  mess2.s = Teensy;
  mess2.d = ESP32;
  mess2.x = 2000; 
  mess2.y = 40;
  mess2.aid = PaletRose;
  mess2.distance = 10;  

  Serial.begin(115200);
  Serial2.begin(115200);
  Logger::setup(&Serial, &Serial, true, true  );
  Serial.println("setup");

}

void loop() {
  currentMillis = millis();
  if ((currentMillis - topWarn) / 1e3 >= 1.0) //Affichage toutes les 25 secondes de la freq moyenne
  {
    
    Serial.println("debut");
    if (compteur%2==0){
      com.send(mess);
    }
    else{
      com.send(mess2);
    }
    Serial.println("send");
    topWarn=currentMillis;
    com.update();
    Serial.println("update");
    Serial.println("fin");
    compteur++;
    
  }

}