#include "Codeuse.h"



void Codeuse::actuate(float dt)
{
    ticks = enc->read();                                                    //On recupère les ticks de l'objet Encoder (automatiquement mis a jour par interruptions cf cours ISE)
    deltaMoved = (ticks - oldTicks) * (PI * wheelDiameter) / ticksPerRound; //Simple géométrie
    v = deltaMoved / dt;
    debug += deltaMoved;
    debug_ticks = ticks;
    oldTicks = ticks;
}

Codeuse::Codeuse(uint8_t pinCodA, uint8_t pinCodB, int32_t ticksPerRound, float wDiam)
{
    enc = new Encoder(pinCodA, pinCodB); //Objet Encoder de la librairie Encoder.
    this->wheelDiameter = wDiam;
    this->ticksPerRound = ticksPerRound; //Nombe de ticks par tour
    ticks = 0;
    oldTicks = 0;
    v = 0.0;
    deltaMoved = 0;
    debug = 0;
}

Codeuse::Codeuse(){}