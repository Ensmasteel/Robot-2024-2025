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

Vector& Vector::operator+=(const Vector& other) {
    _x += other._x;
    _y += other._y;
    return *this;
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

void Vector::printTeleplot(const String& prefix)
{
    Logger::teleplot(">" + prefix+" x :" + String(getX(), 3));
    Logger::teleplot(">" + prefix+" y :" + String(getY(), 3));
    Logger::teleplot(">" + prefix+" xy :" + String(getX(), 3) + ":" + String(getY(), 3) + "|xy");
}

//----------End Vector Class----------//




//----------Vector Oriented Class----------//

VectorOriented::VectorOriented(float x, float y,float th):Vector(x,y){
    theta=th;
}

bool VectorOriented::operator==(VectorOriented const &other){
    return Vector::operator==(other) && abs(theta - other.theta) <=1e-6;
}

VectorOriented& VectorOriented::operator+=(const VectorOriented& other) {
    _x += other._x;
    _y += other._y;
    theta += other.theta;
    return *this;
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

void VectorOriented::printTeleplot(const String& prefix)
{
    Logger::teleplot(">" + prefix+" x :" + String(getX(), 3));
    Logger::teleplot(">" + prefix+" y :" + String(getY(), 3));
    Logger::teleplot(">" + prefix+" xy :" + String(getX(), 3) + ":" + String(getY(), 3) + "|xy");
    Logger::teleplot(">" + prefix+" Th :" + String(getTheta(), 3));

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

Kinetic& Kinetic::operator+=(const Kinetic& other) {
    _x += other._x;
    _y += other._y;
    theta += other.theta;
    v += other.v;
    w += other.w;
    return *this;
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

void Kinetic::printTeleplot(const String& prefix)
{

    Logger::teleplot(">" + prefix+" x :" + String(getX(), 3));
    Logger::teleplot(">" + prefix+" y :" + String(getY(), 3));
    Logger::teleplot(">" + prefix+" xy :" + String(getX(), 3) + ":" + String(getY(), 3) + "|xy");
    Logger::teleplot(">" + prefix+" Th :" + String(getTheta(), 3));
    Logger::teleplot(">" + prefix+" v :" + String(getTranslationSpeed(), 3));
    Logger::teleplot(">" + prefix+" w :" + String(getRotationSpeed(), 3));

}

//----------End Kinetic Class----------//