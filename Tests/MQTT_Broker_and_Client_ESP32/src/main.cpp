#include <Arduino.h>

#include <WiFiMulti.h>
WiFiMulti wifiMulti;

#define WIFI_SSID "Vincent"
#define WIFI_PASS "azertyuiopq"

#include "TinyMqtt.h"
#include "TinyStreaming.h"


const uint16_t PORT = 1883;
const uint8_t  RETAIN = 10;


MqttBroker broker(PORT, RETAIN);
static MqttClient client;

int startTime = millis();

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASS);

  Serial.print("Connecting to wifi AP");

  while(wifiMulti.run() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }

  Serial.println(WiFi.localIP());
  
  Serial.println("Connected");

  broker.begin();

  client.connect(&broker);

}


void loop() {
  
  if (millis() - startTime > 10){
  
  
    client.publish("TESTS/sinewave", String(sin(2 * 3.141592 * millis() / 10000), 5));
    startTime = 0;
  }


  broker.loop();
  client.loop();

}