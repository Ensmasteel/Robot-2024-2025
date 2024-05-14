#ifndef ODOMETRY_H_
#define ODOMETRY_H_

#include "Arduino.h"
#include "Codeuse.h"
#include "Vector.h"

/**
 * Classe permettant de gérer les interrupteurs de recallage notamment
*/
class Switch{

    public:

        /**
         * Constructeur de la classe Switch 
         * @param pin : uint8_t, pin sur lequel l'interrupteur est branche
        */
        Switch(uint8_t pin);
        Switch(){};

        /**
         * Classe mettant a jour le booleen contact qui verifie si l'interrupteur est appuye ou non
        */
        void updateContact();

        /**
         * Getter du booleen 
        */
        bool getContact();

    private:
    
        uint8_t pin;    //Pin de l'interrupteur
        bool contact;   //Interrupteur actif ou non ?

};


class Odometry{

    public :

        /**
         * Constructeur de la classe Odometry : 
         * @param CodeuseL : la codeuse gauche
         * @param CodeuseR : la codeuse droite
         * @param cSpacing : float, parametre fixe d'eloignement des codeuses
         * @param switchL : interrupteur de recallage gauche
         * @param switchR : interrupteur de recallage droite
         * @param kinetic : cinetique du robot a l'instant
        */
        Odometry(Codeuse *codeuseL, Codeuse *codeuseR, float cSpacing, Switch *switchL, Switch *switchR, Kinetic *kinetic);

        Odometry(){};

        /**
         * Donne si l'interrupteur gauche est actif
         * @return booleen de l'interrupteur gauche
        */
        bool getContactSwitchL();

        /**
         * Donne si l'interrupteur droit est actif
         * @return booleen de l'interrupteur droit
        */
        bool getContactSwitchR();

        /**
         * Fonction d'update de l'odometrie en fonction du temps qui s'est deroule
         * @param dt : float, temps d'update, depend de la frequence du robot
        */
        void updateOdometry(float dt);

    private :

        Codeuse *codeuseLeft;
        Codeuse *codeuseRight;
        float codeusesSpacing;
        Switch *switchLeft;
        Switch *switchRight;
        Kinetic *kinetics;

};

#endif