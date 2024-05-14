#include "Tirette.h"

Tirette::Tirette(uint8_t pinIN){
    this->pinIN=pinIN;
    pinMode(pinIN, INPUT_PULLUP);
    lancement= false;
    branche = !digitalRead(pinIN);

}

bool Tirette::testTirette(){
    if(branche){
        if(digitalRead(pinIN)){
            return true;
        }
    }
    else{
        return false;
    }
}