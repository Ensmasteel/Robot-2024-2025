#include <Arduino.h>
#include <TeensyThreads.h>

uint32_t currentMillis = 0, lastMillis = 0;


void sender(){
  while (true){
    Serial1.write(1);
    //Serial2.write();
    delay(1000);
  }
}

void receiver(){
  while (true)
  {
    while(Serial1.available()){
      Serial.println(Serial1.read());
    }
    while(Serial2.available()){
      Serial.println(Serial2.read());
    }
  }
  
}

  

void setup() {
  Serial.begin(115200); // USB / laptop
  Serial1.begin(115200); // Mega
  //Serial2.begin(115200); // ESP32
  
  delay(500);


  threads.addThread(sender);
  threads.addThread(receiver);

}

void loop() {
}


