#ifndef LOGGER_H_
#define LOGGER_H_

#include <Arduino.h>
#include <Print.h>


/**
 * Classe definissant les messages renvoyes par les cartes a l'utilisateur
*/
class Logger
{
    public :

    /**
     * Permet de parametrer le Logger avec la carte et les differents ports
     * Est static car permet de ne pas avoir de constructeurs donc de pouvoir l'utiliser "sans creer un objet" et le passer an parametres de fonctions des qu'on en a besoin
     * @param infoSerial : Serial sur lequel les donnees informatives sont transmises
     * @param debugSerial : Serial sur lequel les donnes de debugage sont transmises (a utiliser pour chercher des erreurs de code)
     * @param teleplotSerial : Serial sur lequel les donnes pour Teleplot sont transmises
     * @param info : Booleen indiquant si le canal info est active
     * @param debug : Booleen indiquant si le canal debug est active
     * @param teleplot : Booleen indiquant si le canal teleplot est active
    */
    static void setup(Print* infoSerial, Print* debugSerial, Print* teleplotSerial, bool info, bool debug, bool teleplot);

    /**
     * Permet d'ecrire sur le canal info
     * @param message : String, Message a ecrire sur le canal info
    */
    static void info(const String &message);

    /**
     * Permet d'ecrire sur le canal debug
     * @param message : String, Message a ecrire sur le canal debug
    */
    static void debug(const String &message);

    /**
     * Permet d'ecrire sur le teleplot
     * @param message : String, Message a ecrire sur le canal debug
    */
    static void teleplot(const String &message);

    private :

    static Print* serialInfo; //Serial ou les donnees d'information sont transmises
    static Print* serialDebug; //Serial ou les donnees servant au debug sont transmises
    static Print* serialTeleplot; //Serial ou les donnees servant au teleplot sont transmises
    static bool infoOpened; //Canal info ouvert ?
    static bool debugOpened; //Canal debug info ?
    static bool teleplotOpened; //Canal teleplot info ?

};

#endif