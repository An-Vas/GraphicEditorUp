#ifndef GRAPHICEDITORUP_DISIRING_H
#define GRAPHICEDITORUP_DISIRING_H

#include <ctime>
#include <cstdlib>
#include "GammaChanger.h"

class Disiring {
public:
    static unsigned char * randomAlorithm(int bitness, unsigned char * arrBytes, int width, int height, int gamma);
    static unsigned char * orderAlorithm(int bitness, unsigned char * arrBytes, int width, int height, int gamma);
    static unsigned char * FloydSteinsbergAlorithm(int bitness, unsigned char * arrBytes, int width, int height, int gamma);
    static unsigned char * AtkinsonAlorithm(int bitness, unsigned char * arrBytes, int width, int height, int gamma);
private:
    static int findNearlestColor(int bitness, int pixel);
    static int curPos(int x, int y, int width);
};


#endif //GRAPHICEDITORUP_DISIRING_H
