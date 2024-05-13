#ifndef MOTOR_H_
#define MOTOR_H_

#include "Arduino.h"

/**
 * Classe permettant de parametrer les moteurs des roues et de leur donner le PWM adequat en fonction du mouvement a effectuer
*/
class Motor{

    public : 

    /**
     * Constructeur de la classe Motor 
     * @param pinPWM : uint8_t, Pin sur laquelle la puissance est donnee
     * @param pinIN1 : uint8_t, Pin avec signal booleen représentant la direction du moteur
     * @param numberBitsPWM : uint8_t, Indique le nombre de bits sur laquelle est defini le PWM du moteur (entre 8 et 16)
     * @param rotationWay : monté à l'endroit?
    */
    Motor(uint8_t pinPWM, uint8_t pinIN1, uint8_t numberBitsPWM, bool rotationWay); 
    Motor(){};
    /**
     * Fonction permettant d'actualiser la puissance moteur
    */
    void actuate();

    /**
     * Stoppe le moteur (impose un ordre prioritaire nul) :  sert pour la gestion des collisions par exemple
    */
    void stop();

    /**
     * Fonction permettant de reprendre le fonctionnement "normal" du moteur : enleve l'ordre prioritaire
    */
    void resume();

    /**
     * Fonction permettant d'update la valeur PWM (puissance) ainsi que le booleen de sens de rotation. Si l'ordre est prioritaire mets la valeur dans priorityPWMValue
     * @param PIDOrder : float [-1,1], Ordre de puissance renvoye apres application du PID, Donne un pourcentage de la puissance moteur maximale (si PIDOrder=1 => PWMValue=maxPWM)
    */
    void setPWMValue(float PIDOrder);

    /**
     * Fonction permettant de determiner la frequence ideal en fonction du nombre de bits de la valeur puissance-moteur.
     * La table est dans le lien ci-dessous dans PWMResolution (Teensy 4.1).
     * On suppose que la vitesse du processeur est de 600 MHz (voir c_cpp_properties.json F_CPU=600MHz).
     * @see https://www.pjrc.com/teensy/td_pulse.html
     * @param numberBitsPWM : uint8_t, Nombre de bits de la valeur puisance-moteur (PWMValue)
     * @return La frequence ideal en Hz
    */
    double idealFrequency(uint8_t numberBitsPWM = 12);

    private :

        uint8_t pinPWM; //Pin sur laquelle la puissance est donnee
        uint8_t pinIN1; // Les drivers des moteurs ont changés cette année, il faut deux signaux pour définir la direction


        uint16_t maxPWM; //La puissance max du moteur en fonction de son bit de resolution
        uint16_t pwmValue; //Donne la puissance au moteur 
        uint8_t numberBitsPWM; //Indique le nombre de bits sur laquelle est defini le PWM du moteur (entre 8 et 16) 
        bool priorityOrder = false; //Booleen d'ordre prioritaire : utile pour les recallages (le moteur tourne tant que les interrupteurs ne sont pas actives) et pour stopper le robot via le Lidar
        uint16_t priorityPWMValue; //Donne la puissance au moteur pendant l'ordre prioritaire
        bool rotationWay = true; //Booleen du sens de rotation du moteur
};


#endif