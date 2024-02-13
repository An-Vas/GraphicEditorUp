
#include "Disiring.h"

unsigned char *Disiring::randomAlorithm(int bitness, unsigned char *arrBytes, int width, int height, int gamma) {
    int a = 256 / bitness;
    srand(time(0));

    int curPos = 0;
    for (int i = 0; i < height * width; i++) {

        int randNumSample = rand() % (255 - 0 + 1);
        randNumSample = randNumSample;

        for (int g = 0; g < 3; g++) {
            int floorPart = GammaChanger::castInt(arrBytes[curPos], gamma, 1) / a;
            int t = (floorPart == 0) ? 0 : 1;

            int randNum = randNumSample % (a - 0) + floorPart * a;

            if (randNum > GammaChanger::castInt(arrBytes[curPos], gamma, 1)) {
                arrBytes[curPos] = GammaChanger::castInt(floorPart * a - t, 1, gamma);
            } else {
                arrBytes[curPos] = (GammaChanger::castInt((floorPart + 1) * a - t, 1, gamma));
            }
            curPos++;
        }

    }

    return arrBytes;
}

unsigned char *Disiring::orderAlorithm(int bitness, unsigned char *arrBytes, int width, int height, int gamma) {
    float matrix[8][8]{
            0, 32, 8, 40, 2, 34, 10, 42,
            48, 16, 56, 24, 50, 18, 58, 26,
            12, 44, 4, 36, 14, 46, 6, 38,
            60, 28, 52, 20, 62, 30, 54, 22,
            3, 35, 11, 43, 1, 33, 9, 41,
            51, 19, 59, 27, 49, 17, 57, 25,
            15, 47, 7, 39, 13, 45, 5, 37,
            63, 31, 55, 23, 61, 29, 53, 21
    };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            matrix[i][j] = (float) matrix[i][j] / 64.0f;
        }
    }

    int curPos = 0;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            for (int i = 0; i < 3; i++) {

                if (i == 0 && curPos < width * height * 3 - 3 && arrBytes[curPos] == arrBytes[curPos + 1] &&
                    arrBytes[curPos] == arrBytes[curPos + 2]) {
                    int calculatedPixel = (abs(GammaChanger::castInt(arrBytes[curPos], gamma, 1) +
                                               (255 / bitness) * (matrix[x % 8][y % 8] - 0.5)));
                    if (calculatedPixel > 255) calculatedPixel = 255;
                    arrBytes[curPos] = GammaChanger::castInt(findNearlestColor(bitness, calculatedPixel), 1, gamma);
                    arrBytes[curPos + 1] = arrBytes[curPos];
                    arrBytes[curPos + 2] = arrBytes[curPos];
                    curPos += 3;
                    break;

                } else {
                    int calculatedPixel = (abs(GammaChanger::castInt(arrBytes[curPos], gamma, 1) +
                                               (255 / bitness) * (matrix[x % 8][y % 8] - 0.5)));
                    if (calculatedPixel > 255) calculatedPixel = 255;
                    arrBytes[curPos] = GammaChanger::castInt(findNearlestColor(bitness, calculatedPixel), 1, gamma);
                    curPos++;
                }
            }
        }
    }

    return arrBytes;

}

