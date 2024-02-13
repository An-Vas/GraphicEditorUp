#include <iostream>
#include <valarray>
#include "JpegInterpreter.h"
#include "ColorSpacesInterpreter.h"

JpegInterpreter::JpegInterpreter(std::ifstream &F) : F(F) {

}

void JpegInterpreter::readJpeg() {
    bool cont = true;
    while (cont) {
        char *marker = new char[4];
        F.read(marker, 2);
        if (marker[0] == 255 && marker[1] == 217) break;
        if ((int) marker[0] == -1 && (int) marker[1] == -39) break;
        char *lengthc = new char[4];
        F.read(lengthc, 2);
        unsigned char uc[4];
        for (int i = 0; i < 2; i++) {
            uc[i] = lengthc[i];
        }
        int length = (uc[0] << 8) | uc[1];

        if (length > 2) {
            char *data = new char[length - 2];
            F.read(data, length - 2);
            cont = readSector(marker, data, length);
            delete[] data;
        }


        if (!cont)
            break;

        delete[] lengthc;

        delete[] marker;

    }

    char *data = new char[25];
    F.read(data, 25);
    dataDecode(data);
    kvanting();
    inversiveDiscreteCosineTransformation();
    limitToColorRange();
    finaleImageGenerate();

    delete data;

}

bool JpegInterpreter::readSector(char *name, char *input, int length) {
    //таблица квантования
    if ((int) name[0] == -1 && (int) name[1] == -37) {
        dqtDecode(input);
    }
    if ((int) name[0] == -1 && (int) name[1] == -64) {
        sof0Decode(input);
    }
    if ((int) name[0] == -1 && (int) name[1] == -60) {
        dhtDecode(input);
    }
    if ((int) name[0] == -1 && (int) name[1] == -38) {
        //start of scan
        sosDecode(input);
        return false;
    }
    return true;
}

void JpegInterpreter::dqtDecode(char *input) {
    unsigned char *table = new unsigned char[64];
    for (int i = 0; i < 64; i++) {
        table[i] = input[i + 1];
    }
    dqtables.push_back(table);
}

void JpegInterpreter::dhtDecode(char *input) {
    int counter = 0;

    char *table = new char[16];
    for (int i = 0; i < 16; i++) {
        table[i] = input[i + 1];
        counter += (int) input[i + 1];
    }

    HuffmanTable dht;
    dht.content = table;
    dht.size = 16;
    dht.type = ((int) input[0] >> 4 << 4);
    dht.id = ((int) input[0] << 4 >> 4);
    treeNode *rootNode = new treeNode();
    dht.root = rootNode;
    rootNode->length = 1000;
    rootNode->parent = nullptr;
    rootNode->left = nullptr;
    rootNode->right = nullptr;

    for (int i = 0; i < counter; i++) {
        std::cout << (int) input[16 + i + 1] << " ";
    }

    for (int i = 0; i < counter; i++) {
        int level = i;
        int tmp = 0;
        for (int j = 0; j < 16; j++) {
            if (tmp + table[j] > i) {
                level = j + 1;// tmp +table[j];
                break;
            } else {
                tmp += table[j];
            }
        }


        treeNode *newNode = new treeNode();
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->length = input[16 + 1 + i];
        addNode(rootNode, level, 0, newNode);


    }

    preorder(rootNode);
    inorder(rootNode);

    if ((int) ((input[0] & 0b11110000) >> 4) == 0) {
        dhtablesDC.push_back(dht);
    } else {
        dhtablesAC.push_back(dht);
    }

}

void JpegInterpreter::preorder(treeNode *root) {
    if (root == nullptr) return;
    preorder(root->left);
    preorder(root->right);
}

void JpegInterpreter::inorder(treeNode *root) {
    if (root == nullptr) return;
    inorder(root->left);
    inorder(root->right);
}

