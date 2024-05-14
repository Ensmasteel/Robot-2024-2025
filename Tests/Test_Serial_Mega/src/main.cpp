#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Serial to Laptop started");
  
  Serial1.begin(115200);
  Serial.println("*****Serial1 started*****");
  

}

void loop() {

  if (Serial1.available()){
    Serial1.write(Serial1.read());
  }

}