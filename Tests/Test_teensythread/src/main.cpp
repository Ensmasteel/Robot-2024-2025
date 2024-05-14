#include <Arduino.h>
#include "TeensyThreads.h"

bool lance;

Threads::Mutex mut;


void thread1(){
  while(1){
      while(!mut.getState()){
        threads.delay(1000);
      }
    threads.yield();
  }
}

void thread2(){
  while(1){
      while(!mut.getState()){
        Serial.println("thread 2");
        threads.delay(1500);
      }
    threads.yield();
  }
}

void threaddem(){
    while(1){
      delay(1000);
      while(mut.getState()){
        Serial.println("lancement");
        threads.delay(2000);
        mut.unlock();
        
      }
      threads.yield();
    }
}

void setup() {
  Serial.begin(115200);
  mut.lock();
  delay(2000);
  threads.addThread(threaddem);
  threads.addThread(thread1);
  threads.addThread(thread2);
}

void loop() {
  ;// put your main code here, to run repeatedly:
}