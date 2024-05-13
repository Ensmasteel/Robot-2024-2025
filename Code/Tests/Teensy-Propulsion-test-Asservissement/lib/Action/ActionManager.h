#ifndef ACTION_MANAGER_H_
#define ACTION_MANAGER_H_

#include "Arduino.h"
#include "Robot.h"
#include "Action.h"
#include "GameElement.h"


class ActionManager{

    public :
        
        ActionManager(Communication * comESP, Communication * comArduino);
        bool getBoolActionInProgress();
        uint8_t getIdAction();
        bool getEnnemy();
        void setIdAction(uint8_t id);
        void setBoolAction(bool act);
        void setWaitMessage(bool wm);
        void update(float dt);
        void updateCommunicationESP();
        void updateCommunicationMega();
        void updateActionPalet();
        void updateActionDepot();
        void updateStrategie();
        
        //for asservissement testing
        void SpinAction();
        void translateAction();

        void evitement();
        void stopMovement();
        Robot * robot;
        VectorOriented nextDest;


    private :

        bool actionInProgress = false;
        bool waitMessageArduino = false;
        bool waitMessage = false;
        bool messageSend = false;
        bool ennemy = false;
        
        bool stockageMarron = false;
        bool stockageRose = false;
        bool stockageJaune = false;

        uint8_t idActionInProgress = 0;
        Communication * comESP;
        Communication * comArduino;

        GameElement gameTable;

        CouleurBase couleurRobot = Bleu;

        uint8_t idActionStage = 0;

        uint8_t countGateauinProgress = 0;       

};


#endif