#include "BasicAlgorithms.h"

#include <string>
#include <cctype>
#include <vector>
#include <cmath>
#include <fstream>




int BasicAlgorithms::readNum(std::ifstream &F) {
    int num = 0;
    while (true) {
        char c[1];
        F.read(c, 1);

        if ((isspace(c[0]) && num != 0) || !F.good()) {
            break;
        }

        if (isspace(c[0]) && num == 0) {
            continue;
        }
        int curNum = c[0] - '0';
        if (c[0] < '0' || c[0] > '9') {
            return -500;
        }
        num = (num * 10) + curNum;

    }
    return num;
};

int BasicAlgorithms::readNum4(std::ifstream &F) {
    int num = 0;

    char cc[4];
    unsigned char uc[4];
    F.read(cc, 4);

    int pos = F.tellg();

    for (int i = 0; i < 4; i++){
        uc[i] = cc[i];

    }
    num = (uc[0] << 24) | (uc[1] << 16) | (uc[2] << 8) | uc[3];

    return num;
};

int BasicAlgorithms::readNum4fromChar(unsigned char *input) {
    int num = 0;
    for (int i = 0; i < 4; i++) {
        unsigned char c = input[i] + 48;
        int curNum = c - '0';
        if (c == 0) curNum = 0;
        if (!(c == 0 || (c >= '0' || c <= '9'))) {
            return -500;
        }
        num = (num * 10) + curNum;
    }

    return num;
};

int BasicAlgorithms::readNum1fromChar(unsigned char c) {
    int curNum = c - '0' + 48;
    if (c == 0){
        curNum  = 0 ;
    }
    if (!(c == 0 || (c >= '0' || c <= '9'))) {
        return -500;
    }
    return curNum;
};

int BasicAlgorithms::readNum1(std::ifstream &F) {

    char c[1];
    F.read(c, 1);

    int curNum = c[0] - '0';
    if (c[0] < '0' || c[0] > '9') {
        return -500;
    }

    return curNum;
};

float BasicAlgorithms::getNum(std::string input) {
    int numBefore = 0, numAfter = 0;
    bool afterPoint = false;

    for (int i = 0; i < input.size(); i++) {
        if (afterPoint) {
            int curNum = input[i] - '0';
            numAfter = (numAfter * 10) + curNum;

        } else {
            if (input[i] == '.') {
                afterPoint = true;
                continue;
            }
            int curNum = input[i] - '0';
            numBefore = (numBefore * 10) + curNum;
        }

    }

    int p = std::ceil(std::log10(numAfter + 1));
    float num = numBefore + numAfter * (std::pow(0.1f, p));

    return num;
}


unsigned char * BasicAlgorithms::get4Bytes(unsigned char *input, int start) {
    unsigned char *res = new unsigned char[4];
    for (int i = 0; i < 4; i++) {
        res[i] = input[start + i];
    }
    return res;
}