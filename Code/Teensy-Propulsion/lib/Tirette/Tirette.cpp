#include "Tirette.h"

Tirette::Tirette(uint8_t pinIN){
    this->pinIN=pinIN;
    pinMode(pinIN, INPUT_PULLDOWN);
    lancement= false;
    branche = digitalRead(pinIN)==HIGH;

}

bool Tirette::testTirette(){
    if(branche){
        if(digitalRead(pinIN)==LOW){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}
