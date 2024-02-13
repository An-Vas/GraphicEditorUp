
#ifndef GRAPHICEDITORUP_SCALER_H
#define GRAPHICEDITORUP_SCALER_H


class Scaler {
public:
    static unsigned char * nearlestNeigboughAlorithm(unsigned char * arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight, float offsetX, float offsetY);
    static unsigned char * bilinearAlorithm(unsigned char * arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight, float offsetX, float offsetY);
    static unsigned char * bcSplainAlorithm(unsigned char * arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight, float b, float c, float offsetX, float offsetY);
    static unsigned char * lancosAlorithm(unsigned char * arrBytes, int originalWidth, int originalHeight, int newWidth, int newHeight, float offsetX, float offsetY);
    static unsigned char * sizeChange(unsigned char * arrBytes,  int originalWidth, int originalHeight, int x, int y);
private:
    static int curPos(int x, int y, int width);
    static float lancosFun(float x);
};


#endif //GRAPHICEDITORUP_SCALER_H
