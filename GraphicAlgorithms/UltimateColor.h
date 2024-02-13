
#ifndef GRAPHICEDITORUP_ULTIMATECOLOR_H
#define GRAPHICEDITORUP_ULTIMATECOLOR_H



class UltimateColor {
public:
    int x;
    int y;
    int z;

    UltimateColor(int x, int y, int z);
    UltimateColor();
    bool operator==(UltimateColor other);
};


#endif //GRAPHICEDITORUP_ULTIMATECOLOR_H
