

#include "GammaChanger.h"
#include <cmath>


unsigned char *GammaChanger::setNewGamma(float lastGamma, float newGamma, unsigned char * arrBytes, int width, int height){
    if (newGamma == lastGamma) return arrBytes;
    for (int i = 0; i < height * width * 3; i++){
        arrBytes[i] = castInt(arrBytes[i], lastGamma, newGamma);
    }
    return arrBytes;
}

int GammaChanger::castInt(int original, float lastGamma, float newGamma){
    float res = original;
    if (lastGamma == 0){
        if ((float) original/255 <= 0.04045f){
            res = (float) original/(12.92f*255)*255;
        } else {
            res = std::pow( (float)((float)original/255 + 0.055f)/1.055f,  2.4f)*255;
        }

    } else if (lastGamma != 1){
        res = std::pow( (float)original/255, 1.0 /((float)1.0/lastGamma))*255;
    }

    if (newGamma == 0){

        if ((float) original/255 <= 0.0031308f){
            res = (float) (original/(255))*12.92f*255;
        } else {
            res = (std::pow( (float)original/255,  1.0f/2.4f)* 1.055f - 0.055f )*255;
        }

    } else if (newGamma != 1){
        res = std::pow(((float)res/255), ((float)1.0/newGamma))*255;
    }

    return std::round(res);
}

