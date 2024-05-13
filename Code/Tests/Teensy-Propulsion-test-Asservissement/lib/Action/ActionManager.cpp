#include "ActionManager.h"

ActionManager::ActionManager(Communication * comESP, Communication * comArduino){
    robot = new Robot(0,0,0);
    MoveProfilesSetup::setup();
    gameTable.setup(couleurRobot);
    this->comESP=comESP;
    this->comArduino=comArduino;
}

bool ActionManager::getBoolActionInProgress(){
    return actionInProgress;
}

bool ActionManager::getEnnemy(){
    return ennemy;
}

uint8_t ActionManager::getIdAction(){
    return idActionInProgress;
}

void ActionManager::setBoolAction(bool act){
    this->actionInProgress=act;
}

void ActionManager::setIdAction(uint8_t id){
    this->idActionInProgress=id;
}

void ActionManager::setWaitMessage(bool wm){
    this->waitMessage=wm;
}

void ActionManager::update(float dt){
    //Serial.println(idActionInProgress);
    switch(idActionInProgress){
        case 0 : //choix strategie
            updateStrategie();
            //TODO : Fonction de strategie qui renvoie la destination - l'action a realiser, etc...
            break;
        case 1 : //recuperer les palets
            //Serial.println("bbbb");
            //updateActionPalet();
            //SpinAction();
            translateAction();
            break;
        case 2 : //deposer les palets
            updateActionDepot();
            break;
        default :
            break;
    }
}

void ActionManager::updateCommunicationESP(){
    if(comESP->waitingRX()){
        Message currentMessage = comESP->peekOldestMessage();
        DataID did = currentMessage.did;
        switch(did){

            case Coordonnees : {
                Serial.println("zzzzzzzzzz");
                VectorOriented next = VectorOriented(currentMessage.x/1000.0,currentMessage.y/1000.0);
                nextDest=next;
                setWaitMessage(false);
                break;
            }

            case MessLidar : {
                ennemy = true;
            }
        default:
            break;
    
        }
        comESP->popOldestMessage();
    }
}

void ActionManager::updateCommunicationMega(){
    if(comArduino->waitingRX()){
        Message currentMessage = comArduino->peekOldestMessage();
        DataID did = currentMessage.did;
        switch(did){

            case EndAction : {
                waitMessageArduino = false;
                break;
            }
            default:
                break;
        }
        comArduino->popOldestMessage();
    }
}

void ActionManager::evitement(){
    robot->stopMovement();
}

void ActionManager::stopMovement(){
    robot->stopMovement();
}


void ActionManager::SpinAction(){
    
    static bool done = false;

    if(actionInProgress){
        robot->updateMovement();
    }
    else{
        if(!done){
            nextDest = VectorOriented(0.225, 0.225, PI);
            robot->startMovement(nextDest, true);
            done = true;
            actionInProgress = true;
        }
    }
}

void ActionManager::translateAction(){
    
    static bool done = false;

    if(actionInProgress){
        robot->updateMovement();
    }
    else{
        if(!done){
            nextDest = VectorOriented(1, 0, 0);
            robot->startMovement(nextDest, false);
            done = true;
            actionInProgress = true;
        }
    }
}


void ActionManager::updateStrategie(){

    


    if (!stockageJaune||!stockageRose||!stockageMarron){
        Vector nextCoord = gameTable.nearestPalet(robot->kineticCurrent,!stockageJaune,!stockageMarron,!stockageRose);
        nextDest = VectorOriented(nextCoord.getX(), nextCoord.getY(),(nextCoord-robot->kineticCurrent).angle());
        idActionInProgress=1;
        //nextDest.printDebug("Next :");
    }
    else{
        nextDest = gameTable.nearestBase(robot->kineticCurrent);
        nextDest.printDebug("Base :");
        delay(2000);
        idActionInProgress=2;
    }
}

