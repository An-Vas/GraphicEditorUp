

#include <cmath>
#include <iostream>
#include "Scaler.h"

unsigned char *
Scaler::nearlestNeigboughAlorithm(unsigned char *arrBytes, int originalWidth, int originalHeight, int newWidth,
                                  int newHeight, float offsetX, float offsetY) {
    //сначала по горизонтали, потом по вертикали
    unsigned char *newArrBytes = new unsigned char[newWidth * newHeight * 3 + newWidth * newHeight + originalWidth * originalHeight];
    float newPixelsToOneOldPixelX = (float) ((float) (newWidth - 1) / (originalWidth - 1));
    float newPixelsToOneOldPixelY = (float) ((float) (newHeight - 1) / (originalHeight - 1));

    for (int y = 0; y < originalHeight; y++) {

        for (int x = 0; x < newWidth; x++) {

            int nearlestNeigbough = std::round((float) ((float) floor(x+offsetX) / (float) newPixelsToOneOldPixelX));
            newArrBytes[curPos(x, y, newWidth)] = arrBytes[curPos(nearlestNeigbough, y, originalWidth)];
            newArrBytes[curPos(x, y, newWidth) + 1] = arrBytes[curPos(nearlestNeigbough, y, originalWidth) + 1];
            newArrBytes[curPos(x, y, newWidth) + 2] = arrBytes[curPos(nearlestNeigbough, y, originalWidth) + 2];
        }
    }



    for (int x = 0; x < newWidth; x++) {

        for (int y = 1; y < newHeight ; y++) {
            int nearlestNeigbough = std::round((float) ((float) floor(y+offsetY) / (float) newPixelsToOneOldPixelY));

            newArrBytes[curPos(x, y, newWidth)] = arrBytes[curPos((x) / newPixelsToOneOldPixelX, nearlestNeigbough,originalWidth)];
            newArrBytes[curPos(x, y, newWidth) + 1] = arrBytes[ curPos((x) / newPixelsToOneOldPixelX, nearlestNeigbough, originalWidth) + 1];
            newArrBytes[curPos(x, y, newWidth) + 2] = arrBytes[curPos((float) (x) / newPixelsToOneOldPixelX, nearlestNeigbough, originalWidth) + 2];
        }

        newArrBytes[curPos(x, newHeight, newWidth)] = arrBytes[curPos((x) / newPixelsToOneOldPixelX, originalHeight,originalWidth)];
        newArrBytes[curPos(x, newHeight, newWidth) + 1] = arrBytes[ curPos((x) / newPixelsToOneOldPixelX, originalHeight, originalWidth) + 1];
        newArrBytes[curPos(x, newHeight, newWidth) + 2] = arrBytes[curPos((float) (x) / newPixelsToOneOldPixelX, originalHeight, originalWidth) + 2];;

    }

    return newArrBytes;

}

