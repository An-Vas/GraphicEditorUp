#ifndef GRAPHICEDITORUP_FILEMANAGER_H
#define GRAPHICEDITORUP_FILEMANAGER_H

#include <fstream>
#include "../GraphicAlgorithms/ColorSpace.h"
class FileManager {
public:
    static std::ifstream chooseAndOpenFile (unsigned char ** arrBytesP, int * width, int * height, int * extraNum, int chanelMode, ColorSpace colorSpace, float * curGamma);
    static void savePnmFile (unsigned char * arrBytesP, int width, int extraNum, int height, int chanelMode, ColorSpace colorSpace);
    static void savePngFile (unsigned char * arrBytes, int width, int extraNum, int height, int chanelMode, ColorSpace colorSpace, float curGamma);
private:
    static int readNum(std::ifstream &F);
};


#endif //GRAPHICEDITORUP_FILEMANAGER_H
