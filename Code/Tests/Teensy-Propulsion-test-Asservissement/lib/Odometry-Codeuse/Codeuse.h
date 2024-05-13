#ifndef Coder_H_
#define Coder_H_


#include "Arduino.h"
#include "Encoder.h"


/**
 * Classe définissant les codeuses
*/
class Codeuse{

    public : 

    /**
     * Constructeur de la classe Codeuse
     * @param pinCodA : pin A de la codeuse
     * @param pinCodB : pin B de la codeuse
     * @param tPerRound : nombre fixe de ticks par tour de roue (à voir en fonction de la codeuse peut être 2048-4096-8192)
     * @param wDiam : diametre de la roue de la codeuse (en m)
     * @param orient : booleen, orientation de la codeuse
    */
    Codeuse(uint8_t pinCodA, uint8_t pinCodB, int32_t tPerRound, float wDiam, bool orient);
    Codeuse(){};

    /**
     * Fonction d'actualisation des valeurs de vitesse du robot en fonction du nombre de ticks depuis la derniere actualisation)
     * @param dt : float, temps depuis la dernière actualisation (depend de la frequence generale du robot)
    */
    void actuate(float dt);

    /**
     * Obtient la vitesse du robot au niveau de la codeuse
     * @return Vitesse du robot d'après la codeuse
    */
    float getTranslationSpeedCodeuse();

    /**
     * Obtient l'avancement de la codeuse depuis la derniere actualisation
     * @return Avancement du robot d'après le codeuse
    */
    float getDeltaAvance();

    private :

    Encoder *enc; //Encoder de la codeuse permet les interruptions (je crois)
    int32_t ticksPerRound; //Nombre de ticks en un tour de roue
    float wheelDiameter; //Diamètre de la roue sur laquelle la codeuse est
    int32_t oldTicks = 0; //sert à comparer combien de ticks on a effectué
    int32_t ticks =0 ;
    float v = 0.0;
    float deltaAvance = 0.0;
    bool orientation = true; //true si codeuse montee a l'endroit, false sinon


};

#endif