unsigned char *
Scaler::bilinearAlorithm(unsigned char *arrBytes, int originalWidth, int originalHeight, int newWidth,
                         int newHeight, float offsetX, float offsetY) {
    newHeight++;
    unsigned char *newArrBytes = new unsigned char[newWidth * newHeight * 3];
    float newPixelsToOneOldPixelX = (float) ((float) (newWidth - 1) / (originalWidth - 1));
    float newPixelsToOneOldPixelY = (float) ((float) (newHeight - 1) / (originalHeight - 1));

    for (int x = 0; x < newWidth; x++) {
        for (int y = 0; y < newHeight; y++) {

            int leftCoordinate = x / newPixelsToOneOldPixelX;
            int rightCoordinate = x / newPixelsToOneOldPixelX + 1;
            int upCoordinate = y / newPixelsToOneOldPixelY;
            int downCoordinate = y / newPixelsToOneOldPixelY + 1;

            if (leftCoordinate < 0) leftCoordinate = 0;
            if (upCoordinate < 0) upCoordinate = 0;
            if (rightCoordinate >= originalWidth) rightCoordinate = originalWidth - 1;
            if (downCoordinate >= originalHeight) downCoordinate = originalHeight - 1;


            float deltaXLeft =
                    (float) ((float) floor(x+offsetX) / newPixelsToOneOldPixelX) - floor((float) floor(x+offsetX) / newPixelsToOneOldPixelX);
            float deltaXRight =
                    ceil((float) floor(x+offsetX) / newPixelsToOneOldPixelX) - (float) ((float) floor(x+offsetX) / newPixelsToOneOldPixelX);
            float deltaYUp = (float) ((float) floor(y+offsetY) / newPixelsToOneOldPixelY) - floor((float) floor(y+offsetY) / newPixelsToOneOldPixelY);
            float deltaYDown =
                    ceil((float) floor(y+offsetY) / newPixelsToOneOldPixelY) - (float) ((float) floor(y+offsetY) / newPixelsToOneOldPixelY);

            if (deltaXLeft == originalWidth && deltaXRight == originalWidth) deltaXLeft = originalWidth - 2;
            if (deltaYUp == originalHeight && deltaYDown == originalHeight) deltaYUp = originalHeight - 2;

            if (deltaXLeft == 0 && deltaXRight == 0) deltaXRight = 1;
            if (deltaYUp == 0 && deltaYDown == 0) deltaYDown = 1;

            int pixelLeftUp = arrBytes[curPos(leftCoordinate, upCoordinate, originalWidth)];
            int pixelLeftDown = arrBytes[curPos(leftCoordinate, downCoordinate, originalWidth)];
            int pixelRightUp = arrBytes[curPos(rightCoordinate, upCoordinate, originalWidth)];
            int pixelRightDown = arrBytes[curPos(rightCoordinate, downCoordinate, originalWidth)];


            newArrBytes[curPos(x, y, newWidth)] = (deltaXRight * pixelLeftUp + deltaXLeft * pixelRightUp) * deltaYDown +
                                                  (deltaXRight * pixelLeftDown + deltaXLeft * pixelRightDown) *
                                                  deltaYUp;

            pixelLeftUp = arrBytes[curPos(leftCoordinate, upCoordinate, originalWidth) + 1];
            pixelLeftDown = arrBytes[curPos(leftCoordinate, downCoordinate, originalWidth) + 1];
            pixelRightUp = arrBytes[curPos(rightCoordinate, upCoordinate, originalWidth) + 1];
            pixelRightDown = arrBytes[curPos(rightCoordinate, downCoordinate, originalWidth) + 1];

            newArrBytes[curPos(x, y, newWidth) + 1] =
                    (deltaXRight * pixelLeftUp + deltaXLeft * pixelRightUp) * deltaYDown +
                    (deltaXRight * pixelLeftDown + deltaXLeft * pixelRightDown) * deltaYUp;

            pixelLeftUp = arrBytes[curPos(leftCoordinate, upCoordinate, originalWidth) + 2];
            pixelLeftDown = arrBytes[curPos(leftCoordinate, downCoordinate, originalWidth) + 2];
            pixelRightUp = arrBytes[curPos(rightCoordinate, upCoordinate, originalWidth) + 2];
            pixelRightDown = arrBytes[curPos(rightCoordinate, downCoordinate, originalWidth) + 2];

            newArrBytes[curPos(x, y, newWidth) + 2] =
                    (deltaXRight * pixelLeftUp + deltaXLeft * pixelRightUp) * deltaYDown +
                    (deltaXRight * pixelLeftDown + deltaXLeft * pixelRightDown) * deltaYUp;

        }
    }


    for (int x = 0; x < newWidth && newWidth < originalWidth; x++){
        newArrBytes[curPos(x, newHeight-1, newWidth)] = 0;
        newArrBytes[curPos(x, newHeight-1, newWidth) + 1] = 0;
        newArrBytes[curPos(x, newHeight-1, newWidth) + 2] = 0;
    }
    return newArrBytes;
}