bool JpegInterpreter::addNode(treeNode *root, int level, int curLevel, treeNode *newNode) {

    if (curLevel == level - 1 && root->left == nullptr) {
        root->left = newNode;
        newNode->parent = root;
        return true;
    }
    if (curLevel == level - 1 && root->right == nullptr) {
        root->right = newNode;
        newNode->parent = root;
        return true;
    }

    if (curLevel == level - 1 && root->left != nullptr && root->right != nullptr) {
        return false;
    }
    if (root->left != nullptr && root->right != nullptr && root->left->length != 1000 && root->right->length != 1000) {
        return false;
    }

    if (curLevel < level - 1) {
        if (root->left != nullptr && root->left->length == 1000) {
            if (addNode(root->left, level, curLevel + 1, newNode)) return true;
        }
        if (root->right != nullptr && root->right->length == 1000) {
            if (addNode(root->right, level, curLevel + 1, newNode)) return true;
        }
    }


    if (curLevel < level - 1 && root->left == nullptr) {
        treeNode *extraNode = new treeNode();
        extraNode->length = 1000;
        extraNode->left = nullptr;
        extraNode->right = nullptr;
        extraNode->parent = root;
        root->left = extraNode;
        if (addNode(root->left, level, curLevel + 1, newNode)) return true;


    } else if (curLevel < level - 1 && root->right == nullptr) {
        treeNode *extraNode = new treeNode();
        extraNode->length = 1000;
        extraNode->left = nullptr;
        extraNode->right = nullptr;
        extraNode->parent = root;
        root->right = extraNode;
        if (addNode(root->right, level, curLevel + 1, newNode)) return true;


    }


    return false;

}

void JpegInterpreter::sof0Decode(char *input) {

    unsigned char uc[4];
    for (int i = 0; i < 4; i++) {
        uc[i] = input[i + 1];
    }
    width = (uc[0] << 8) | uc[1];
    height = (uc[2] << 8) | uc[3];

    chanelCount = (int) input[5];
    ChanelInfo *chi = new ChanelInfo[3];
    chanelInfos.push_back(chi[0]);
    chanelInfos.push_back(chi[1]);
    chanelInfos.push_back(chi[2]);
    for (int i = 0; i < chanelCount; i++) {
        chtableId[i] = input[5 + (i * 3) + 2];
        chanelInfos[i].id = i;
        chanelInfos[i].horiz_thining = ((input[5 + (i * 3) + 2] & 0b11110000) >> 4);
        chanelInfos[i].vertical_thining = ((input[5 + (i * 3) + 2] & 0b00001111));
        chanelInfos[i].kvant_table_id = input[5 + (i * 3) + 3];

    }


}

void JpegInterpreter::sosDecode(char *input) {

    int chanelCount = input[0];

    for (int i = 0; i < chanelCount; i++) {
        chanelInfos[i].haffman_id_dc = ((input[2 + (i * 2)] & 0b11110000) >> 4);
        chanelInfos[i].haffman_id_ac = ((input[2 + (i * 2)] & 0b00001111));
    }

}

