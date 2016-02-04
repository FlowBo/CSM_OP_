
#include "Mirror.hpp"


void Mirror::setColor(Color c){
    mColor = c;
}

void Mirror::setDiffence(double d){
    mDifference = d;
}

void Mirror::setRotationalPosition(double rp){
    mRotationalPosition = rp;
}

Color Mirror::getColor(){
    return mColor;
}

double Mirror::getRotationalPosition(){
    return mRotationalPosition;
}

double Mirror::getDifference(){
    return mDifference;
}

void Mirror::setId(int id){
    mId = id;
}

int Mirror::getId(){
    return mId;
}