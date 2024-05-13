#ifndef ASSERVISSEMENT_H_
#define ASSERVISSEMENT_H_

#include "Arduino.h"
#include "Vector.h"

/**
 * Classe definissant un filtre passe bas utile dans le calcul du PID (on va filtrer le terme derive)
 */
class Filtre {
   public:
    /**
     * Constructeur de la classe Filtre - Est un Filtre Passe-Haut
     * @param initValue : float, Valeur initiale d'ou on filtre les valeurs
     * @param frequency : a quelle frequence le filtre est execute sert a determiner la pulsation !
     */
    Filtre(float initValue, float frequency);

    /**
     * Fonction appliquant une fois le filtre passe-haut
     * @param newValue : float, Nouvelle valeur a appliquer et a mettre en relief avec l'ancienne
     * @param dt : Temps depuis le dernier appel de la fonction in
     */
    void in(float newValue, float dt);

    /**
     * Fonction renvoyant la valeur filtree demande a ce moment
     * @return la valeur filtree
     */
    float out();

   private:
    float wc;     // Pulsation propre associee a la frequence
    float value;  // Valeur differentiel actuelle apres la derniere application du PID
};

/**
 * Classe enumeree donnant les differentes facon de se deplacer
 */
enum MoveProfileName {
    off,            // A l'arret.
    brake,          // Freinage rapide
    accurate,       // Excellente precision mais plus lent
    standard,       // Compromis entre precision et vitesse
    fast,           // Rapide mais peu precis
    recalibrate,    // Fonction de recalibrage sur les bordures
    __NBPROFILES__  // Nombre de profils existant (ici 6*2=12)
    // Ne pas rajouter de profils apres __NBPROFILES__
};

/**
 * Classe MoveProfile : donne un profil de vitesse et de precision en fonction du type de mouvement que l'on veut faire
 */
class MoveProfile {
   public:
    float KP;             // PID facteur proportionnel
    float KI;             // PID facteur integrateur
    float KD;             // PID facteur derivateur
    float epsilon;        // Max erreur sur la position (ou rotation)
    float dEpsilon;       // Max erreur sur la vitesse
    float maxError;       // Erreur max avant de considerer le robot trop loin
    float speedRamps;     // Acceleration du robot (et deceleration)
    float cruisingSpeed;  // Vitesse max de croisiere du robot

    /**
     * @param KP : float, PID facteur proportionnel
     * @param KI : float, PID facteur integrateur
     * @param KD : float, PID facteur derivateur
     * @param epsilon : float, Max erreur sur la position (ou rotation)
     * @param depsilon : float, Max erreur sur la vitesse
     * @param maxError : float, Erreur max avant de considerer le robot trop loin
     * @param speedRamps : float, Acceleration (deceleration)
     * @param cruisingSpeed : float, Vitesse constante de croisiere demandee
     */
    void set(float KP, float KI, float KD, float epsilon, float dEpsilon, float maxErr, float speedRamps, float cruisingSpeed);
};

/**
 * Classe MoveProfilesSetup : Classe de parametrages des differents MoveProfile
 */
class MoveProfilesSetup {
   public:
    static MoveProfile profiles[2 * __NBPROFILES__];

    /**
     * Fonction de parametrage des MoveProfile (reglages de tous les parametres PID ici)
     * Doit être appelé avant d'utiliser des moves profils
     */
    static void setup();

    /**
     * Fonction creant un MoveProfile dans le tableau de MoveProfiles (pas tres clair mais je me comprend)
     * @param name : MoveProfileName, Nom du profil
     * @param translation : bool, true si translation - false si rotation
     * @return Un MoveProfile
     */
    static MoveProfile *get(MoveProfileName name, bool translation);
};

/**
 * Classe permettant de calculer un PID en fonction des parametres du profil de vitesse
 * En tout on en aura besoin de deux differents, celui de translation et celui de rotation
 * Ainsi on aura notre asservissement globale
 * Pour plus d'infos allez voir le principe de l'asservissement polaire (utilisé ici)
 */
class PID {
   public:
    /**
     * Constructeur de la classe PID
     * @param transla : booleen indiquant si c'est le PID de translation ou de rotation
     * @param frequency : permet d'appliquer un filtre sur le terme de derivation
     */
    PID(bool transla, float frequency);

    /**
     * Fonction permettant de mettre le profil a etudier
     * @param profile : MoveProfile*, profil a etudier
     */
    void setCurrentProfile(MoveProfileName profile);

    /**
     * Fonction de reset du PID
     */
    void reset();

    /**
     * Fonction permettant de calculer l'ordre de regulation
     * @param x : float, Position ou angle actuel (on ignore l'un ou l'autre en fonction du PID)
     * @param dx : float, Vitesse ou vitesse de rotation actuelle (on ignore l'un ou l'autre en fonction du PID)
     * @param xTarget : float, Cible de position ou d'angle
     * @param dxTarget : float, Cible de vitesse
     * @param dt : Temps depuis le dernier calcul (sans doute a reverifier)
     * @return la valeur d'ordre de regulation de ce PID
     */
    float compute(float x, float dx, float xTarget, float dxTarget, float dt);

    bool getClose();

    bool getTooFar();

    /**
     * Returns current profil in use
     */
    MoveProfile *getCurrentProfile();

   private:
    bool translation;             // Booleen indiquant si on agit sur le PID de rotation ou de translation (true = translation)
    MoveProfile *currentProfile;  // Profil utilise par le mouvement actuel a reguler
    Filtre *dxF;                  // Valeur d'erreur du terme derivateur (en vitesse) filtree
    bool close;                   // Booleen indiquant si on est suffisament proche de la cible (Agit avec epsilon et depsilon)
    bool tooFar;                  // Booleen indiquant si le robot ne s'est pas trop eloigne (Agit avec maxError)
    float iTerm;                  // Terme d'integration
};

class Asservissement {
   public:
    /**
     * @brief Construct a new Asservissement object
     *
     * @param translaOrder Pointer to translation order (setpoint) from user
     * @param rotaOrder Pointer to rotation order (setpoint) from user
     * @param kRobot Pointer to Kinetic object representing the robot's current state
     * @param kGhost Pointer to Kinetic object representing the desired state of the robot
     * @param frequency Filter cutoff frequency
     */
    Asservissement(float *translaOrder, float *rotaOrder, Kinetic *kRobot, Kinetic *kGhost, const float frequency);
    Asservissement(){};

    /**
     * @brief Resets the PID controllers used for control loop (asservissement)
     */
    void reset();

    /**
     * @brief Computes the control signals for the robotic system based on the PID controllers and desired setpoints
     * @param dt Time elapsed since the last control signal was computed
     */
    void compute(float dt);

    bool close;

   private:
    PID *pidTranslation;      // PID de la translation du robot
    PID *pidRotation;         // PID de la rotation du robot
    Kinetic *kineticGhost;    // cinetique du ghost a essayer de rejoindre
    Kinetic *kineticRobot;    // cinetique du robot a faire rejoindre celle du ghost
    float *translationOrder;  // Consigne du pid de translation
    float *rotationOrder;     // Consigne du pîd de rotation
    bool tooFar;
    bool needForward;
};

#endif
