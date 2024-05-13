#include "Math_functions.h"

//----------Polynome Class----------//

Polynome::Polynome(float a0, float a1, float a2, float a3, float a4, float a5, float a6){
    K[0]=a0;
    K[1]=a1;
    K[2]=a2;
    K[3]=a3;
    K[4]=a4;
    K[5]=a5;
    K[6]=a6;
}





void Polynome::initPolynome(){
    for (int i = 0 ; i <= 6 ; i++){
        K[i]=0.0;
    }
}

float Polynome::getCoeff(int degreCoeff){
    if (degreCoeff<=6){
        return K[degreCoeff];
    }
    else{
        return 0.0;
    }
}




void Polynome::setCoeff(int degreCoeff, float value){
    K[degreCoeff] = value;
}



float Polynome::f(float x){
    float out = 0.0;
    for (int i = 0; i <=6; i += 1){
        out += K[i] * pow(x,i);
    }
    return out;
}

float Polynome::df(float x){
    float out = 0.0;
    for (int i = 1; i <=6; i += 1){
        out += K[i] * pow(x,i-1) * i;
    }
    return out;
}



Polynome Polynome::derivatePolynome(){
    Polynome out;
    for (int i = 0; i < 6; i++){
        out.K[i] = (float)(i + 1) * K[i + 1];
        
    }
    return out;
}

void Polynome::setPolynome(float a0, float a1, float a2, float a3, float a4, float a5, float a6){
    K[0] = a0;
    K[1] = a1;
    K[2] = a2;
    K[3] = a3;
    K[4] = a4;
    K[5] = a5;
    K[6] = a6;
}

Polynome derivativePtr(Polynome *P)
{
    Polynome out;
    out = Polynome();
    for (int d = 0; d < 3 - 1; d += 1)
    {
        out.setCoeff(d, (float)(d + 1) * P-> getCoeff(d + 1)) ;
    }
    return out;
}


//-------------Friends-------------------//
Polynome operator*(const Polynome &lhs, const Polynome &rhs ){
    Polynome out;
    for (int k = 0; k <=6; k += 1){
        for (int i = 0; i <= k; i += 1){
            out.K[k] += lhs.K[i] * rhs.K[k - i];
        }
    }
    return out;
}

Polynome operator+(const Polynome &lhs, const Polynome &rhs){
    Polynome out;
    for (int i = 0 ; i<=6 ; i++){
        out.K[i]=lhs.K[i] + rhs.K[i];
    }
    return out;
}

Polynome operator-(const Polynome &lhs, const Polynome &rhs){
    Polynome out;
    for (int i = 0 ; i<=6 ; i++){
        out.K[i]=lhs.K[i] - rhs.K[i];
    }
    return out;
}



Polynome operator+=(Polynome &lhs, const Polynome &rhs){
    for (int i = 0 ; i<=6 ; i++){
        lhs.K[i] += rhs.K[i];
    }
    return lhs;
}

bool operator==(const Polynome &lhs, const Polynome &rhs){
    bool res = true;
    for (int i = 0 ; i <= 6 ; i++){
        if (abs(lhs.K[i] - rhs.K[i]) >= 1e-6){
            res = false;
        }
    }
    return res;
}
//----------End Polynome Class----------//



//----------Trapezoidal Function Class----------//

Trapezoidal_Function::Trapezoidal_Function(float upRamp, float downRamp, float maxSpeed, float realDistance){
    _upRamp=upRamp;
    _downRamp=downRamp;
    _maxSpeed=maxSpeed;
    _realDistance=realDistance;
    computeDuration();
}

void Trapezoidal_Function::setTrapezoidalFunction(float upRamp, float downRamp, float maxSpeed, float realDistance){
    _upRamp=upRamp;
    _downRamp=downRamp;
    _maxSpeed=maxSpeed;
    _realDistance=realDistance;
    computeDuration();
}

void Trapezoidal_Function::setZeroTrapezoidalFunction(){
    _upRamp = 1.0;
    _downRamp = 1.0;
    _maxSpeed = 0.0;
    _realDistance = 0.0;
    duration = 0.0;
}

void Trapezoidal_Function::computeDuration(){
    float averageInvertRamp = (1.0/_upRamp + 1.0/_downRamp)/2.0;
    float theoricalDistance = pow(_maxSpeed,2)*averageInvertRamp; //Distance theorique pour avoir une fonction triangulaire parfaite (pic en maxSpeed) ou distance d'acceleration et de freinage
    if (theoricalDistance > _realDistance){ //la rampe est triangulaire : pas le temps d'atteindre la vitesse maximale
        _maxSpeed = sqrt(_realDistance/averageInvertRamp);
        duration = _maxSpeed/_upRamp + _maxSpeed/_downRamp;
        boolTriangular = true;
        tAcceleration = _maxSpeed/_upRamp;
    }
    else{ //Sinon fonction trapezoidale : ajout de chaque temps : temps d'acceleration, de freinage et de croisiere
        duration = _maxSpeed/_upRamp + _maxSpeed/_downRamp + (_realDistance-theoricalDistance)/_maxSpeed;
        boolTriangular = false;
        tAcceleration = _maxSpeed/_upRamp;
    }
}

float Trapezoidal_Function::getDuration(){
    return duration;
}

float Trapezoidal_Function::f(float t){
    float out = 0.0;
    if (t>=0.0){

        if (boolTriangular){

            if (t < tAcceleration){
                out = t * _upRamp;
            }
            else if (t < duration){
                out = (duration - t)*_downRamp;
            }
        }

        else{
            if (t < tAcceleration){
                out = t * _upRamp;
            }
            else if (t < duration - _maxSpeed /_downRamp){
                out = _maxSpeed;
            }
            else if (t < duration){
                out = (duration - t)*_downRamp;
            }
        }

    }
    return out;
}

//----------End Trapezoidal Function Class----------//