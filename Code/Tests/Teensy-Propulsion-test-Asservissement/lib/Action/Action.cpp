#include "Action.h"




Action::Action(uint8_t id){
    idAction=id;
}



void Action::start(){

}

void Action::finish(){

}

MoveAction::MoveAction():Action(1){

}

void MoveAction::start(){
    Action::start();

}

