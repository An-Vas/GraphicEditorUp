
#include <cmath>
#include <iostream>
#include <sstream>
#include "LineDrawer.h"

unsigned char *
LineDrawer::drawLine(int sx, int sy, int tx, int ty, int ch1, int ch2, int ch3, float thickness, float transparency,
                     unsigned char *arrBytes, int width, int height, int chanel) {


    if (sx > tx) {
        std::swap(sx, tx);
        std::swap(sy, ty);
    }
    if (sx != tx) {
        if (sy <= ty) {
            float k = (float) (ty - sy) / (tx - sx);
            float a = sy - k * sx;
            float realHeight = thickness / cos(atan(k));

            for (int x = sx; x < tx; x++) {
                float y1 = k * x + a;
                float y2 = k * (x + 1) + a;

                float s = LineDrawer::findSquare(y1, y2, thickness);

                int curPos = LineDrawer::curPos(x, floor(y1), width);

                if (curPos < width * height * 3 && curPos > 0)
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1 - s, arrBytes, chanel);

                curPos += width * 3;

                int counter = 0;
                while (counter < realHeight - 1 && y1 + counter < height) {
                    float tempS = 1;
                    if (y1 - y2 > counter + 1) {
                        tempS =  findSquare((float)(y1 - a) / (k), (float)(y2 - a + counter) / (k), thickness);
                    }
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, tempS, arrBytes, chanel);
                    curPos += width * 3;
                    counter++;
                }
                if (y2 + counter < height) {
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                }

            }
            if (thickness > 1) {
                float cornerWidth = (float) thickness * sin(atan(k));
                float cornerHight = (float) thickness * cos(atan(k));

                float startY = ty, finY = k * (tx + cornerWidth) + a;

                for (int x = tx; x < tx + cornerWidth + 1 && x < width; x++) {

                    startY = ty + (realHeight - cornerHight -
                                   ((float) ((float) (cornerWidth - x + tx) * (realHeight - cornerHight)) /
                                    (cornerWidth))) ;
                    finY = ty + realHeight - cornerHight + (((cornerWidth - x + tx) * cornerHight) / (cornerWidth)) + 1;


                    float y1 = k * x + a;
                    float y2 = k * (x + 1) + a;

                    float s = LineDrawer::findSquare(y1, y2, thickness);
                    int curPos = LineDrawer::curPos(x, floor(startY), width);

                    if (curPos < width * height * 3)
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1 - s, arrBytes, chanel);
                    curPos += width * 3;

                    for (int y = startY; y < finY && y < height && curPos < width * height * 3 && curPos > 0; y++) {
                        if (!(y > finY - 2 && x == tx) && curPos < width * height * 3) {
                            squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1, arrBytes, chanel);
                            curPos += width * 3;
                        }
                    }

                    if (finY + 1 < height && finY < ty + realHeight && x != tx && curPos < width * height * 3) {
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    }
                }


                for (int x = sx - 1; x > sx - cornerWidth -1 && x >= 0; x--) {

                    startY = sy + cornerHight -
                             ((float) ((float) (cornerWidth - sx + x) * (cornerHight)) /
                              (cornerWidth)) - 1;
                    finY = sy + (cornerHight) +
                           (((cornerWidth - sx + x) * (realHeight - cornerHight)) / (cornerWidth)) - 1;

                    if (finY < 0 || startY < 0) break;

                    float y1 = k * x + a;
                    float y2 = k * (x - 1) + a;

                    float s = LineDrawer::findSquare(y2, y1, thickness);
                    int curPos = LineDrawer::curPos(x, floor(startY), width);
                    if (curPos < width * height * 3 && curPos > 0)
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    curPos += width * 3;


                    for (int y = startY; y < finY && y > 0 && curPos < width * height * 3 && curPos > 0; y++) {
                        if (!(y > finY - 2 && x == tx)) {
                            squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1, arrBytes, chanel);
                            curPos += width * 3;
                        }
                    }


                    if (finY + 1 > 0 && finY < sy + realHeight && x != tx && curPos < width * height * 3 &&
                        curPos > 0) {
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    }
                }
            }


        } else if (sy > ty) {
            float k = (float) (ty - sy) / (tx - sx);
            float a = sy - k * sx;
            float realHeight = thickness / cos(atan(k));
            for (int x = sx + 1; x < tx; x++) {

                float y1 = k * x + a;
                float y2 = k * (x + 1) + a;

                float s = LineDrawer::findSquare(y2, y1, thickness);

                int curPos = LineDrawer::curPos(x, floor(y2), width);

                if (curPos < width * height * 3 && curPos > 0)
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                curPos -= width * 3;

                int counter = 0;

                while (counter < realHeight - 1 && y2 + counter > 0 && curPos < width * height * 3 && curPos > 0) {
                    float tempS = 1;
                    if (y1 - y2 > counter + 1) {
                        tempS = 1 - findSquare((float)(y2 - a) / (k), (float)(y1 - a) / (k), thickness);
                    }
                    if (tempS == 0) tempS = 1;
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, tempS, arrBytes, chanel);

                    curPos -= width * 3;
                    counter++;
                }
                if (y1 + counter > 0 && curPos < width * height * 3 && curPos > 0) {
                    squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1 - s, arrBytes, chanel);

                }
            }


            if (thickness > 1) {

                float cornerWidth = (float) -thickness * sin(atan(k));
                float cornerHight = (float) thickness * cos(atan(k));

                float startY = ty, finY = k * (tx + cornerWidth) + a;

                for (int x = tx; x < tx + cornerWidth && x < width; x++) {

                    startY = (ty - realHeight) + cornerHight -
                             ((float) ((float) (cornerWidth - x + tx) * (cornerHight)) /
                              (cornerWidth)) - 1;
                    finY = (ty - realHeight) + cornerHight +
                           (((cornerWidth - x + tx) * (realHeight - cornerHight)) / (cornerWidth)) - 2;

                    if (startY < 0) startY = 0;

                    float y1 = k * x + a;
                    float y2 = k * (x + 1) + a;

                    float s = LineDrawer::findSquare(y1, y2, thickness);
                    int curPos = LineDrawer::curPos(x, (startY), width);
                    if (curPos < width * height * 3 && curPos > 0)
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1 - s, arrBytes, chanel);
                    curPos += width * 3;

                    for (int y = startY; y < finY && y < height; y++) {
                        if ((curPos < width * height * 3 && curPos > 0))
                            squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1, arrBytes, chanel);
                        curPos += width * 3;

                    }

                    if (finY + 1 < height && finY < ty + realHeight) {
                        if ((curPos < width * height * 3 && curPos > 0))
                            squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    }
                }


                for (int x = sx ; x > sx - cornerWidth - 1 && x >= 0; x--) {

                    startY = sy - realHeight + realHeight - cornerHight -
                             ((float) ((float) (cornerWidth - sx + x) * (realHeight - cornerHight)) /
                              (cornerWidth)) - 2;
                    finY = sy - realHeight + (realHeight - cornerHight) +
                           (((cornerWidth - sx + x) * cornerHight) / (cornerWidth)) - 3;

                    if (startY < 0) startY = 0;

                    if (finY < 0 || startY < 0) break;

                    float y1 = k * x + a;
                    float y2 = k * (x - 1) + a;

                    float s = LineDrawer::findSquare(y2, y1, thickness);
                    int curPos = LineDrawer::curPos(x, floor(startY), width);
                    if (curPos < width * height * 3 && curPos > 0)
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    curPos += width * 3;

                    for (int y = startY; y < finY && y > 0 && curPos < width * height * 3 && curPos > 0; y++) {
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1, arrBytes, chanel);
                        curPos += width * 3;
                    }

                    if (finY + 1 > 0 && finY < sy + realHeight && x != tx && curPos < width * height * 3 &&
                        curPos > 0) {
                        squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, s, arrBytes, chanel);
                    }
                }
            }
        }
    } else if (sx == tx) {

        if (sy > ty) {
            std::swap(sx, tx);
            std::swap(sy, ty);
        }
        for (int y = sy; y < ty; y++) {
            for (int x = sx; x < tx + thickness && x < width; x++) {
                int curPos = LineDrawer::curPos(x, y, width);
                squareUsingDrawing(curPos, ch1, ch2, ch3, transparency, 1, arrBytes, chanel);
            }
        }
    }


    return arrBytes;
}

