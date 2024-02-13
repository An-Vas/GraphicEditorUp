
#include <iostream>
#include "GistagramChanger.h"

unsigned char *
GistagramChanger::gistagrammChange(unsigned char *arrBytes, int width, int height, float unimportantPart) {
    int *chanel1count = new int[256]{0};
    int *chanel2count = new int[256]{0};
    int *chanel3count = new int[256]{0};

    for (int i = 0; i < height * width * 3; i += 3) {
        int r = arrBytes[i], g = arrBytes[i + 1], b = arrBytes[i + 2];
        chanel1count[r]++;
        chanel2count[g]++;
        chanel3count[b]++;
    }
    int pixelCount = height * width;
    int remain = pixelCount;

    bool more = true;
    for (int i = 0; i < 256 && more; i++){
        if ((remain - chanel1count[i])/pixelCount > unimportantPart){
            chanel1count[i] = 0;
        } else {
            more = false;
        }
    }

    remain = pixelCount;
    for (int i = 0; i < 256 && more; i++){
        if ((remain - chanel2count[i])/pixelCount > unimportantPart){
            chanel2count[i] = 0;
        } else {
            more = false;
        }
    }

    remain = pixelCount;
    for (int i = 0; i < 256 && more; i++){
        if ((remain - chanel3count[i])/pixelCount > unimportantPart){
            chanel3count[i] = 0;
        } else {
            more = false;
        }
    }

    for (int i = 255; i > 0 && more; i--){
        if ((remain - chanel1count[i])/pixelCount > unimportantPart){
            chanel1count[i] = 0;
        } else {
            more = false;
        }
    }

    remain = pixelCount;
    for (int i = 255; i > 0 && more; i--){
        if ((remain - chanel2count[i])/pixelCount > unimportantPart){
            chanel2count[i] = 0;
        } else {
            more = false;
        }
    }

    remain = pixelCount;
    for (int i = 255; i > 0 && more; i--){
        if ((remain - chanel3count[i])/pixelCount > unimportantPart){
            chanel3count[i] = 0;
        } else {
            more = false;
        }
    }




    int max1 = 0, max2 = 0, max3 = 0;

    for (int i = 255; i >= 0; i--) {
        if (chanel1count[i] != 0) {max1 = i; break;}
    }
    for (int i = 255; i >= 0; i--) {
        if (chanel2count[i] != 0) {max2 = i; break;}
    }
    for (int i = 255; i >= 0; i--) {
        if (chanel3count[i] != 0) {max3 = i; break;}
    }


    int min1 = 0, min2 = 0, min3 = 0;

    for (int i = 0;  i < 256; i++) {
        if (chanel1count[i] != 0) {min1 = i; break;}
    }
    for (int i = 0;  i < 256; i++) {
        if (chanel2count[i] != 0) {min2 = i; break;}
    }
    for (int i = 0;  i < 256; i++) {
        if (chanel3count[i] != 0) {min3 = i; break;}
    }



    int absoluteMax = 0;
    if (max1 >= max2 && max1 >= max3) absoluteMax = max1;
    if (max3 >= max2 && max3 >= max1) absoluteMax = max3;
    if (max2 >= max1 && max2 >= max3) absoluteMax = max2;

    int absoluteMin = 1000;
    if (min1 <= min2 && min1 <= min3) absoluteMin = min1;
    if (min3 <= min2 && min3 <= min1) absoluteMin = min3;
    if (min2 <= min1 && min2 <= min3) absoluteMin = min2;

    int valuesCount = absoluteMax - absoluteMin;

    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            arrBytes[curPos(x, y, width)] = newValue (absoluteMin, valuesCount, arrBytes[curPos(x, y, width)]);
            arrBytes[curPos(x, y, width) + 1] = newValue (absoluteMin, valuesCount, arrBytes[curPos(x, y, width)+1]);
            arrBytes[curPos(x, y, width) + 2] = newValue (absoluteMin, valuesCount, arrBytes[curPos(x, y, width)+2]);
        }
    }

    return arrBytes;
}

int GistagramChanger::curPos(int x, int y, int width) {
    if (y > 0)
        return ((y - 1) * width * 3 + x * 3);
    else return x * 3;
}

int GistagramChanger::newValue (int absoluteMin, int valuesCount, int oldValue){
    int res = (float)((float)(oldValue - absoluteMin)/valuesCount) * 255;

    if (res < 0) res = 0;
    if (res > 255) res = 255;

    return res;
}