unsigned char *
Scaler::bcSplainAlorithm(unsigned char *arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight,
                         float b, float c, float offsetX, float offsetY) {
    unsigned char *newArrBytes = new unsigned char[newWidth * newHeight * 3 + newWidth * newHeight + originalWidth * originalHeight * 3];
    float newPixelsToOneOldPixelX = (float) ((float) (newWidth - 1) / (originalWidth - 1));
    float newPixelsToOneOldPixelY = (float) ((float) (newHeight - 1) / (originalHeight - 1));

    for (int y = 0; y < originalHeight; y++) {

        for (int x = 0; x < newWidth; x++) {

            float delta = (float) ((float) floor(x+offsetX) / newPixelsToOneOldPixelX) - floor((float) floor(x+offsetX) / newPixelsToOneOldPixelX);

            int coordinate1 = std::floor((float) ((float) x / (float) newPixelsToOneOldPixelX));
            int coordinate0 = coordinate1 - 1;
            int coordinate2 = coordinate1 + 1;
            int coordinate3 = coordinate1 + 2;

            if (coordinate0 < 0) coordinate0 = 0;
            if (coordinate2 > originalWidth - 1) coordinate2 = originalWidth - 1;
            if (coordinate3 > originalWidth - 1) coordinate3 = originalWidth - 1;


            for (int g = 0; g < 3; g++){
                int pixel1 = arrBytes[curPos(coordinate1, y, originalWidth) + g];
                int pixel0 = arrBytes[curPos(coordinate0, y, originalWidth) + g];
                int pixel2 = arrBytes[curPos(coordinate2, y, originalWidth) + g];
                int pixel3 = arrBytes[curPos(coordinate3, y, originalWidth) + g];
                int value = ((((float) -1 / 6) * b - c) * pixel0 + (((float) -3 / 2) * b - c + 2) * pixel1 +
                             (((float) 3 / 2 * b + c - 2) * pixel2) + ((float) (1 / 6) * b + c) * pixel3) * delta * delta *
                            delta + ((((float) 1 / 2) * b + 2 * c) * pixel0 + (2 * b + c - 3) * pixel1 +
                                     (((float) -5 / 2) * b - 2 * c + 3) * pixel2 - c * pixel3) * delta * delta +
                            ((((float) -1 / 2) * b - c) * pixel0 + (((float) 1 / 2) * b + c) * pixel2) * delta +
                            (((float) 1 / 6) * b * pixel0) + (((float) -1 / 3) * b + 1) * pixel1 +
                            ((float) 1 / 6) * b * pixel2;

                if (value > 255) value = 255;
                if (value < 0)value = 0;
                if (originalHeight <= newHeight) newArrBytes[curPos(x, y, newWidth) + g] = value;
                else newArrBytes[curPos(x, y*newPixelsToOneOldPixelY, newWidth) + g] = value;

            }


        }
    }

    unsigned char *newArrBytesCopy = new unsigned char[newWidth * newHeight * 3  + newWidth * newHeight + originalWidth * originalHeight * 3];
    for (int i = 0; i < newWidth * newHeight * 3; i++) {
        newArrBytesCopy[i] = newArrBytes[i];
    }



    for (int x = 0; x < newWidth; x++) {

        for (int y = 0; y < newHeight; y++) {

            float delta = (float) ((float) floor(y+offsetY) / newPixelsToOneOldPixelY) - floor((float) floor(y+offsetY) / newPixelsToOneOldPixelY);

            int coordinate1 = std::round((float) ((float) y / (float) newPixelsToOneOldPixelY));
            int coordinate0 = coordinate1 - 1;
            int coordinate2 = coordinate1 + 1;
            int coordinate3 = coordinate1 + 2;

            if (coordinate0 < 0) coordinate0 = 0;
            if (coordinate2 > originalHeight - 1) coordinate2 = originalHeight - 1;
            if (coordinate3 > originalHeight - 1) coordinate3 = originalHeight - 1;
            if (originalHeight <= newHeight)
                for (int g = 0; g < 3; g++){
                    int pixel1 = newArrBytesCopy[curPos(x, coordinate1, newWidth) + g];
                    int pixel0 = newArrBytesCopy[curPos(x, coordinate0, newWidth)+ g];
                    int pixel2 = newArrBytesCopy[curPos(x, coordinate2, newWidth)+ g];
                    int pixel3 = newArrBytesCopy[curPos(x, coordinate3, newWidth)+ g];


                    int value = ((((float) -1 / 6) * b - c) * pixel0 + (((float) -3 / 2) * b - c + 2) * pixel1 +
                                 (((float) 3 / 2 * b + c - 2) * pixel2) + ((float) (1 / 6) * b + c) * pixel3) * delta * delta *
                                delta + ((((float) 1 / 2) * b + 2 * c) * pixel0 + (2 * b + c - 3) * pixel1 +
                                         (((float) -5 / 2) * b - 2 * c + 3) * pixel2 - c * pixel3) * delta * delta +
                                ((((float) -1 / 2) * b - c) * pixel0 + (((float) 1 / 2) * b + c) * pixel2) * delta +
                                (((float) 1 / 6) * b * pixel0) + (((float) -1 / 3) * b + 1) * pixel1 +
                                ((float) 1 / 6) * b * pixel2;

                    if (value > 255) value = 255;
                    if (value < 0)value = 0;

                    newArrBytes[curPos(x, y, newWidth)+g] = value;
                }
        }
    }

    return newArrBytes;
}

