#ifndef Coder_H_
#define Coder_H_


#include "Arduino.h"
#include <Encoder.h>
/**
 * Classe définissant les codeuses
*/
class Codeuse{


    Encoder *enc;  
    
  
public : 

    double debug;
    double debug_ticks;

    /**
     * Constructeur de la classe Coder
     * @param pinCodA : pin A de la codeuse
     * @param pinCodB : pin B de la codeuse
     * @param tByRound : nombre fixe de ticks par tour de roue (à voir en fonction de la codeuse peut être 2048-4096-8192)
     * @param wDiam : diametre de la roue de la codeuse (en m)
    */
    Codeuse(uint8_t pinCodA, uint8_t pinCodB, int32_t ticksPerRound, float wDiam);
    Codeuse();
    float v, deltaMoved; //vitesse et avance du robot au niveau de la roue codeuse en question
    int32_t ticks;
    void actuate(float dt); // Update v et deltaMoved a partir du nombre de ticks


private :

    uint8_t pinA;
    uint8_t pinB;
    int32_t ticksPerRound;  //Nombre de ticks en un tour de roue
    float wheelDiameter;    //Diamètre de la roue sur laquelle la codeuse est
    int32_t oldTicks;       //sert à comparer combien de ticks on a effectué

};

#endif 