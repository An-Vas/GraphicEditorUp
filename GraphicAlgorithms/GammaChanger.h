

#ifndef GRAPHICEDITORUP_GAMMACHANGER_H
#define GRAPHICEDITORUP_GAMMACHANGER_H

class GammaChanger {
public:
    static unsigned char * setNewGamma(float lastGamma, float newGamma, unsigned char * arrBytes, int width, int height);
    static int castInt(int original, float lastGamma, float newGamma);
    static int castIntSRGB(int original, float lastGamma, float newGamma);
};


#endif //GRAPHICEDITORUP_GAMMACHANGER_H
