#ifndef GRAPHICEDITORUP_PNGINTERPRETER_H
#define GRAPHICEDITORUP_PNGINTERPRETER_H


#include <iostream>


class PngInterpreter {
public:
    int width;
    int height;
    int bitDepth;
    int colorType;
    int compressionType;
    int filterType;
    int interlaceType;

    float pngGamma = 1;


    std::ifstream &F;

    unsigned char *arrBytesToShow;
    char *arrBytesFromFile;

    PngInterpreter(std::ifstream &F);

    unsigned char * pltch1;
    unsigned char * pltch2;
    unsigned char * pltch3;

    bool readChunk(char* name, char* input, int length);

    void readPng();
    static unsigned long update_crc(unsigned long crc, unsigned char *buf,
                                    int len);
    static int libdeflateCode(unsigned char * input, unsigned char * output, int inputLenth);


private:

    int positionArrBytes = 0;

    void ihdrDecode(char* input);
    void idatDecode(char* input, int length);
    void plteDecode(char* input, int length);

    void gamaDecode(char* input);

    void crcCheck(char* input, char* crc, int inputLength);


    void deflateDecodeAll();
    void defilterAllType2(char* output, int outputLenth);
    void defilterAllType0(char* output, int outputLenth);
    char toLowercase(char c);
    bool is(char c1, char c2);



};


#endif //GRAPHICEDITORUP_PNGINTERPRETER_H
