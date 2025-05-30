#include "Robot.h"

#define SYM 0.001

Robot::Robot(float x_ini, float y_ini, float theta_ini) {
    this->vectIni = VectorOriented(x_ini, y_ini, theta_ini);
    this->kineticCurrent = Kinetic(x_ini, y_ini, theta_ini, 0, 0);
    this->kineticNext = Kinetic(x_ini, y_ini, theta_ini, 0, 0);

    this->ghost = Ghost(vectIni);
    this->motorL = Motor(PIN_LEFT_MOTOR_PWM, PIN_LEFT_MOTOR_IN1, 13, false);
    this->motorR = Motor(PIN_RIGHT_MOTOR_PWM, PIN_RIGHT_MOTOR_IN1, 13, false);

    this->codeuseL = Codeuse(PIN_CODEUSE_GAUCHE_A, PIN_CODEUSE_GAUCHE_B, 8192 * (1 - SYM), 0.05827, false);
    this->codeuseR = Codeuse(PIN_CODEUSE_DROITE_A, PIN_CODEUSE_DROITE_B, 16384 * (1 + SYM), 0.05761, true);

    this->switchL = Switch(PIN_SWITCH_L);
    this->switchR = Switch(PIN_SWITCH_R);

    this->odometry = Odometry(&codeuseL, &codeuseR, 0.375, &switchL, &switchR, &kineticCurrent);

    this->controller = Asservissement(&translationOrder, &rotationOrder, &kineticCurrent, &kineticNext, 100.0);

    this->comMega = Communication(&Serial1);
    this->comESP = Communication(&Serial2);

    this->tirette = Tirette(PIN_TIRETTE);

}

void Robot::init(float x_ini, float y_ini, float theta_ini) {
    this->vectIni = VectorOriented(x_ini, y_ini, theta_ini);
    this->kineticCurrent = Kinetic(x_ini, y_ini, theta_ini, 0, 0);
    this->kineticNext = Kinetic(x_ini, y_ini, theta_ini, 0, 0);
}

Ghost& Robot::getGhost() {
    return ghost;
}

void Robot::updateMovement() {
    ghost.actuatePosition(1.0 / 100.0);
    kineticNext = ghost.getControllerKinetic();
    controller.compute(1.0 / 100.0);  // This call updates translationOrder and rotationOrder
    motorL.setPWMValue(-(translationOrder - rotationOrder));
    motorL.actuate();
    motorR.setPWMValue((translationOrder + rotationOrder));
    motorR.actuate();
}

void Robot::startMovement(VectorOriented nextDest, bool isOnlyRotation, bool isBackward, bool nullInitSpeed, bool nullFinalSpeed) {
    ghost.computeTrajectory(nextDest, 0.3, MoveProfilesSetup::get(standard, !isOnlyRotation)->speedRamps, MoveProfilesSetup::get(standard, !isOnlyRotation)->cruisingSpeed, isOnlyRotation, isBackward, nullInitSpeed, nullFinalSpeed);
    ghost.setLock(false);
    startActionMillis = millis();
}

bool Robot::movementDone() {
    bool out = ghost.getTrajectoryIsFinished() && controller.close;
    if (out) {
        ghost.goToRobot(kineticCurrent);

        // Mettre les moteurs à 0 en fin de mouvement
        motorL.setPWMValue(0.0f);   //TODO enchaine mouvement v non nulle
        motorR.setPWMValue(0.0f);
        motorL.actuate();
        motorR.actuate();
    } else if ((millis() - startActionMillis) > 10000) {
        Logger::info("Mouvement failed et arrete");
        out = true;
        ghost.goToRobot(kineticCurrent);
    } else {
    }
    return out;
}

void Robot::resumeMotor() {
    motorL.resume();
    motorR.resume();
}

void Robot::stopMovement() {
    motorL.stop();
    motorR.stop();

    motorL.actuate();
    motorR.actuate();
}

void Robot::updateOdometry(float dt) {
    odometry.updateOdometry(dt);
}


void Robot::openFrontRato() {
    comMega.send(newMessageToDo(Teensy, Arduino, OpenFrontRato));
}

void Robot::closeFrontRato() {
    comMega.send(newMessageToDo(Teensy, Arduino, CloseFrontRato));
}

void Robot::openBackRato() {
    comMega.send(newMessageToDo(Teensy, Arduino, OpenBackRato));
}

void Robot::closeBackRato() {
    comMega.send(newMessageToDo(Teensy, Arduino, CloseBackRato));
}

void Robot::raiseFrontMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, RaiseFrontMagnets));
}

void Robot::lowerFrontMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, LowerFrontMagnets));
}

void Robot::raiseBackMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, RaiseBackMagnets));
}

void Robot::lowerBackMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, LowerBackMagnets));
}

void Robot::startFrontLeftMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StartFrontLeftMagnets));
}

void Robot::stopFrontLeftMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StopFrontLeftMagnets));
}

void Robot::startFrontRightMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StartFrontRightMagnets));
}

void Robot::stopFrontRightMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StopFrontRightMagnets));
}

void Robot::startBackLeftMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StartBackLeftMagnets));
}

void Robot::stopBackLeftMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StopBackLeftMagnets));
}

void Robot::startBackRightMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StartBackRightMagnets));
}

void Robot::stopBackRightMagnets() {
    comMega.send(newMessageToDo(Teensy, Arduino, StopBackRightMagnets));
}

bool Robot::testTirette() {
    return tirette.testTirette();
}
