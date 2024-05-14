#ifndef GHOST_H_
#define GHOST_H_

#include "Arduino.h"
#include "Math_functions.h"
#include "Vector.h"

/**
 * Classe definissant le ghost : image virtuelle du robot dont ce dernier doit suivre les mouvements
*/
class Ghost{

    public :

    /**
     * Constructeur de la classe Ghost
     * @param initialPosition : VectorOriented, position du Ghost actuel avant un mouvement
    */
    Ghost(VectorOriented initialPosition = VectorOriented());

    /**
     * @return Trajectoire finie ou non
    */
    bool getTrajectoryIsFinished();

    /**
     * @param lock : bool, le ghost est bloque (aucun mouvement) ou non
    */
    void setLock(bool lock);

    /**
     * @return Ghost bloque ou non
    */
    bool getLock();

    /**
     * Fonction de calcul de la trajectoire a effectuer (utilise lors du start d'une action)
     * @param endPosition : VectorOriented, position a atteindre
     * @param deltaCurve : float, parametre de courbure (voir sur le drive : lien sur Facebook (post de Julien Taton du 17 novembre 2020) pour voir ce que c'est)
     * @param speedRamps : float, valeur d'acceleration (et deceleration) pour la rampe de vitesse
     * @param cruisingSpeed : float, vitesse max atteinte par le robot lors de sa trajectoire
     * @param isOnlyRotation : bool, indique si ce n'est qu'une rotation
     * @param isBackward : bool, indique si on est en marche arriere
     * @return bool, false si aucune erreur true sinon
    */
    bool computeTrajectory(VectorOriented endPosition, float deltaCurve, float speedRamps, float cruisingSpeed, bool isOnlyRotation, bool isBackward); 

    /**
     * Calcule la position suivante du ghost en fonction de la trajectoire predetermine par la fonction computeTrajectory
     * @param dt : float, Delta de temps entre deux executions de l'actualisations
    */
    void actuatePosition(float dt);

    /**
     * Mise a niveau des profils de vitesse lineaire et rotationnel
     * @param curPosition : VectorOriented, position actuelle du ghost
     * @param lastPosition : VectorOriented, derniere position connue du ghost
     * @param dt : float, delta de temps entre les deux appels
    */
    void updateSpeeds(VectorOriented curPosition, VectorOriented lastPostion, float dt);

    Kinetic getControllerKinetic();

    private : 

    float MIN_MOVEMENT = 0.005; //en m, distance minimale pour permettre un deplacement
    float MIN_ROTATION = 0.1*DEG_TO_RAD; //en rad, rotation minimale permise pour un deplacement
    float DELAY_POSITION = 0.04; //en s, Delai entre la position actuelle et la position repoussee : permet une approche plus reel (laisse le temps au robot de se deplacer)
    float MAX_SPEED = 50.0; //en m/s, vitesse a ne pas depasser (est la plus par theorie que par pratique)
    float MAX_DISTANCE = 5.0; //en m, distance entre deux mouvements a ne pas depasser (est plus la par theorie que par pratique si aucune erreur)

    VectorOriented currentPosition; //Position actuelle du ghost
    VectorOriented delayedPosition; //Position differe du ghost (voir DELAY_POSITION)
    VectorOriented previousPosition; //Position precedente du ghost
    VectorOriented goaledPosition; //Position finale visee du ghost
    Polynome trajectory_X; //Courbe de Bezier en x de la trajectoire
    Polynome trajectory_Y; //Courbe de Bezier en y de la trajectoire
    Polynome speedSquare_v; //Evolution de la vitesse theorique (enfin un truc du genre)
    Trapezoidal_Function speedProfileLinear; //Profil de la vitesse lineaire
    Trapezoidal_Function speedProfileRotation; //Profil de la vitesse de rotation

    float t=0.0; //en s, temps reel du ghost
    float t_delayed = 0.0; //en s, temps repousse du ghost (voir DELAY_POSITION)
    float t_v = 0.0; // en s (compris entre 0 et 1), temps virtuel reel du ghost utilise pour les courbes de Bezier  
    float t_v_delayed = 0.0; // en s (compris entre 0 et 1), temps virtuel repousse (voir DELAY_POSITION) du ghost utilise pour les courbes de Bezier  

    float durationTrajectory = 0.0; //en s, temps theorique en laquelle la trajectoire est cense s'effectuer
    float lenghtTrajectory = 0.0; //en rad si rotation en m sinon, angle ou distance theorique de la trajectoire 
    float currentSpeedLinear =0.0; // en m/s, vitesse lineaire actuelle
    float currentSpeedRotation = 0.0; //en rad/s, vitesse de rotation actuelle

    bool locked = true; //si true : aucun mouvement permis
    bool pureRotation = false; //si true : seulement une rotation
    bool backward = false; //si true : robot avance en marche arriere
    bool trajFinished = false; //si true : la trajectoire est finie : on peut passer a l'action suivante

};

#endif