int LineDrawer::curPos(int x, int y, int width) {
    return ((y - 1) * width * 3 + x * 3);
}


void LineDrawer::squareUsingDrawing(int curPos, int ch1, int ch2, int ch3, float transparency, float s,
                                    unsigned char *arrBytes, int chanel) {
    if (chanel != 0){

        char Sample = arrBytes[curPos + chanel - 1], ch;
        if (chanel == 1) ch = ch1;
        if (chanel == 2) ch = ch2;
        if (chanel == 3) ch = ch3;

        arrBytes[curPos] =
                (ch * s + Sample * (1 - s)) * transparency + Sample * (1 - transparency);
        arrBytes[curPos + 1] = (ch * s + Sample * (1 - s)) * transparency +
                               Sample * (1 - transparency);
        arrBytes[curPos + 2] = (ch * s + Sample * (1 - s)) * transparency +
                               Sample * (1 - transparency);
    } else {
        arrBytes[curPos] = (ch1 * s + arrBytes[curPos] * (1 - s)) * transparency + arrBytes[curPos] * (1 - transparency);
        arrBytes[curPos + 1] = (ch2 * s + arrBytes[curPos + 1] * (1 - s)) * transparency +
                               arrBytes[curPos + 1] * (1 - transparency);
        arrBytes[curPos + 2] = (ch3 * s + arrBytes[curPos + 2] * (1 - s)) * transparency +
                               arrBytes[curPos + 2] * (1 - transparency);
    }

}

float LineDrawer::findSquare(float y1, float y2, float thickness) {
    float up = (float) (y2 - y1);
    if (floor(y1) != floor(y2) && up != 1)
        up = (float) (y1 - floor(y1));
    if (up < 0) up *= (-1);
    if (thickness < 1) up = (y2 - y1 + thickness);
    return (float) (up) / (2.0f);
};