unsigned char *
Scaler::lancosAlorithm(unsigned char *arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight, float offsetX, float offsetY) {


    unsigned char *newArrBytes = new unsigned char[newWidth * newHeight * 3 + newWidth * newHeight + originalWidth * originalHeight * 3];
    float newPixelsToOneOldPixelX = (float) ((float) (newWidth - 1) / (originalWidth - 1));
    float newPixelsToOneOldPixelY = (float) ((float) (newHeight - 1) / (originalHeight - 1));

    for (int y = 0; y < originalHeight; y++) {

        for (int x = 0; x < newWidth; x++) {

            float delta = (float) ((float) floor(x+offsetX) / newPixelsToOneOldPixelX) - floor((float) floor(x+offsetX) / newPixelsToOneOldPixelX);
            float x1 = 0 - delta;
            float x0 = x1 - 1;
            float x2 = x1 + 1;
            float x3 = x1 + 2;

            int coordinate1 = std::round((float) ((float) x / (float) newPixelsToOneOldPixelX));
            int coordinate0 = coordinate1 - 1;
            int coordinate2 = coordinate1 + 1;
            int coordinate3 = coordinate1 + 2;

            if (coordinate0 < 0) coordinate0 = 0;
            if (coordinate2 > originalWidth - 1) coordinate2 = originalWidth - 1;
            if (coordinate3 > originalWidth - 1) coordinate3 = originalWidth - 1;

            if (x0 <= -2) x0 = -1.9999999f;
            if (x2 >= 2) x2 = 1.999999f;
            if (x3 >= 2) x3 = 1.999999f;



            for (int g = 0; g < 3; g++){
                int pixel1 = arrBytes[curPos(coordinate1, y, originalWidth) + g];
                int pixel0 = arrBytes[curPos(coordinate0, y, originalWidth) + g];
                int pixel2 = arrBytes[curPos(coordinate2, y, originalWidth) + g];
                int pixel3 = arrBytes[curPos(coordinate3, y, originalWidth) + g];
                int value = 0;
                if (abs(x1) == 0 && ((x == 0) || (x == newWidth - 1))) {
                    value = (pixel1 + pixel2) / 2;
                } else {
                    value = pixel0 * lancosFun(x0) + pixel1 * lancosFun(x1) + pixel2 * lancosFun(x2) +
                            pixel3 * lancosFun(x3);
                }
                if (value > 255) value = 255;
                if (value < 0) value = 0;
                if (originalHeight <= newHeight) newArrBytes[curPos(x, y, newWidth)  + g] = value;
                else newArrBytes[curPos(x, y*newPixelsToOneOldPixelY, newWidth)  + g] = value;
            }



        }
    }



    unsigned char *newArrBytesCopy = new unsigned char[newWidth * newHeight * 3 + originalWidth * originalHeight * 3];
    for (int i = 0; i < newWidth * newHeight * 3; i++) {
        newArrBytesCopy[i] = newArrBytes[i];
    }


    for (int x = 0; x < newWidth; x++) {
        for (int y = 0; y < newHeight; y++) {

            float delta = (float) ((float) floor(y+offsetY) / newPixelsToOneOldPixelY) - floor((float) floor(y+offsetY) / newPixelsToOneOldPixelY);
            float x1 = 0 - delta;
            float x0 = x1 - 1;
            float x2 = x1 + 1;
            float x3 = x1 + 2;

            int coordinate1 = std::round((float) ((float) y / (float) newPixelsToOneOldPixelY));
            int coordinate0 = coordinate1 - 1;
            int coordinate2 = coordinate1 + 1;
            int coordinate3 = coordinate1 + 2;

            if (coordinate0 < 0) coordinate0 = 0;
            if (coordinate2 > originalWidth - 1) coordinate2 = originalWidth - 1;
            if (coordinate3 > originalWidth - 1) coordinate3 = originalWidth - 1;

            if (x0 <= -2) x0 = -1.9999999f;
            if (x2 >= 2) x2 = 1.999999f;
            if (x3 >= 2) x3 = 1.999999f;

            if (originalHeight <= newHeight)
                for (int g = 0; g < 3; g++){
                    int pixel1 = newArrBytesCopy[curPos(x, coordinate1, newWidth) + g];
                    int pixel0 = newArrBytesCopy[curPos(x, coordinate0, newWidth) + g];
                    int pixel2 = newArrBytesCopy[curPos(x, coordinate2, newWidth) + g];
                    int pixel3 = newArrBytesCopy[curPos(x, coordinate3, newWidth) + g];
                    int value = 0;
                    if (abs(x1) == 0 && ((y == 0) || (y == newHeight - 1))) {
                        value = (pixel1 + pixel2) / 2;
                    } else {
                        value = pixel0 * lancosFun(x0) + pixel1 * lancosFun(x1) + pixel2 * lancosFun(x2) +
                                pixel3 * lancosFun(x3);
                    }

                    if (value > 255) value = 255;
                    if (value < 0)value = 0;

                    newArrBytes[curPos(x, y, newWidth) + g] = value;
                }
        }
    }

    return newArrBytes;
}

