#include "Robot.h"


Robot::Robot(float x_ini, float y_ini, float theta_ini){

    this->vectIni=VectorOriented(x_ini,y_ini,theta_ini);
    //vectIni=VectorOriented(0.725,1.125,PI/4);
    this->kineticCurrent=Kinetic(x_ini,y_ini,theta_ini,0,0);
    this->kineticNext=Kinetic(x_ini,y_ini,theta_ini,0,0);

    this->ghost = Ghost(vectIni);
    this->motorL =  Motor(PIN_LEFT_MOTOR_PWM, PIN_LEFT_MOTOR_IN1, PIN_LEFT_MOTOR_IN2, 12); 
    this->motorR = Motor(PIN_RIGHT_MOTOR_PWM, PIN_RIGHT_MOTOR_IN1, PIN_RIGHT_MOTOR_IN2, 12);

    this->codeuseR = Codeuse(PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, 8192, 0.0566 , true);
    this->codeuseL = Codeuse(PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, 16384, 0.0566 , true);
  
    this->switchL = Switch(18);
    this->switchR = Switch(19);

    this->odometry = Odometry(&codeuseL,&codeuseR,0.255,&switchL,&switchR,&kineticCurrent);

    this->controller = Asservissement(&translationOrder, &rotationOrder, &kineticCurrent, &kineticNext, 100.0);


}

void Robot::updateMovement(){

    odometry.updateOdometry(0.01);  // This call updates cinetiqueCurrent
    ghost.actuatePosition(0.01);
    kineticNext = ghost.getControllerKinetic();
    //kineticNext.printDebug("Ghost");
    controller.compute(1/100.0);      // This call updates translationOrder and rotationOrder
    Serial.println(translationOrder);
    
    Serial.println(rotationOrder); 
    //kineticCurrent.printDebug("Robot");
    motorL.setPWMValue(-(translationOrder - rotationOrder));
    motorL.actuate();
    motorR.setPWMValue((translationOrder + rotationOrder));
    motorR.actuate();
}

void Robot::startMovement(VectorOriented nextDest, bool isPureRotation){
    Serial.println("compute");

    if(!isPureRotation){
        ghost.computeTrajectory(nextDest,0.3, MoveProfilesSetup::get(standard, true)->speedRamps, MoveProfilesSetup::get(standard, true)->cruisingSpeed, false, false);
    }else{
        ghost.computeTrajectory(nextDest, 0.3, MoveProfilesSetup::get(standard, false)->speedRamps, MoveProfilesSetup::get(standard, false)->cruisingSpeed, true, false);
    }
    
    
    ghost.setLock(false);


    startActionMillis=millis();
}

void Robot::startMovementBackwardDepot(VectorOriented nextDest){
    Serial.println("compute");
    ghost.computeTrajectory(nextDest,0.3, MoveProfilesSetup::get(standard, true)->speedRamps, MoveProfilesSetup::get(standard, true)->cruisingSpeed, false, true);
    ghost.setLock(false);
    startActionMillis=millis();
}

bool Robot::endMovement(){
    bool out = ghost.getTrajectoryIsFinished() && controller.close;
    if (out){
        //Serial.println("Mouvement termine");
    }
    else if (!out && (millis()-startActionMillis)>10000){
        Serial.println("Mouvement failed et arrete");
        out=true;
    }
    else{}
    return out;
}

void Robot::stopMovement(){
    motorL.roueLibre();
    motorR.roueLibre();


    motorL.actuate();
    motorR.actuate();
}