void ActionManager::updateActionPalet(){


    switch (idActionStage)
    {
        case 0: //Mouvement au palet selectionne
            if (actionInProgress){
                if (robot->endMovement()){
                    gameTable.getPaletInProgress()->onTable=false;
                    Serial.println(gameTable.getPaletInProgress()->onTable);

                    idActionStage=1;
                    actionInProgress=false;
                }
                else {
                    robot->updateMovement();
                }
            }
            else{
                robot->startMovement(nextDest, false);
                //nextDest.printDebug("Robot : ");
                actionInProgress=true;
                //Serial.println(":!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            }
        break;
    case 1: //Update de la communication actionneurs en attendant la fin de l'action
        if(actionInProgress){
            if (waitMessageArduino){
                updateCommunicationMega();
                waitMessageArduino=false;
        //Serial.println("ccccccccc");
            }
            else{
                Serial.println("Fin Action");
                actionInProgress=false;
                idActionStage=2;
            }
        } 
        else{
            Message mess;

            switch(gameTable.getCouleurPalet())
            {
                case Jaune :
                    mess = newMessageToDo(Teensy,Arduino,PaletJaune);
                    break;
                case Marron :
                    mess = newMessageToDo(Teensy,Arduino,PaletMarron);
                    break;
                case Rose :
                    mess = newMessageToDo(Teensy,Arduino,PaletRose);
                    break;
                default :
                    break;
            }
            comArduino->send(mess);
            waitMessageArduino = true;
            actionInProgress=true;
        }
        break;
    case 2: //Remise des parametres a zero et prise du palet 
        //Serial.println("JEEEEEEEEEEEEEEE");
        idActionInProgress=0;
        idActionStage=0;
        switch(gameTable.getCouleurPalet())
        {
            case Jaune :
                stockageJaune=true;
                break;
            case Marron :
                stockageMarron=true;
                break;
            case Rose :
                stockageRose=true;
                break;
            default :
                break;
            }
        break;
    default:
        break;
    }
}

void ActionManager::updateActionDepot(){
    switch(idActionStage){
        case 0: //Deplacement a la base
            if (actionInProgress){
                if (robot->endMovement()){
                    idActionStage=1;
                    actionInProgress=false;
                }
                else {
                    robot->updateMovement();
                }
            }
            else{
                robot->startMovement(nextDest, false);
                //nextDest.printDebug("Robot : ");
                actionInProgress=true;
                //Serial.println(":!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            }
        break;
        case 1: //envoi et attente du message de fin d'action de l'arduino
            if(actionInProgress){
                if (waitMessageArduino){
                    updateCommunicationMega();
                    waitMessageArduino=false;
            //Serial.println("ccccccccc");
                }
                else{
                    Serial.println("Fin Action");
                    actionInProgress=false;
                    idActionStage=2;
                }
            } 
            else{
                Message mess;
                mess = newMessageToDo(Teensy,Arduino,Depot);
                comArduino->send(mess);
                waitMessageArduino = true;
                actionInProgress=true;
            }
            break;
        case 2 : //choix si encore gateau a construire ou si fin donc choix de quel recul
            countGateauinProgress+=1;
            gameTable.getBaseInProgress()->setNbGateau(countGateauinProgress);
            float thetaBase = gameTable.getBaseInProgress()->getCoordonnees().getTheta();
            if(countGateauinProgress==3){
                nextDest=robot->kineticCurrent-VectorOriented(0.2*cos(thetaBase),0.2*sin(thetaBase),0.0);
                idActionStage=4;
            }
            else{
                nextDest=robot->kineticCurrent-VectorOriented(0.0,0.15,0.0);
                idActionStage=3;
            }
            break;
        case 3: //Recul entrainant un nouveau gateau
            if (actionInProgress){
                if (robot->endMovement()){
                    idActionStage=1;
                    actionInProgress=false;
                }
                else {
                    robot->updateMovement();
                }
            }
            else{
                robot->startMovementBackwardDepot(nextDest);
                //nextDest.printDebug("Robot : ");
                actionInProgress=true;
                //Serial.println(":!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            }
            break;
        case 4 : //Recul et reinitialisation des parametres pour la suite
            
            if (actionInProgress){
                if (robot->endMovement()){
                    idActionStage=0;
                    idActionInProgress=0;
                    countGateauinProgress=0;
                    actionInProgress=false;
                    stockageJaune=false;
                    stockageMarron=false;
                    stockageRose=false;
                }
                else {
                    robot->updateMovement();
                }
            }
            else{
                robot->startMovementBackwardDepot(nextDest);
                //nextDest.printDebug("Robot : ");
                actionInProgress=true;
                //Serial.println(":!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            }
            break;
        default :
            break;
    }
}