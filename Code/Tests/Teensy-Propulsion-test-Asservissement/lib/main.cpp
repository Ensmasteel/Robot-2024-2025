#include <Arduino.h>
#include <Logger.h>
#include "Vector.h"
#include "Math_functions.h"
#include "Ghost.h"
#include "Motor.h"
#include "ActionManager.h"

#include "Communication.h"
#include "Message.h"
#include "pinSetup.h"

#include "TeensyThreads.h"

#define FREQUENCY 100
#define dt 1/FREQUENCY

Communication comMega = Communication(&Serial1);
Communication comESP = Communication(&Serial2);

ActionManager BBB = ActionManager(&comESP,&comMega);

Threads::Mutex mut;





void threadCommunication(){
  while(1){
    //comMega.update();
    //Serial.println(comESP.waitingRX());
    comESP.update();
    threads.yield();
  }
}



void threadAction(){
  while(1){
    while(!mut.getState() && !BBB.getEnnemy()){
      threads.delay(10);
      BBB.update(dt);
      BBB.robot->kineticCurrent.printDebug("Robot");
      BBB.robot->ghost.getControllerKinetic().printDebug("Ghost");
    }
    threads.yield();
  }
}

void threadEvitement(){
  while(1){
    if (!mut.getState() && BBB.getEnnemy()){
      mut.lock();
      BBB.setIdAction(0);
      BBB.evitement();
    }
    threads.yield();
  }
}

void threadArretUrgence(){
  while(1){
    if (digitalRead(PIN_ARRET_URGENCE)==LOW){
      BBB.stopMovement();
      threads.stop();
    }
    threads.yield();
  }
  
}



void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Logger::setup(&Serial, &Serial, true, true  );
  delay(3000);
  mut.unlock();
  threads.addThread(threadAction);
  threads.addThread(threadArretUrgence);
  pinMode(PIN_ARRET_URGENCE,INPUT_PULLDOWN);
  
  threads.addThread(threadCommunication);

}

void loop() {

}