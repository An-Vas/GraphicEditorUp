
#ifndef GRAPHICEDITORUP_BASICALGORITHMS_H
#define GRAPHICEDITORUP_BASICALGORITHMS_H

#include <iostream>

class BasicAlgorithms {

public:
    static int readNum(std::ifstream &F);

    static int readNum4(std::ifstream &F) ;
    static int readNum4fromChar(unsigned char * input);

    static int readNum1(std::ifstream &F);

    static float getNum(std::string input);
    static unsigned char* get4Bytes(unsigned char* input, int start);

    static int readNum1fromChar(unsigned char c);



};

#endif //GRAPHICEDITORUP_BASICALGORITHMS_H
