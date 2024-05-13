#include "Communication.h"
#include "Logger.h"

#define BYTES_MESS sizeof(Message)

Communication::Communication(Stream* port){
    this->port=port;
    this->receivingBox= new MessageBox();
    this->sendingBox= new MessageBox();
    while (port->available()>0){
        port->read();
    }
    this->millisLastSend=millis();
}

void Communication::send(Message mess){
    sendingBox->push(mess);
}

void Communication::update(){
    if(this->waitingTX() ){
        Message toSend = sendingBox->pull();
        uint8_t out[BYTES_MESS];
        memcpy(out, &toSend, BYTES_MESS);
        for (int i=0; i<BYTES_MESS; i++){
            port->write(out[i]);
        }
    }
    if(port->available()>=BYTES_MESS){
        uint8_t in[BYTES_MESS];
        while(port->peek()!=255 && port->available()>=BYTES_MESS){
            port->read();
        }
        for (int i=0 ; i<BYTES_MESS ; i++){
            in[i]=port->read();
            delay(1);
        }
        Message mess;
        memcpy(&mess,in,sizeof(in));
        receivingBox->push(mess);
    }
}

void Communication::popOldestMessage(){
    this->receivingBox->pull();
}

Message Communication::peekOldestMessage()
{
    return receivingBox->peek();
}

bool Communication::waitingRX(){
    return !this->receivingBox->getEmpty();
}

bool Communication::waitingTX(){
    return !this->sendingBox->getEmpty();
}