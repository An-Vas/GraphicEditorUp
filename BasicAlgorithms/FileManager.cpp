

#include "FileManager.h"
#include "../GraphicAlgorithms/PngInterpreter.h"
#include "../GraphicAlgorithms/JpegInterpreter.h"
#include "../GraphicAlgorithms/ColorSpacesInterpreter.h"
#include <windows.h>
#include <Commdlg.h>
#include <iostream>
#include <iomanip>

std::ifstream
FileManager::chooseAndOpenFile(unsigned char **arrBytesP, int *width, int *height, int *extraNum, int chanelMode,
                               ColorSpace colorSpace, float *curGamma) {
    unsigned char *arrBytes;
    std::ifstream F;
    CHAR FilePath[MAX_PATH] = "";
    OPENFILENAME ofn = {sizeof ofn};
    wchar_t file[1024];
    file[0] = '\0';
    ofn.lpstrFile = reinterpret_cast<LPSTR>(file);
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFile = FilePath;
    ofn.Flags = OFN_ALLOWMULTISELECT | OFN_EXPLORER;

    if (GetOpenFileName(&ofn)) {
        char buffer[200];
        if (F.is_open()) F.close();

        F.open(FilePath, std::ios::binary);
        F.read(buffer, 3);

        char c[3];


        if (buffer[0] == 'P' && (buffer[1] == '5' || buffer[1] == '6')) {

            *width = readNum(F);
            *height = readNum(F);
            *extraNum = readNum(F);

            arrBytes = new unsigned char[*width * *height * 3];

            for (int y = 0, curPos = 0; y < *height; y++) {
                for (int x = 0; x < *width; x++) {

                    if (buffer[1] == '5') {
                        F.read(c, 1);
                        c[1] = c[0];
                        c[2] = c[0];
                    } else {
                        F.read(c, 3);
                    }

                    unsigned char uc[3];

                    uc[0] = (unsigned char) c[0];
                    uc[1] = (unsigned char) c[1];
                    uc[2] = (unsigned char) c[2];

                    arrBytes[curPos] = uc[0];
                    arrBytes[curPos + 1] = uc[1];
                    arrBytes[curPos + 2] = uc[2];

                    curPos += 3;

                }
            }
            if (colorSpace.value != ColorSpace::Value::RGB) {
                arrBytes = ColorSpacesInterpreter::cast(colorSpace, ColorSpace::Value::RGB,
                                                        arrBytes, *width, *height);
            }
            *curGamma = 1.0;

        } else if ((int) buffer[1] == 80 && buffer[2] == 78) {

            F.read(buffer, 5);
            PngInterpreter pngInterpreter = PngInterpreter(F);
            pngInterpreter.readPng();
            *width = pngInterpreter.width;
            *height = pngInterpreter.height;

            arrBytes = pngInterpreter.arrBytesToShow;
            *curGamma = pngInterpreter.pngGamma;


        } else if ((int) buffer[0] == -1 && (int) buffer[1] == -40) {
            F.seekg(-1, std::ios_base::cur);

            JpegInterpreter jpegInterpreter = JpegInterpreter(F);
            jpegInterpreter.readJpeg();

            *width = jpegInterpreter.width;
            *height = jpegInterpreter.height;

            arrBytes = new unsigned char[*width * *height * 3];
            arrBytes = jpegInterpreter.arrBytesToShow;

            *curGamma = 1.0;
        }
    }
    *arrBytesP = arrBytes;
    return F;
}

void FileManager::savePnmFile(unsigned char *arrBytes, int width, int height, int extraNum, int chanelMode,
                              ColorSpace colorSpace) {
    if (width == 0 || height == 0 || arrBytes == 0) {
        return;

    } else {

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::string fileName;
        std::ostringstream oss;

        oss << std::put_time(&tm, "GraphicsEditor_%d-%m-%Y--%H-%M-%S");
        auto str = oss.str();

        if (chanelMode == 0) {
            fileName = str + "_" + colorSpace.toString() + ".ppm";
        } else {
            ColorSpace tmpColorSpace = ColorSpace(ColorSpace::Value::RGB);
            fileName =
                    str + "_" + colorSpace.toString() + "-" + std::to_string(chanelMode) + "-as-" +
                    tmpColorSpace.toString() + ".ppm";
        }


        std::ofstream output(fileName, std::ios::binary);

        if (output.is_open()) {
            output << "P6\r" << width << " " << height << "\r" << extraNum << "\r";

            if (chanelMode == 0) {
                for (int i = 0; i < width * height * 3; i++) {
                    output << arrBytes[i];
                }

            } else {
                for (int i = 0; i < width * height * 3; i++) {
                    output << arrBytes[i];
                }
            }


            output.flush();
        } else {

        }
        output.close();
    }
}

