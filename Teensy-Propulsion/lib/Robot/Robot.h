#ifndef ROBOT_H_
#define ROBOT_H_

#include "Asservissement.h"
#include "Communication.h"
#include "Ghost.h"
#include "Motor.h"
#include "Odometry.h"
#include "Vector.h"
#include "Tirette.h"
#include "pinSetup.h"

class Robot {
   public:
        Robot(float x_ini, float y_ini, float theta_ini);
        Robot(){};
        void init(float x_ini, float y_ini, float theta_ini);

        Ghost& getGhost();

        void updateMovement();
        void startMovement(VectorOriented nextDest, bool isOnlyRotation, bool isBackward, bool nullInitSpeed, bool nullFinalSpeed);
        bool movementDone();
        void stopMovement();
        void resumeMotor();

        void updateOdometry(float dt);

        void openClaws();
        void closeClaws();
        void raiseClaws();
        void lowerClaws();
        void startMagnet();
        void shutdownMagnet();
        void solarLeftOn();
        void solarLeftOff();
        void solarRightOn();
        void solarRightOff();

        bool testTirette();

        Communication comMega;
        Communication comESP;


        Kinetic& getCurKinetic() {return kineticCurrent;};

   private:
        Asservissement controller;
        Codeuse codeuseL, codeuseR;
        Switch switchL, switchR;
        Tirette tirette;
        Odometry odometry;
        Ghost ghost;
        Kinetic kineticCurrent, kineticNext;

        Motor motorL, motorR;
        VectorOriented vectIni;

        float translationOrder, rotationOrder;

        float startActionMillis;

};

#endif