void JpegInterpreter::dataDecode(char *input) {

    int numOfCurImageTable = 0;
    int curPos = 0, curTableId = 0, curType = 0;
    char CurChar = 0;
    CurChar = input[curPos];
    curPos++;
    int NumOfBit = 7;

    for (int chanel = 0; chanel < 3; chanel++) {
        curType = 0;

        for (int readTable = 0;

             readTable < chanelInfos[chanel].horiz_thining * chanelInfos[chanel].vertical_thining; readTable++) {

            treeNode *curPointer;

            bool cont = true;
            int curBit = 1;
            int *decodedTable = new int[64];
            for (int i = 0; i < 64; i++) {
                decodedTable[i] = 0;
            }
            int curPositionInTable = 0;
            bool matrix = true;
            int **ary = new int *[8];
            for (int i = 0; i < 8; ++i) {
                ary[i] = new int[8];
            }
            curType = 0;
            while (matrix) {

                if (curType == 0) curPointer = dhtablesDC[chanelInfos[chanel].haffman_id_dc].root;
                if (curType == 1) curPointer = dhtablesAC[chanelInfos[chanel].haffman_id_ac].root;

                while (1) {
                    curBit = getBitfromByte(CurChar, NumOfBit);
                    if (curBit == 1) {
                        curPointer = curPointer->right;
                    } else if (curBit == 0) {
                        curPointer = curPointer->left;
                    }

                    NumOfBit--;
                    if (NumOfBit < 0) {
                        CurChar = input[curPos];
                        NumOfBit = 7;
                        curPos++;
                    }
                    curBit = getBitfromByte(CurChar, NumOfBit);
                    if (curBit == 0 && curPointer->left == nullptr) break;
                    if (curBit == 1 && curPointer->right == nullptr) break;

                }

                int nodeValue = curPointer->length;
                if (curPointer->length == 0 && curType == 1) { matrix = false; }

                int finalValue;
                if (nodeValue == 0) {
                    curPointer->c = 0;
                    decodedTable[curPositionInTable] = 0;
                    curPositionInTable++;
                } else {

                    finalValue = 0;
                    int realLenth = curType ? (curPointer->length & 0b00001111) : curPointer->length;


                    int countZeros = ((curPointer->length & 0b11110000) >> 4);

                    for (int j = 0; j < countZeros; j++) {
                        decodedTable[curPositionInTable] = 0;
                        curPositionInTable++;
                    }

                    int firstBit = curBit;
                    if (realLenth == 0 && curType == 1) { matrix = false; }
                    for (int j = 0; j < realLenth; j++) {
                        NumOfBit--;
                        if (NumOfBit < 0) {
                            CurChar = input[curPos];
                            NumOfBit = 7;
                            curPos++;

                        }
                        finalValue = (finalValue * 2) + (curBit);
                        curBit = getBitfromByte(CurChar, NumOfBit);

                    }

                    if (firstBit) curPointer->c = finalValue;
                    else {
                        int p = 0;
                        finalValue = finalValue - std::pow(2, realLenth) + 1;

                    }


                    if (curType == 0) {

                        decodedTable[curPositionInTable] = finalValue;

                        curPositionInTable++;

                    } else if (curType == 1) {

                        decodedTable[curPositionInTable] = finalValue;

                        curPositionInTable++;


                    }
                }


                if (curType == 0) {
                    curType = 1;
                }

                curBit = getBitfromByte(CurChar, NumOfBit);


            }
            straightArrToZigZagArr(decodedTable, ary);


            if (readTable != 0) {
                ary[0][0] = ary[0][0] + imageTables[numOfCurImageTable - 1][0][0];
            }
            imageTables.push_back(ary);
            numOfCurImageTable++;

        }

    }


}

void JpegInterpreter::kvanting() {
    int curTable = 0;

    for (int chanel = 0; chanel < 3; chanel++) {
        int tablesCount = chanelInfos[chanel].horiz_thining * chanelInfos[chanel].vertical_thining;
        int kvantTableId = chanelInfos[chanel].kvant_table_id;

        int **ary = new int *[8];
        for (int i = 0; i < 8; ++i) {
            ary[i] = new int[8];
        }

        straightArrToZigZagArr(dqtables[kvantTableId], ary);


        for (int readTable = 0; readTable < tablesCount; readTable++) {
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                    imageTables[curTable][a][b] = (int) imageTables[curTable][a][b] * (int) ary[a][b];
                }
            }

            curTable++;
        }

    }
}