void FileManager::savePngFile(unsigned char *arrBytes, int width, int extraNum, int height, int chanelMode,
                              ColorSpace colorSpace, float curGamma) {
    if (width == 0 || height == 0 || arrBytes == 0) {


    } else {

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::string fileName;
        std::ostringstream oss;

        oss << std::put_time(&tm, "GraphicsEditor_%d-%m-%Y--%H-%M-%S");
        auto str = oss.str();


        fileName = str + ".png";

        std::ofstream output(fileName, std::ios::binary);

        if (output.is_open()) {
            output << (unsigned char) 137 << (unsigned char) 80 << (unsigned char) 78
                   << (unsigned char) 71 << (unsigned char) 13 << (unsigned char) 10
                   << (unsigned char) 26 << (unsigned char) 10;

            unsigned char *finalihdrdata = new unsigned char[17];
            finalihdrdata[0] = 'I';
            finalihdrdata[1] = 'H';
            finalihdrdata[2] = 'D';
            finalihdrdata[3] = 'R';
            int num = 13;
            unsigned char bytes[4];
            bytes[0] = (num >> 0) & 0xFF;
            bytes[1] = (num >> 8) & 0xFF;
            bytes[2] = (num >> 16) & 0xFF;
            bytes[3] = (num >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];

            output << 'I' << 'H' << 'D' << 'R';
            num = width;
            bytes[0] = (num >> 0) & 0xFF;
            bytes[1] = (num >> 8) & 0xFF;
            bytes[2] = (num >> 16) & 0xFF;
            bytes[3] = (num >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];
            finalihdrdata[0 + 4] = bytes[3];
            finalihdrdata[1 + 4] = bytes[2];
            finalihdrdata[2 + 4] = bytes[1];
            finalihdrdata[3 + 4] = bytes[0];
            num = height;
            bytes[0] = (num >> 0) & 0xFF;
            bytes[1] = (num >> 8) & 0xFF;
            bytes[2] = (num >> 16) & 0xFF;
            bytes[3] = (num >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];
            finalihdrdata[4 + 4] = bytes[3];
            finalihdrdata[5 + 4] = bytes[2];
            finalihdrdata[6 + 4] = bytes[1];
            finalihdrdata[7 + 4] = bytes[0];
            num = 8; //bitDeep
            bytes[0] = (num >> 0) & 0xFF;
            output << bytes[0];
            finalihdrdata[8 + 4] = bytes[0];
            num = 2; //colorType
            bytes[0] = (num >> 0) & 0xFF;
            output << bytes[0];
            finalihdrdata[9 + 4] = bytes[0];
            num = 0; //lastparameteres
            bytes[0] = (num >> 0) & 0xFF;
            output << bytes[0];
            output << bytes[0];
            output << bytes[0];
            finalihdrdata[10 + 4] = bytes[0];
            finalihdrdata[11 + 4] = bytes[0];
            finalihdrdata[12 + 4] = bytes[0];
            unsigned long res =
                    PngInterpreter::update_crc(0xffffffffL, finalihdrdata, 13 + 4) ^ 0xffffffffL;
            bytes[0] = (res >> 0) & 0xFF;
            bytes[1] = (res >> 8) & 0xFF;
            bytes[2] = (res >> 16) & 0xFF;
            bytes[3] = (res >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];


            unsigned char *finalihdrdata1 = new unsigned char[17];
            bytes[0] = (4 >> 0) & 0xFF;
            bytes[1] = (4 >> 8) & 0xFF;
            bytes[2] = (4 >> 16) & 0xFF;
            bytes[3] = (4 >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];
            output << 'g' << 'A' << 'M' << 'A';
            num = curGamma * 100000;
            bytes[0] = (num >> 0) & 0xFF;
            bytes[1] = (num >> 8) & 0xFF;
            bytes[2] = (num >> 16) & 0xFF;
            bytes[3] = (num >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];

            finalihdrdata1[0] = 'g';
            finalihdrdata1[1] = 'A';
            finalihdrdata1[2] = 'M';
            finalihdrdata1[3] = 'A';
            finalihdrdata1[0 + 4] = bytes[3];
            finalihdrdata1[1 + 4] = bytes[2];
            finalihdrdata1[2 + 4] = bytes[1];
            finalihdrdata1[3 + 4] = bytes[0];

            res =
                    PngInterpreter::update_crc(0xffffffffL, finalihdrdata1, 4 + 4) ^ 0xffffffffL;
            bytes[0] = (res >> 0) & 0xFF;
            bytes[1] = (res >> 8) & 0xFF;
            bytes[2] = (res >> 16) & 0xFF;
            bytes[3] = (res >> 24) & 0xFF;
            output << bytes[3] << bytes[2] << bytes[1] << bytes[0];


            unsigned char *output1 = new unsigned char[width * height * 3 + height];
            unsigned char *arrBytesRgbWithFilters = new unsigned char[width * height * 3 + height];


            for (int g = 0, h = 0; g < width * height * 3; g++, h++) {
                if (g % (width * 3) == 0) {
                    arrBytesRgbWithFilters[h] = 0;
                    h++;
                }
                arrBytesRgbWithFilters[h] = arrBytes[g];
            }
            int compresedSize = PngInterpreter::libdeflateCode(arrBytesRgbWithFilters, output1,
                                                               width * height * 3 + height);

            int curCount = 0, curPos = 0, curSize = 0;
            while (true) {


                if (curCount + 8192 < compresedSize) {
                    curCount += 8192;
                } else {
                    curCount += compresedSize - curCount;
                }
                curSize = curCount - curPos;

                if (curSize == 0) break;

                bytes[0] = (curSize >> 0) & 0xFF;
                bytes[1] = (curSize >> 8) & 0xFF;
                bytes[2] = (curSize >> 16) & 0xFF;
                bytes[3] = (curSize >> 24) & 0xFF;
                output << bytes[3] << bytes[2] << bytes[1] << bytes[0];
                output << 'I' << 'D' << 'A' << 'T';

                for (int i = curPos; i < curPos + curSize; i++) {
                    output << output1[i];
                }


                unsigned char *tempForIDAT = new unsigned char[curSize + 4];
                tempForIDAT[0] = 'I';
                tempForIDAT[1] = 'D';
                tempForIDAT[2] = 'A';
                tempForIDAT[3] = 'T';

                for (int i = curPos; i < curPos + curSize; i++) {
                    tempForIDAT[i + 4 - curPos] = output1[i];
                }
                res =
                        PngInterpreter::update_crc(0xffffffffL, tempForIDAT, curSize + 4) ^ 0xffffffffL;
                bytes[0] = (res >> 0) & 0xFF;
                bytes[1] = (res >> 8) & 0xFF;
                bytes[2] = (res >> 16) & 0xFF;
                bytes[3] = (res >> 24) & 0xFF;
                output << bytes[3] << bytes[2] << bytes[1] << bytes[0];


                curPos = curCount;

            }


            output << (unsigned char) 0 << (unsigned char) 0 << (unsigned char) 0 << (unsigned char) 0 << 'I' << 'E'
                   << 'N' << 'D' << (unsigned char) 174 << (unsigned char) 66 << (unsigned char) 96
                   << (unsigned char) 130 << '\0';


            output.flush();

            delete arrBytesRgbWithFilters;
            delete output1;
            delete finalihdrdata1;
            delete finalihdrdata;
        } else {

        }
        output.close();
    }
}

int FileManager::readNum(std::ifstream &F) {
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

        }
        num = (num * 10) + curNum;

    }
    return num;
};