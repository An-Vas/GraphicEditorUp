
#ifndef GRAPHICEDITORUP_GISTAGRAMCHANGER_H
#define GRAPHICEDITORUP_GISTAGRAMCHANGER_H


class GistagramChanger {
public:
    static unsigned char * gistagrammChange(unsigned char * arrBytes, int originalWidth, int originalHeight, float unimportantPart);
private:
    static int curPos(int x, int y, int width);
    static int newValue (int absoluteMin, int valuesCount, int oldValue);
};



#endif //GRAPHICEDITORUP_GISTAGRAMCHANGER_H
