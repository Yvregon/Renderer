#include "Vector3f.h"

void Vector3f::computeNorm(){
    norm = std::sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

Vector3f::Vector3f(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;

    computeNorm();
}

Vector3f::Vector3f(point_t point){
    x = point.x;
    y = point.y;
    z = point.z;

    computeNorm();
}

void Vector3f::setX(float x){
    this->x = x;
    computeNorm();
}

void Vector3f::setY(float y){
    this->y = y;
    computeNorm();
}

void Vector3f::setZ(float z){
    this->z = z;
    computeNorm();
}

float Vector3f::getX(){
    return x;
}

float Vector3f::getY(){
    return y;
}

float Vector3f::getZ(){
    return z;
}

double Vector3f::getNorm(){
    return norm;
}

Vector3f Vector3f::normalize(){
    x /= norm;
    y /= norm;
    z /= norm;

    computeNorm();

    return *this;
}

Vector3f Vector3f::operator+(Vector3f v){
    v.x = v.x + this->x;
    v.y = v.y + this->y;
    v.z = v.z + this->z;

    v.computeNorm();

    return v;
}

Vector3f Vector3f::operator-(Vector3f v){
    v.x = v.x - this->x;
    v.y = v.y - this->y;
    v.z = v.z - this->z;

    v.computeNorm();

    return v;
}

Vector3f Vector3f::operator*(Vector3f v){
    v.x = v.x * this->x;
    v.y = v.y * this->y;
    v.z = v.z * this->z;

    v.computeNorm();

    return v;
}

Vector3f Vector3f::operator*(float f){
    Vector3f v = Vector3f(0,0,0);

    v.x = f * this->x;
    v.y = f * this->y;
    v.z = f * this->z;

    v.computeNorm();

    return v;
}
