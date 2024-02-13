#include "ColorSpace.h"
#include "UltimateColor.h"

#ifndef GRAPHICEDITORUP_COLORSPACESINTERPRETER_H
#define GRAPHICEDITORUP_COLORSPACESINTERPRETER_H

class ColorSpacesInterpreter {
public:
    static unsigned char * cast(ColorSpace original, ColorSpace goal, unsigned char * arrBytes, int width, int height);

    static unsigned char * toOneChanel(int chanel, unsigned char * arrBytes, unsigned char * resArrBytes, int width, int height);
    static unsigned char * toAllChanel(unsigned char * arrBytes, unsigned char * resArrBytes, int width, int height);

    static UltimateColor rgbToHsv(UltimateColor original);
    static UltimateColor hsvToRgb(UltimateColor original);

    static UltimateColor rgbToHsl(UltimateColor original);
    static UltimateColor hslToRgb(UltimateColor original);

    static UltimateColor rgbToYCoCg(UltimateColor original);
    static UltimateColor yCoCgToRgb(UltimateColor original);

    static UltimateColor rgbToYCgCr601(UltimateColor original);
    static UltimateColor yCgCr601ToRgb(UltimateColor original);

    static UltimateColor rgbToYCgCr709(UltimateColor original);
    static UltimateColor yCgCr709ToRgb(UltimateColor original);

    static UltimateColor rgbToCmy(UltimateColor original);
    static UltimateColor cmyToRgb(UltimateColor original);


private:
    static unsigned char * castToRgb(ColorSpace original, unsigned char * arrBytes, int width, int height);
};


#endif //GRAPHICEDITORUP_COLORSPACESINTERPRETER_H
