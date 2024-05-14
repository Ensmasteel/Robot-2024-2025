#ifndef ROBOT_H_
#define ROBOT_H_

#include "Vector.h"
#include "Ghost.h"
#include "Asservissement.h"
#include "Motor.h"
#include "pinSetup.h"
#include "Odometry.h"

class Robot{

    public :

        Robot(float x_ini, float y_ini, float theta_ini);
        Robot(){};
        void updateMovement();
        void startMovement(VectorOriented nextDest, bool isPureRotation);
        void startMovementBackwardDepot(VectorOriented nextDest);
        bool endMovement();
        void stopMovement();

        Kinetic kineticCurrent, kineticNext;
        Ghost ghost;

    private :

        //Kinetic kineticCurrent, kineticNext;
        
        Asservissement controller;
        Odometry odometry;
        Codeuse codeuseL, codeuseR;
        Switch switchL,switchR;


        Motor motorL,motorR;
        VectorOriented vectIni;

        float translationOrder, rotationOrder;
        

        float startActionMillis;


};

#endif