void JpegInterpreter::inversiveDiscreteCosineTransformation() {
    int curTable = 0;

    for (int chanel = 0; chanel < 3; chanel++) {
        int tablesCount = chanelInfos[chanel].horiz_thining * chanelInfos[chanel].vertical_thining;

        for (int readTable = 0; readTable < tablesCount; readTable++) {
            int **temp = new int *[8];
            for (int i = 0; i < 8; ++i) {
                temp[i] = new int[8];
            }

            int **coefTable = imageTables[curTable];

            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    double sum = 0;
                    for (int u = 0; u < 8; u++) {
                        for (int v = 0; v < 8; v++) {
                            sum += C(u) * C(v) * coefTable[u][v] * cos((2 * x + 1) * u * 3.14159265358979323846 / 16) *
                                   cos((2 * y + 1) * v * 3.14159265358979323846 / 16);
                        }
                    }
                    temp[x][y] = round(sum / 4);
                }
            }

            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    coefTable[x][y] = temp[x][y];
                }
            }


            for (int i = 0; i < 8; ++i) {
                delete temp[i];
            }

            curTable++;
        }
    }
}

void JpegInterpreter::limitToColorRange() {
    int curTable = 0;

    for (int chanel = 0; chanel < 3; chanel++) {
        int tablesCount = chanelInfos[chanel].horiz_thining * chanelInfos[chanel].vertical_thining;

        for (int readTable = 0; readTable < tablesCount; readTable++) {


            int **coefTable = imageTables[curTable];

            for (int x = 0; x < 8; x++) {
                for (int y = 0; y < 8; y++) {
                    coefTable[x][y] = std::min(std::max(0, coefTable[x][y] + 128), 255);
                }
            }



            curTable++;
        }
    }
}

void JpegInterpreter::finaleImageGenerate() {

    arrBytesToShow = new unsigned char[width * height * 3];
    unsigned char *Y = new unsigned char[width * height * 3];
    unsigned char *Cb = new unsigned char[width * height * 3];
    unsigned char *Cr = new unsigned char[width * height * 3];
    int maxH = 0, maxV = 0;
    if (chanelInfos[0].horiz_thining > maxH) maxH = chanelInfos[0].horiz_thining;
    if (chanelInfos[1].horiz_thining > maxH) maxH = chanelInfos[1].horiz_thining;
    if (chanelInfos[2].horiz_thining > maxH) maxH = chanelInfos[2].horiz_thining;
    if (chanelInfos[0].vertical_thining > maxV) maxV = chanelInfos[0].vertical_thining;
    if (chanelInfos[1].vertical_thining > maxV) maxV = chanelInfos[1].vertical_thining;
    if (chanelInfos[2].vertical_thining > maxV) maxV = chanelInfos[2].vertical_thining;

    int YcoefH = maxH / chanelInfos[0].horiz_thining, CbcoefH = maxH / chanelInfos[1].horiz_thining, CrcoefH =
            maxH / chanelInfos[2].horiz_thining;
    int YcoefV = maxV / chanelInfos[0].vertical_thining, CbcoefV = maxV / chanelInfos[1].vertical_thining, CrcoefV =
            maxV / chanelInfos[2].vertical_thining;



    for (int i = 0; i < chanelInfos[0].horiz_thining; i++) {
        for (int j = 0; j < chanelInfos[0].vertical_thining; j++) {

            int curTable = j * chanelInfos[0].horiz_thining + i;
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                    Y[j * chanelInfos[0].horiz_thining * 64 + i * 8 + a * width + b] = imageTables[curTable][a][b];
                    if (imageTables[curTable][a][b] < 0 || imageTables[curTable][a][b] > 255)
                        std::cout << imageTables[curTable][a][b] << " ";

                }
            }

        }
    }

    for (int i = 0; i < chanelInfos[1].horiz_thining; i++) {
        for (int j = 0; j < chanelInfos[1].vertical_thining; j++) {
            int curTable = j * chanelInfos[1].horiz_thining + i +
                           chanelInfos[0].horiz_thining * chanelInfos[0].vertical_thining;
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                        Cb[j * chanelInfos[1].horiz_thining * 64 + i * 8 + a * width / CbcoefH +
                       b] = imageTables[curTable][a][b];
                    if (imageTables[curTable][a][b] < 0 || imageTables[curTable][a][b] > 255)
                        std::cout << imageTables[curTable][a][b] << " ";

                }
            }
        }
    }

    for (int i = 0; i < chanelInfos[2].horiz_thining; i++) {
        for (int j = 0; j < chanelInfos[2].vertical_thining; j++) {
            int curTable = j * chanelInfos[2].horiz_thining + i +
                           chanelInfos[0].horiz_thining * chanelInfos[0].vertical_thining +
                           +chanelInfos[1].horiz_thining * chanelInfos[1].vertical_thining;
            for (int a = 0; a < 8; a++) {
                for (int b = 0; b < 8; b++) {
                    Cr[j * chanelInfos[2].horiz_thining * 64 + i * 8 + a * width / CrcoefH +
                       b] = imageTables[curTable][a][b];
                    if (imageTables[curTable][a][b] < 0 || imageTables[curTable][a][b] > 255)
                        std::cout << imageTables[curTable][a][b] << " ";

                }
            }
        }
    }


    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {

            arrBytesToShow[(y * width + x) * 3] = Y[((y) / YcoefV * chanelInfos[0].horiz_thining * 8 + (x) / YcoefH)];
            arrBytesToShow[(y * width + x) * 3 + 1] = Cb[(y / CbcoefH * chanelInfos[1].horiz_thining * 8 +
                                                          x / CbcoefV)];
            arrBytesToShow[(y * width + x) * 3 + 2] = Cr[(y / CrcoefH * chanelInfos[2].horiz_thining * 8 +
                                                          x / CrcoefV)];
        }
    }



    delete Y;
    delete Cr;
    delete Cb;

}

