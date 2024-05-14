#include <Arduino.h>

#include "Ghost.h"
#include "Odometry.h"
#include "Motor.h"
#include "Asservissement.h"
#include "pinSetup.h"
#include "Codeuse.h"


#define FREQUENCY 100
#define DT 1.0/FREQUENCY

uint32_t currentMillis = 0, lastMillis = 0;
uint32_t topWarn;
  
Kinetic cinetiqueCurrent = Kinetic(0.0, 0.0, 0.0);
Kinetic cinetiqueNext;

VectorOriented goal = VectorOriented(0.5, 0.0, 0.0);


float translationOrder = 0.0;
float rotationOrder = 0.0;

Asservissement *controller;
Odometry odometry = Odometry();
Ghost ghost;
Motor motorLeft, motorRight;

Codeuse Right_Encoder = Codeuse(PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, 8192, 0.0565 , true);
Codeuse Left_Encoder = Codeuse(PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, 16384, 0.0565 , true);
  
int count = 0;

void setup() {
  Serial.begin(115200);

  MoveProfilesSetup::setup();
  controller = new Asservissement(&translationOrder, &rotationOrder, &cinetiqueCurrent, &cinetiqueNext, 100.0);


  motorLeft =  Motor(PIN_LEFT_MOTOR_PWM, PIN_LEFT_MOTOR_IN1, PIN_LEFT_MOTOR_IN2, 12); 
  motorRight = Motor(PIN_RIGHT_MOTOR_PWM, PIN_RIGHT_MOTOR_IN1, PIN_RIGHT_MOTOR_IN2, 12);
  
  Switch leftSwitch = Switch(18);
  Switch rightSwitch = Switch(19);
  
  odometry = Odometry(&Left_Encoder,&Right_Encoder,0.25,&leftSwitch,&rightSwitch,&cinetiqueCurrent);
  
  ghost = Ghost(VectorOriented(0.0, 0.0, 0.0)); //starting position

  delay(1000);
  
  ghost.computeTrajectory(goal,0.3, MoveProfilesSetup::get(standard, true)->speedRamps, MoveProfilesSetup::get(standard, true)->cruisingSpeed, false, false);
  
  ghost.setLock(false);

}

void loop() {
  
  currentMillis = millis();
  if ((currentMillis-lastMillis )/ 1e3 >+ DT){
    count++;
    odometry.updateOdometry(DT);  // This call updates cinetiqueCurrent
    ghost.actuatePosition(DT);
    cinetiqueNext = ghost.getControllerKinetic();
    controller->compute(DT);      // This call updates translationOrder and rotationOrder

    motorLeft.setPWMValue(-(translationOrder - rotationOrder));
    motorLeft.actuate();
    motorRight.setPWMValue((translationOrder + rotationOrder));
    motorRight.actuate();

    if (count >50){
      Serial.println(String(rotationOrder, 3));
      Serial.println("################");
      Serial.println(controller->close);

    cinetiqueCurrent.printDebug("Robot");
    ghost.getControllerKinetic().printDebug("Ghost");
    count = 0;
    }

    lastMillis = currentMillis;
  }
}