unsigned char *
Disiring::FloydSteinsbergAlorithm(int bitness, unsigned char *arrBytes, int width, int height, int gamma) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int i = 0; i < 3; i++) {
                int oldpixel = GammaChanger::castInt(arrBytes[curPos(x, y, width) + i], gamma, 1);
                int newpixel = findNearlestColor(bitness, oldpixel);
                arrBytes[curPos(x, y, width) + i] = GammaChanger::castInt(newpixel, 1, gamma);
                int quant_error = oldpixel - newpixel;

                int correctedPixels[4];

                correctedPixels[0] = GammaChanger::castInt(arrBytes[curPos(x + 1, y, width) + i], gamma, 1) +
                                     quant_error * (float) 7 / 16;
                correctedPixels[1] = GammaChanger::castInt(arrBytes[curPos(x - 1, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 3 / 16;
                correctedPixels[2] = GammaChanger::castInt(arrBytes[curPos(x, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 5 / 16;
                correctedPixels[3] = GammaChanger::castInt(arrBytes[curPos(x + 1, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 1 / 16;

                for (int j = 0; j < 4; j++) {
                    if (correctedPixels[j] < 0) correctedPixels[j] = 0;
                    if (correctedPixels[j] > 255) correctedPixels[j] = 255;

                }

                arrBytes[curPos(x + 1, y, width) + i] = GammaChanger::castInt(correctedPixels[0], 1, gamma);
                arrBytes[curPos(x - 1, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[1], 1, gamma);
                arrBytes[curPos(x, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[2], 1, gamma);
                arrBytes[curPos(x + 1, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[3], 1, gamma);
            }


        }
    }

    return arrBytes;
}

unsigned char *Disiring::AtkinsonAlorithm(int bitness, unsigned char *arrBytes, int width, int height, int gamma) {

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            for (int i = 0; i < 3; i++) {
                int oldpixel = GammaChanger::castInt(arrBytes[curPos(x, y, width) + i], gamma, 1);
                int newpixel = findNearlestColor(bitness, oldpixel);
                arrBytes[curPos(x, y, width) + i] = GammaChanger::castInt(newpixel, 1, gamma);
                int quant_error = oldpixel - newpixel;

                int correctedPixels[6];

                correctedPixels[0] = GammaChanger::castInt(arrBytes[curPos(x + 1, y, width) + i], gamma, 1) +
                                     quant_error * (float) 1 / 8;
                correctedPixels[1] = GammaChanger::castInt(arrBytes[curPos(x - 1, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 1 / 8;
                correctedPixels[2] = GammaChanger::castInt(arrBytes[curPos(x, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 1 / 8;
                correctedPixels[3] = GammaChanger::castInt(arrBytes[curPos(x + 1, y + 1, width) + i], gamma, 1) +
                                     quant_error * (float) 1 / 8;

                if (x + 2 < width) {
                    correctedPixels[4] = GammaChanger::castInt(arrBytes[curPos(x + 2, y, width) + i], gamma, 1) +
                                         quant_error * (float) 1 / 8;
                }
                if (y + 2 < height) {
                    correctedPixels[5] = GammaChanger::castInt(arrBytes[curPos(x, y + 2, width) + i], gamma, 1) +
                                         quant_error * (float) 1 / 8;
                }

                for (int j = 0; j < 6; j++) {
                    if (correctedPixels[j] < 0) correctedPixels[j] = 0;
                    if (correctedPixels[j] > 255) correctedPixels[j] = 255;

                }

                arrBytes[curPos(x + 1, y, width) + i] = GammaChanger::castInt(correctedPixels[0], 1, gamma);
                arrBytes[curPos(x - 1, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[1], 1, gamma);
                arrBytes[curPos(x, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[2], 1, gamma);
                arrBytes[curPos(x + 1, y + 1, width) + i] = GammaChanger::castInt(correctedPixels[3], 1, gamma);

                if (x + 2 < width) {
                    arrBytes[curPos(x + 2, y, width) + i] = GammaChanger::castInt(correctedPixels[4], 1, gamma);
                }
                if (y + 2 < height) {
                    arrBytes[curPos(x, y + 2, width) + i] = GammaChanger::castInt(correctedPixels[5], 1, gamma);
                }
            }


        }
    }


    return arrBytes;
}


int Disiring::findNearlestColor(int bitness, int pixel) {

    if (bitness == 1) return 0;
    bitness--;
    int x = 256 / bitness;
    int floorPart = pixel / x;
    int remainderPart = pixel % x;
    int res;
    if (remainderPart > x / 2)
        res = (floorPart + 1) * x - 1;
    else {
        res = floorPart * x - 1;
    }
    if (res == -1) res = 0;

    return res;
}

int Disiring::curPos(int x, int y, int width) {
    if (y > 0)
        return ((y - 1) * width * 3 + x * 3);
    else return x * 3;
}