int JpegInterpreter::getBitfromByte(char Byte, int numOfBit) {
    char mask = 1;
    return (Byte & (mask << numOfBit)) >> numOfBit;
}

void JpegInterpreter::straightArrToZigZagArr(int *straight, int **zigzag) {

    zigzag[0][0] = straight[0];
    zigzag[0][1] = straight[1];
    zigzag[0][2] = straight[5];
    zigzag[0][3] = straight[6];

    zigzag[0][4] = straight[14];
    zigzag[0][5] = straight[15];
    zigzag[0][6] = straight[27];
    zigzag[0][7] = straight[28];


    zigzag[1][0] = straight[2];
    zigzag[1][1] = straight[4];
    zigzag[1][2] = straight[7];
    zigzag[1][3] = straight[13];

    zigzag[1][4] = straight[16];
    zigzag[1][5] = straight[26];
    zigzag[1][6] = straight[29];
    zigzag[1][7] = straight[42];


    zigzag[2][0] = straight[3];
    zigzag[2][1] = straight[8];
    zigzag[2][2] = straight[12];
    zigzag[2][3] = straight[17];

    zigzag[2][4] = straight[25];
    zigzag[2][5] = straight[30];
    zigzag[2][6] = straight[41];
    zigzag[2][7] = straight[43];


    zigzag[3][0] = straight[9];
    zigzag[3][1] = straight[11];
    zigzag[3][2] = straight[18];
    zigzag[3][3] = straight[24];

    zigzag[3][4] = straight[31];
    zigzag[3][5] = straight[40];
    zigzag[3][6] = straight[44];
    zigzag[3][7] = straight[53];


    zigzag[4][0] = straight[10];
    zigzag[4][1] = straight[19];
    zigzag[4][2] = straight[23];
    zigzag[4][3] = straight[32];

    zigzag[4][4] = straight[39];
    zigzag[4][5] = straight[45];
    zigzag[4][6] = straight[52];
    zigzag[4][7] = straight[54];


    zigzag[5][0] = straight[20];
    zigzag[5][1] = straight[22];
    zigzag[5][2] = straight[33];
    zigzag[5][3] = straight[38];

    zigzag[5][4] = straight[46];
    zigzag[5][5] = straight[51];
    zigzag[5][6] = straight[55];
    zigzag[5][7] = straight[60];


    zigzag[6][0] = straight[21];
    zigzag[6][1] = straight[34];
    zigzag[6][2] = straight[37];
    zigzag[6][3] = straight[47];

    zigzag[6][4] = straight[50];
    zigzag[6][5] = straight[56];
    zigzag[6][6] = straight[59];
    zigzag[6][7] = straight[61];


    zigzag[7][0] = straight[35];
    zigzag[7][1] = straight[36];
    zigzag[7][2] = straight[48];
    zigzag[7][3] = straight[49];

    zigzag[7][4] = straight[57];
    zigzag[7][5] = straight[58];
    zigzag[7][6] = straight[62];
    zigzag[7][7] = straight[63];


}

