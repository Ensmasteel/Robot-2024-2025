#include <Arduino.h>
#include "Communication.h"
#include "Message.h"
Communication com = Communication(&Serial);
Message message;


void setup() {

  Serial.begin(115200);
  

}

void loop() {
<<<<<<< Updated upstream
  delay(100);
  com.update();

  if (com.waitingRX()){
    //com.popOldestMessage();
    message = com.receivingBox->pull();
    Serial.println(message.x);
    Serial.println(message.y);
    Serial.println(message.aid);
    Serial.println(message.distance);
    Serial.println("################");
  }
  /*
  if (Serial.available()){
    Serial.println((Serial.readString()));
  }
  */
=======
  if (Serial.available()){
    Serial.write(Serial.read() + 10);
  }
>>>>>>> Stashed changes
}