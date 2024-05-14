#ifndef Vector_H_
#define Vector_H_

#include <Logger.h>
/**
 * Normalisation d'un angle.
 * @param angle : float, angle en radian.
 * @return : float, angle en radian entre -PI et PI.
 */
float normalizeAngle(float angle);

/**
 * Classe definissant un vecteur-coordonnee de base (x,y)
*/
class Vector
{
    public : 

    /**
     * Constructeur d'un vecteur-coordonnee
     * @param x : float, x-coordonnee
     * @param y : float, y-coordonnee
    */
   Vector(float x = 0.0 , float y = 0.0);

   //Surcharges des operateurs +, +=, -, *, ==
   Vector operator+(const Vector &other);
   Vector operator-(const Vector &other);
   Vector operator*(const float scalaire);
   void operator+=(const Vector &other);
   bool operator==(const Vector &other);

   /**
    * @return Norme du vecteur-coordonnee
   */
   float norm();

   /**
    * @return Angle par rapport au vecteur normalise du vecteur-coordonnee
   */
   float angle();

   /**
    * @return Distance entre deux vecteurs-coordonnees
    * @param other : Vector, le deuxieme vecteur
   */
   float distanceWith(Vector &other);

   /**
    * @return Coordonnee x
   */
   float getX();  

   /**
    * @return Coordonnee y
   */
   float getY();

      /**
    * @param x : Nouvelle coordonnee x
   */
   void setX(float x);  

   /**
    * @param y : Nouvelle coordonnee y
   */
   void setY(float y);

   void printDebug(const String& prefix);

   private :

   float _x,_y;  

};

/**
 *Classe etendu de la classe Vector : Rajout d'un angle d'orientation du robot
*/

class VectorOriented : public Vector
{
    public :

    /**
     * Constructeur d'un vecteur oriente
     * @param x : float, x-coordonnee
     * @param y : float, y-coordonnee
     * @param th : float, orientation du robot
    */
    VectorOriented(float x=0.0, float y=0.0, float th=0.0);

    bool operator==(VectorOriented const &other);
    VectorOriented operator-(VectorOriented const &other);

    /**
     * @return Angle d'orientation theta
    */
    float getTheta();

    /**
     * @param th : Nouvel angle theta
    */
    void setTheta(float th);

    /**
     * Normalisation de l'angle theta (mise en radian entre 0 et 2PI)
     * @return Angle theta normalise
    */
    void normalizeTheta();

    void printDebug(const String& prefix);

    private :

    float theta;  

};

/**
 * Classe definissant la cinetique du robot c'est a dire sa situation en positionnement et en vitesse
*/
class Kinetic : public VectorOriented{

    public : 

    /**
     * Constructeur de la classe Kinetic 
     * @param x : float, Position en x
     * @param y : float, Position en y
     * @param th : float : Angle d'orientation
     * @param v : float, Vitesse de translation du robot
     * @param w : float, Vitesse de rotation du robot 
    */
    Kinetic(float x=0.0, float y=0.0, float th=0.0, float v=0.0, float w=0.0);

    bool operator==(Kinetic const &other);

    /**
     * @return Vitesse de translation du robot
    */
    float getTranslationSpeed();

    /**
     * @param ts : Nouvelle vitesse de translation
    */
    void setTranslationSpeed(float ts);

        /**
     * @return Vitesse de rotation du robot
    */
    float getRotationSpeed();

    /**
     * @param rs : Nouvelle vitesse de rotation
    */
    void setRotationSpeed(float rs);

    /**
     * Prints in Serial consol kinetic parameters :
     * x, y, Theta, v, w
     * @param prefix : string being printed in serial monitor
    */
    void printDebug(const String& prefix);

    private :

    float v; //Vitesse de translation du robot
    float w; //Vitesse de rotation du robot


    
};

#endif