int Scaler::curPos(int x, int y, int width) {
    if (x >= width) x = width - 1;
    if (y > 0)
        return ((y - 1) * width * 3 + x * 3);
    else return x * 3;
}

float Scaler::lancosFun(float x) {
    if (x == 0) return 1;
    float pi = 3.14;
    int a = 3;

    float upPart = a * (sin(pi * x)) * (sin((float) ((float) pi * (float) x / a)));
    float downPart = pi * pi * x * x;

    float res = (float) ((float) upPart / downPart);
    return res;
}

unsigned char * Scaler::sizeChange(unsigned char * arrBytes,  int originalWidth, int originalHeight, int x, int y){
    unsigned char *newArrBytes = new unsigned char[(originalWidth - abs(x)) * (originalHeight - abs(y)) * 3];

    if (x >= 0 && y >= 0){
        for (int i = x; i < originalWidth; i++){
            for (int j = y; j < originalHeight; j++){
                for (int g = 0; g < 3; g++){
                    newArrBytes[curPos(i - x, j - y, originalWidth - abs(x)) + g] = arrBytes[curPos(i, j, originalWidth) + g];
                }
            }
        }
    } else if (x <= 0 && y <= 0){
        for (int i = 0; i < originalWidth - x; i++){
            for (int j = 0; j < originalHeight - y; j++){
                for (int g = 0; g < 3; g++){
                    newArrBytes[curPos(i, j , originalWidth) + g] = arrBytes[curPos(i + x, j + y, originalWidth + x) + g];
                }
            }
        }
    } else if (x <= 0 && y >= 0){
        for (int i = 0; i < originalWidth - x; i++){
            for (int j = y; j < originalHeight; j++){
                for (int g = 0; g < 3; g++){
                    newArrBytes[curPos(i, j - y, originalWidth) + g] = arrBytes[curPos(i + x, j, originalWidth + x) + g];
                }
            }
        }
    }  else if (x >= 0 && y <= 0){
        for (int i = x; i < originalWidth; i++){
            for (int j = 0; j < originalHeight - y; j++){
                for (int g = 0; g < 3; g++){
                    newArrBytes[curPos(i - x, j , originalWidth - x) + g] = arrBytes[curPos(i, j + y, originalWidth ) + g];
                }
            }
        }
    }

    return newArrBytes;
}
