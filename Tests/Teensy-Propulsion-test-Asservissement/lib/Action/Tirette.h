#ifndef TIRETTE_H_
#define TIRETTE_H_

#include "Arduino.h"

class Tirette{

    public :

        Tirette(uint8_t pinIN);
        bool testTirette();

    private :

        uint8_t pinIN;
        bool lancement;
        bool branche;

};


#endif