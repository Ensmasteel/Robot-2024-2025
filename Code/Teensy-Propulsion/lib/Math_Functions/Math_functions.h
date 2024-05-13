#ifndef MATH_FUNCTION_H
#define MATH_FUNCTION_H

#include "Arduino.h"

/**
 * Classe permettant la manipulation de polynomes (Utilisation pour l'interpolation de trajectoires : Courbe de Bezier)
 */
class Polynome {
   public:
    /**
     * Constructeur d'un polynome d'ordre 3 : Utilisation dans les courbes de Bezier
     * @param a0 : float, coefficient ordre 0 (dans les courbes le terme en (1-t)^3)
     * @param a1 : float, coefficient ordre 1 (dans les courbes le terme en t*(1-t)^2)
     * @param a2 : float, coefficient ordre 2 (dans les courbes le terme en t^2*(1-t))
     * @param a3 : float, coefficient ordre 3 (dans les courbes le terme en t^3)
     */
    Polynome(float a0 = 0.0, float a1 = 0.0, float a2 = 0.0, float a3 = 0.0, float a4 = 0.0, float a5 = 0.0, float a6 = 0.0);

    /**
     * Set all coeficients back to 0
     */
    void initPolynome();

    /**
     * @brief Returns the coefficient of the polynomial at the given degree
     *
     * @param degreCoeff the degree of the coefficient to be retrieved
     * @return the coefficient at the given degree if degreCoeff is less than or equal to 6, 0.0 otherwise
     */
    float getCoeff(int degreCoeff);

    /**
     * @brief Sets the coefficient of the polynomial at the given degree
     *
     * @param degreCoeff the degree of the coefficient to be set
     * @param value the value of the coefficient to be set
     */
    void setCoeff(int degreCoeff, float value);

    /**
     * @param x : float
     * @return Valeur du polynome en x
     */
    float f(float x);

    /**
     * @param x : float
     * @return Valeur du polynome derive en x
     */
    float df(float x);

    /**
     * @param a0 : float, nouveau coefficient ordre 0 (dans les courbes le terme en (1-t)^3)
     * @param a1 : float, nouveau coefficient ordre 1 (dans les courbes le terme en t*(1-t)^2)
     * @param a2 : float, nouveau coefficient ordre 2 (dans les courbes le terme en t^2*(1-t))
     * @param a3 : float, nouveau coefficient ordre 3 (dans les courbes le terme en t^3)
     */
    void setPolynome(float a0 = 0.0, float a1 = 0.0, float a2 = 0.0, float a3 = 0.0, float a4 = 0.0, float a5 = 0.0, float a6 = 0.0);

    /**
     * @param P : Polynome a deriver
     * @return Le polynome P derive
     */
    Polynome derivatePolynome();

    /**
     * @param P : Polynome a mettre au carre
     * @return Le polynome P au carre
     */
    /*Polynome Square(Polynome P);*/

    /*
    Surcharge de l'opérateur * pour les polynomes
    */
    friend Polynome operator*(const Polynome &lhs, const Polynome &rhs);  // a friend is used here to be ablle to access private variables of the function while being able to declare outside of the class Polynome
    friend Polynome operator+(const Polynome &lhs, const Polynome &rhs);
    friend Polynome operator-(const Polynome &lhs, const Polynome &rhs);
    friend Polynome operator+=(Polynome &lhs, const Polynome &rhs);
    friend bool operator==(const Polynome &lhs, const Polynome &rhs);

   private:
    float K[7];
};

Polynome derivativePtr(Polynome *P);

/**
 * Classe permettant de creer une rampe de vitesse (fonction en trapeze : acceleration - vitesse de croisiere - deceleration)
 */
class Trapezoidal_Function {
   public:
    /**
     * Constructeur d'une fonction trapezoidal
     * @param upRamp : float (acceleretaion m.s^-2), Rampe d'entree de la fonction (coefficient a de la fonction affine)
     * @param downRamp : float (acceleretaion m.s^-2), Rampe de sortie de la fonction (coefficient -a de la fonction affine) : souvent identique a upRamp
     * @param maxSpeed : float, Vitesse de croisiere atteinte : constante entre les deux fonctions affines
     * @param realDistance : float, Distance reel a parcourir pendant le mouvement
     */
    Trapezoidal_Function(float upRamp = 1.0, float downRamp = 1.0, float maxSpeed = 0.0, float realDistance = 0.0);

    /**
     * @param upRamp : float, Nouvelle rampe d'entree de la fonction (coefficient a de la fonction affine)
     * @param downRamp : float, Nouvelle rampe de sortie de la fonction (coefficient -a de la fonction affine) : souvent identique a upRamp
     * @param maxSpeed : float, Nouvelle vitesse de croisiere atteinte : constante entre les deux fonctions affines
     * @param realDistance : float, Nouvelle distance reel a parcourir pendant le mouvement
     */
    void setTrapezoidalFunction(float upRamp = 1.0, float downRamp = 1.0, float maxSpeed = 0.0, float realDistance = 0.0);

    /**
     * Remise a zero de la fonction trapezoidale
     */
    void setZeroTrapezoidalFunction();

    /**
     * Calcul du temps reel mis par le robot pour aller au point en fonction de la fonction
     */
    void computeDuration();

    /**
     * @return duration : float, temps reel mis pour parcourir la distance
     */
    float getDuration();

    /**
     * @param t : float
     * @return Valeur de la vitesse au temps t
     */
    float f(float t);

   private:
    float duration;       // Temps reel mis pour effectuer le mouvement selon la fonction trapezoidale
    float tAcceleration;  // Temps mis pour effectuer la rampe d'acceleration et atteindre la vitesse maximale
    bool boolTriangular;  // Booleen indiquant si la fonction est triangulaire
    float _upRamp, _downRamp, _maxSpeed, _realDistance;
};

#endif
