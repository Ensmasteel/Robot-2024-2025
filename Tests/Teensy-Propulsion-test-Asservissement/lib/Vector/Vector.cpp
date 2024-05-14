#include "Vector.h"
#include "Arduino.h"

float normalizeAngle(float angle)
{
    float out;
    out = angle - (2 * PI) * ((int)(angle / (2 * PI)));
    if (out > PI)
        return (out - 2 * PI);
    else if (out <= -PI)
        return (out + 2 * PI);

    return out;
}

//----------Vector Class----------//

Vector::Vector(float x, float y){
    _x=x;
    _y=y;
};

Vector Vector::operator+(const Vector &other){
    return Vector(_x + other._x, _y + other._y);
}

Vector Vector::operator-(const Vector &other){
    return Vector(_x - other._x, _y - other._y);
}

Vector Vector::operator*(float scalaire){
    return Vector(_x * scalaire, _y * scalaire);
}

void Vector::operator+=(const Vector &other){
    _x+=other._x;
    _y+=other._y;
}

bool Vector::operator==(Vector const &other){
    return (abs(_x - other._x) <= 1e-6) && (abs(_y - other._y) <= 1e-6);
}

float Vector::norm(){
    return sqrt(_x * _x + _y * _y);
}

float Vector::angle(){
    return atan2(_y,_x);
}

float Vector::distanceWith(Vector &other){
    return sqrt( pow(_x - other._x , 2) + pow(_y - other._y , 2) );
}

float Vector::getX(){
    return _x;
}

float Vector::getY(){
    return _y;
}

void Vector::setX(float x){
    _x=x;
}

void Vector::setY(float y){
    _y=y;
}

void Vector::printDebug(const String& prefix)
{
    Serial.println(">" + prefix + " x :" + String(getX(),3));
    Serial.println(">" + prefix + " y :" + String(getY(), 3));
    Serial.println(">" + prefix + " xy :" + String(getX(),3) + ":" + String(getY(),3) + "|xy");
}

//----------End Vector Class----------//




//----------Vector Oriented Class----------//

VectorOriented::VectorOriented(float x, float y,float th):Vector(x,y){
    theta=th;
}

bool VectorOriented::operator==(VectorOriented const &other){
    return Vector::operator==(other) && abs(theta - other.theta) <=1e-6;
}

VectorOriented VectorOriented::operator-(VectorOriented const &other){
    Vector vect = Vector::operator-(other);
    return VectorOriented(vect.getX(),vect.getY(), theta - other.theta); 
}

float VectorOriented::getTheta(){
    return theta;
}

void VectorOriented::setTheta(float th){
    theta=th;
}

void VectorOriented::normalizeTheta()
{
    theta = normalizeAngle(theta);
}

void VectorOriented::printDebug(const String& prefix)
{
    Serial.println(">" + prefix+" x :" + String(getX(),3));
    Serial.println(">" + prefix+" y :" + String(getY(), 3));
    Serial.println(">" + prefix+" Th :" + String(getTheta(), 3));
    Serial.println(">" + prefix + " xy :" + String(getX(),3) + ":" + String(getY(),3) + "|xy");
}

//----------End Vector Oriented Class----------//




//----------Kinetic Class----------//

Kinetic::Kinetic(float x, float y, float th, float v, float w):VectorOriented(x,y,th){
    this->v=v;
    this->w=w;
}

bool Kinetic::operator==(Kinetic const &other){
    return VectorOriented::operator==(other) && abs(v-other.v)<1e-6 && abs(w-other.w)<1e-6;
}

float Kinetic::getTranslationSpeed(){
    return v;
}

void Kinetic::setTranslationSpeed(float ts){
    v=ts;
}

float Kinetic::getRotationSpeed(){
    return w;
}

void Kinetic::setRotationSpeed(float rs){
    w=rs;
}

void Kinetic::printDebug(const String& prefix)
{
    Serial.println(">" + prefix+" x :" + String(getX(),3));
    Serial.println(">" + prefix+" y :" + String(getY(), 3));
    Serial.println(">" + prefix + " xy :" + String(getX(),3) + ":" + String(getY(),3) + "|xy");
    Serial.println(">" + prefix+" Th :" + String(getTheta(), 3));
    Serial.println(">" + prefix+" v :" + String(getTranslationSpeed(), 3));
    Serial.println(">" + prefix+" w :" + String(getRotationSpeed(), 3));
}



//----------End Kinetic Class----------//