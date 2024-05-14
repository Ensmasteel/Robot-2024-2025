#include "Message.h"
#include "Logger.h"

Message newMessageCoordonnees(EquipmentID s, EquipmentID d, uint16_t x, uint16_t y){
    Message out;
    out.s=s;
    out.d=d;
    out.did=Coordonnees;
    out.x=x;
    out.y=y;
    return out;
}

Message newMessageLidar(EquipmentID s, EquipmentID d, uint16_t distance, uint16_t angle){
    Message out;
    out.s=s;
    out.d=d;
    out.did=MessLidar;
    out.distance=distance;
    out.angle=angle;
    return out;
}

Message newMessageToDo(EquipmentID s, EquipmentID d, ActionID aid){
    Message out;
    out.s=s;
    out.d=d;
    out.did=Todo;
    out.aid=aid;
    return out;
}

Message newMessageEndAction(EquipmentID s, EquipmentID d, ActionID aid){
    Message out;
    out.s=s;
    out.d=d;
    out.did=EndAction;
    out.aid=aid;
    return out;
}

Message newMessageActuator(EquipmentID s, EquipmentID d, ActionID aid){
    Message out;
    out.s=s;
    out.d=d;
    out.did=MessActuator;
    out.aid=aid;
    return out;
}

MessageBox::MessageBox(){
    ifirst = 0;
    iend = 0;
}

Message MessageBox::pull(){
    if (empty){
        //Logger::infoln("Pas de message a enlever");
    }
    else{
        Message out = box[ifirst];
        ifirst=(ifirst+1)%MESSAGE_BOX_SIZE;
        empty=(ifirst==iend);
        return out;
    }
}

void MessageBox::push(Message mess){

        box[iend]=mess;
        iend=(iend+1)%MESSAGE_BOX_SIZE;
        empty=false;
    
}

Message MessageBox::peek(){
    return box[ifirst];
}

int MessageBox::size(){
    if (empty){
        return 0;
    }
    else if (iend==ifirst){
        return MESSAGE_BOX_SIZE;
    }
    else{
        return(iend-ifirst+MESSAGE_BOX_SIZE)%MESSAGE_BOX_SIZE;
    }
}

bool MessageBox::getEmpty(){
    return this->empty;
}