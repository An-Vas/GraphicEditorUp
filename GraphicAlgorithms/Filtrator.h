
#ifndef GRAPHICEDITORUP_FILTRATOR_H
#define GRAPHICEDITORUP_FILTRATOR_H


class Filtrator {
public:
    static unsigned char * threshold2classesFilter(unsigned char * arrBytes, int width, int height, int threshold);
    static unsigned char * threshold3classesFilter(unsigned char * arrBytes, int width, int height, int threshold1, int threshold2);
    static unsigned char * thresholdOtsu2classesFilter(unsigned char * arrBytes, int width, int height);
    static unsigned char * thresholdOtsu3classesFilter(unsigned char * arrBytes, int width, int height);
    static unsigned char * medianFilter(unsigned char * arrBytes, int width, int height, int r);
    static unsigned char * gaussFilter(unsigned char * arrBytes, int width, int height, float s);
    static unsigned char * boxBlurFilter(unsigned char * arrBytes, int width, int height, float r);
    static unsigned char * unsharpedFilter(unsigned char * arrBytes, int width, int height, float amount, float s, int threshold);
    static unsigned char * sharpedFilter(unsigned char * arrBytes, int width, int height, float sharpness);
    static unsigned char * sobelFilter(unsigned char * arrBytes, int width, int height);
    static unsigned char * cannyFilter(unsigned char * arrBytes, int width, int height);
private:
    static int curPos(int x, int y, int width);
    static int curPosExtra(int x, int y, int width, int height);
};


#endif //GRAPHICEDITORUP_FILTRATOR_H
