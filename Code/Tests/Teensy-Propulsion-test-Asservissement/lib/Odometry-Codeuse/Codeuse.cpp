#include "Codeuse.h"

Codeuse::Codeuse(uint8_t pinCodA, uint8_t pinCodB, int32_t tPerRound, float wDiam, bool orient){
    enc = new Encoder(pinCodA,pinCodB);
    this->ticksPerRound=tPerRound;
    this->wheelDiameter=wDiam;
    this->ticks=0;
    this->oldTicks=0;
    this->v=0.0;
    this->deltaAvance=0.0;
    this->orientation=orient;
}

void Codeuse::actuate(float dt){
    ticks=enc->read();
    deltaAvance = (ticks-oldTicks)*(PI*wheelDiameter)/ticksPerRound;
    v = deltaAvance/dt;
    oldTicks=ticks;
}

float Codeuse::getTranslationSpeedCodeuse(){
    return this->v;
}

float Codeuse::getDeltaAvance(){
    return this->deltaAvance;
}