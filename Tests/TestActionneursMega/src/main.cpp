#include <Arduino.h>

#include "Actuators.h"

void setup() {
  Elevator elevator = Elevator();
  elevator.move(12000, true);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