void JpegInterpreter::straightArrToZigZagArr(unsigned char *straight, int **zigzag) {

    zigzag[0][0] = (int) straight[0];
    zigzag[0][1] = (int) straight[1];
    zigzag[0][2] = (int) straight[5];
    zigzag[0][3] = (int) straight[6];

    zigzag[0][4] = (int) straight[14];
    zigzag[0][5] = (int) straight[15];
    zigzag[0][6] = (int) straight[27];
    zigzag[0][7] = (int) straight[28];


    zigzag[1][0] = (int) straight[2];
    zigzag[1][1] = (int) straight[4];
    zigzag[1][2] = (int) straight[7];
    zigzag[1][3] = (int) straight[13];

    zigzag[1][4] = (int) straight[16];
    zigzag[1][5] = (int) straight[26];
    zigzag[1][6] = (int) straight[29];
    zigzag[1][7] = (int) straight[42];


    zigzag[2][0] = (int) straight[3];
    zigzag[2][1] = (int) straight[8];
    zigzag[2][2] = (int) straight[12];
    zigzag[2][3] = (int) straight[17];

    zigzag[2][4] = (int) straight[25];
    zigzag[2][5] = (int) straight[30];
    zigzag[2][6] = (int) straight[41];
    zigzag[2][7] = (int) straight[43];


    zigzag[3][0] = (int) straight[9];
    zigzag[3][1] = (int) straight[11];
    zigzag[3][2] = (int) straight[18];
    zigzag[3][3] = (int) straight[24];

    zigzag[3][4] = (int) straight[31];
    zigzag[3][5] = (int) straight[40];
    zigzag[3][6] = (int) straight[44];
    zigzag[3][7] = (int) straight[53];


    zigzag[4][0] = (int) straight[10];
    zigzag[4][1] = (int) straight[19];
    zigzag[4][2] = (int) straight[23];
    zigzag[4][3] = (int) straight[32];

    zigzag[4][4] = (int) straight[39];
    zigzag[4][5] = (int) straight[45];
    zigzag[4][6] = (int) straight[52];
    zigzag[4][7] = (int) straight[54];


    zigzag[5][0] = (int) straight[20];
    zigzag[5][1] = (int) straight[22];
    zigzag[5][2] = (int) straight[33];
    zigzag[5][3] = (int) straight[38];

    zigzag[5][4] = (int) straight[46];
    zigzag[5][5] = (int) straight[51];
    zigzag[5][6] = (int) straight[55];
    zigzag[5][7] = (int) straight[60];


    zigzag[6][0] = (int) straight[21];
    zigzag[6][1] = (int) straight[34];
    zigzag[6][2] = (int) straight[37];
    zigzag[6][3] = (int) straight[47];

    zigzag[6][4] = (int) straight[50];
    zigzag[6][5] = (int) straight[56];
    zigzag[6][6] = (int) straight[59];
    zigzag[6][7] = (int) straight[61];


    zigzag[7][0] = (int) straight[35];
    zigzag[7][1] = (int) straight[36];
    zigzag[7][2] = (int) straight[48];
    zigzag[7][3] = (int) straight[49];

    zigzag[7][4] = (int) straight[57];
    zigzag[7][5] = (int) straight[58];
    zigzag[7][6] = (int) straight[62];
    zigzag[7][7] = (int) straight[63];

}


double JpegInterpreter::C(int u) {
    if (u == 0) {
        return 1 / sqrt(2);
    } else {
        return 1;
    }
}