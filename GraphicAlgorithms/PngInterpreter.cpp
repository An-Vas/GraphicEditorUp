
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <cmath>
#include <cstring>
#include "libdeflate.h"
#include "PngInterpreter.h"
#include "BasicAlgorithms.h"
#include "GammaChanger.h"

PngInterpreter::PngInterpreter(std::ifstream &F) : F(F) {

}

void PngInterpreter::readPng() {
    int counter = 0;

    while (true) {
        int length = BasicAlgorithms::readNum4(F);
        std::cout << "  CHUNK. len:" << length << " ";
        counter++;
        char *name = new char[4];
        F.read(name, 4);
        std::cout << "name: " << name[0] << name[1] << name[2] << name[3] << " \n";
        char *data = new char[length];
        F.read(data, length);
        bool cont = readChunk(name, data, length);
        char *crc = new char[4];
        F.read(crc, 4);

        char * temp = new char [length+4];
        for (int g = 0; g < 4; g++)temp[g]=name[g];
        for (int g = 4; g < length + 4; g++)temp[g]=data[g-4];
        crcCheck(temp, crc, length+4);

        if (!cont)
            break;



        delete[] name;
        delete[] data;
        delete[] crc;

    }

}

bool PngInterpreter::readChunk(char *name, char *input, int length) {
    if (is(name[0], 'i') && is(name[1], 'h') && is(name[2], 'd') && is(name[3], 'r')) {
        ihdrDecode(input);
    }
    else if (is(name[0], 'i') && is(name[1], 'd') && is(name[2], 'a') && is(name[3], 't')) {
        idatDecode(input, length);
    }
    else if (is(name[0], 'p') && is(name[1], 'l') && is(name[2], 't') && is(name[3], 'e')) {
        plteDecode(input, length);
    }
    else if (is(name[0], 'g') && is(name[1], 'a') && is(name[2], 'm') && is(name[3], 'a')) {
        gamaDecode(input);
    }
    else if (is(name[0], 'i') && is(name[1], 'e') && is(name[2], 'n') && is(name[3], 'd')) {
        deflateDecodeAll();
        return false;
    } else if (name[0]<='Z' && name[0]>='A'){
        std::cout<<"UNDERFINED CRIRICAL CHUNK CAN'T BE DECODED\n";
    }

    return true;
}

void PngInterpreter::ihdrDecode(char *input) {
    width = BasicAlgorithms::readNum4fromChar(BasicAlgorithms::get4Bytes(reinterpret_cast<unsigned char *>(input), 0));
    height = BasicAlgorithms::readNum4fromChar(BasicAlgorithms::get4Bytes(reinterpret_cast<unsigned char *>(input), 4));
    bitDepth = BasicAlgorithms::readNum1fromChar(input[8]);
    colorType = BasicAlgorithms::readNum1fromChar(input[9]);
    compressionType = BasicAlgorithms::readNum1fromChar(input[10]);
    filterType = BasicAlgorithms::readNum1fromChar(input[11]);
    interlaceType = BasicAlgorithms::readNum1fromChar(input[12]);


    if (width < 0 || height < 0 || !(colorType == 0 || colorType == 2 || colorType == 3) ||
        (bitDepth != 8 && (colorType == 2 || colorType == 3)) || compressionType != 0 || filterType != 0) {
        // TODO  ошибка входного файла
    }

    arrBytesToShow = new unsigned char[width * height * 3];
    arrBytesFromFile = new char[width * height * 3];

}

void PngInterpreter::gamaDecode(char *input){
    unsigned char uc[4];
    for (int i = 0; i < 4; i++){
        uc[i] = input[i];
    }
    int temp =  (uc[0] << 24) | (uc[1] << 16) | (uc[2] << 8) | uc[3];;
    pngGamma = (float)temp/100000;

}

void PngInterpreter::idatDecode(char *input, int length) {


    unsigned char *temp = new unsigned char[length];

    for (int i = 0; i < length; i++) {

        temp[i] = input[i] + 127;
    }
    for (int i = 0; i < length; i++) {
        arrBytesFromFile[i + positionArrBytes] = input[i];
    }

    positionArrBytes += length;
    delete temp;
}

void PngInterpreter::plteDecode(char *input, int length) {


    pltch1 = new unsigned char[length / 3];
    pltch2 = new unsigned char[length / 3];
    pltch3 = new unsigned char[length / 3];

    for (int color = 0; color < length / 3; color++) {
        pltch1[color] = input[color * 3];
        pltch2[color] = input[color * 3 + 1];
        pltch3[color] = input[color * 3 + 2];
    }
}



void PngInterpreter::deflateDecodeAll() {

    char *output = new char[width * height * 3 * 10];
    libdeflate_decompressor *decompressor = libdeflate_alloc_decompressor();
    size_t actual_out_size;
    int res = libdeflate_zlib_decompress(decompressor, arrBytesFromFile, positionArrBytes, output,
                                         width * height * 3 * 10, &actual_out_size);

    if (colorType == 2){
        defilterAllType2(output, actual_out_size);
    } else if (colorType == 0) {
        defilterAllType0(output, actual_out_size);

    } else if (colorType == 3){
        defilterAllType0(output, actual_out_size);
        for (int j = 0; j < width * height * 3; j+=3){
            arrBytesToShow[j] = pltch1[arrBytesToShow[j]];
            arrBytesToShow[j+1] = pltch2[arrBytesToShow[j+1]];
            arrBytesToShow[j+2] = pltch3[arrBytesToShow[j+2]];
        }
    }

    delete[] output;
    libdeflate_free_decompressor(decompressor);
}

void PngInterpreter::defilterAllType2(char *output, int outputLenth) {
    //i по массиву с фильтрами, j по результату
    for (int i = 0, j = 0; j < width * height * 3; i++, j++) {
        if (j % (width * 3) == 0) {
            filterType = (int) output[i];
            i++;
        }
        int res;
        switch (filterType) {
            case 0:
                res = output[i];
                break;
            case 1:
                if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) res = output[i];
                else res = output[i] + arrBytesToShow[j - 3];

                break;
            case 2:
                if (j - (width * 3) < 0 + j % (width * 3)) res = output[i];
                else res = output[i] + arrBytesToShow[j - width * 3];
                break;
            case 3:
                if (((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) && (j - (width * 3) < 0)) res = output[i];
                else if (j - (width * 3) < 0) res = output[i] + floor(arrBytesToShow[j - 3] / 2);
                else if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) res = output[i] + floor(arrBytesToShow[j - width * 3] / 2);
                else res = output[i] + (floor((arrBytesToShow[j - width * 3] + arrBytesToShow[j - 3]) / 2));

                break;
            case 4:
                int a, b, c, p, pathPredicator;
                if (((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) && (j - (width * 3) < 0 + j % (width * 3) )) {
                    a = 0;
                    b = 0;
                    c = 0;
                }
                else
                if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) {
                    a = 0;
                    c = 0;
                    b = arrBytesToShow[j - (width * 3)];
                }
                else if (j - (width * 3) < 0 + j % (width * 3) ) {
                    b = 0;
                    c = 0;
                    a = arrBytesToShow[j - 3];
                } else {
                    a = arrBytesToShow[j - 3];
                    b = arrBytesToShow[j - (width * 3)];
                    c = arrBytesToShow[j - (width * 3) - 3];
                }


                p = a + b - c;


                int pa = abs(p - a);
                int pb = abs(p - b);
                int pc = abs(p - c);

                if (pa <= pb && pa <= pc) pathPredicator = a;
                else if (pb <= pc)  pathPredicator = b;
                else pathPredicator = c;

                res = output[i] + pathPredicator;

                break;


        }
        if (res > 255) res %= 256;

        arrBytesToShow[j] = res;
    }

}
void PngInterpreter::defilterAllType0(char *output, int outputLenth) {
    //i по массиву с фильтрами, j по результату
    for (int i = 0, j = 0; j < width * height * 3; i++, j+=3){
        if (j % (width * 3) == 0) {
            filterType = (int) output[i];
            i++;
        }
        int res ;
        switch (filterType) {
            case 0:
                res = output[i];
                break;
            case 1:
                if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) res = output[i];
                else res = output[i] + arrBytesToShow[j - 3];

                break;
            case 2:
                if (j - (width * 3) < 0 + j % (width * 3)) res = output[i];
                else res = output[i] + arrBytesToShow[j - width * 3];
                break;
            case 3:
                if (((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) && (j - (width * 3) < 0)) res = output[i];
                else if (j - (width * 3) < 0) res = output[i] + floor(arrBytesToShow[j - 3] / 2);
                else if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) res = output[i] + floor(arrBytesToShow[j - width * 3] / 2);
                else res = output[i] + (floor((arrBytesToShow[j - width * 3] + arrBytesToShow[j - 3]) / 2));

                break;
            case 4:
                int a, b, c, p, pathPredicator;
                if (((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) && (j - (width * 3) < 0 + j % (width * 3) )) {
                    a = 0;
                    b = 0;
                    c = 0;
                }
                else
                if ((j % (width * 3) == 0 || j % (width * 3) == 1 || j % (width * 3) == 2)) {
                    a = 0;
                    c = 0;
                    b = arrBytesToShow[j - (width * 3)];
                }
                else if (j - (width * 3) < 0 + j % (width * 3) ) {
                    b = 0;
                    c = 0;
                    a = arrBytesToShow[j - 3];
                } else {
                    a = arrBytesToShow[j - 3];
                    b = arrBytesToShow[j - (width * 3)];
                    c = arrBytesToShow[j - (width * 3) - 3];
                }


                p = a + b - c;


                int pa = abs(p - a);
                int pb = abs(p - b);
                int pc = abs(p - c);

                if (pa <= pb && pa <= pc) pathPredicator = a;
                else if (pb <= pc)  pathPredicator = b;
                else pathPredicator = c;

                res = output[i] + pathPredicator;

                break;


        }
        if (res > 255) res %= 256;

        arrBytesToShow[j] = res;
        arrBytesToShow[j + 1] = res;
        arrBytesToShow[j + 2] = res;

    }




}

void PngInterpreter::crcCheck(char* input, char* chunkCrc, int inputLength){

    unsigned char  * temp = new unsigned char [inputLength];
    unsigned char  * tempcrc = new unsigned char [inputLength];
    for (int g = 0; g < inputLength; g++)temp[g]=input[g];
    for (int g = 0; g < 4; g++)tempcrc[g]=chunkCrc[g];

    unsigned long res =  update_crc(0xffffffffL, temp, inputLength) ^ 0xffffffffL;
    unsigned char bytes[4];

    bytes[3] = (res >> 0) & 0xFF;
    bytes[2] = (res >> 8) & 0xFF;
    bytes[1] = (res >> 16) & 0xFF;
    bytes[0] = (res >> 24) & 0xFF;

    if ((bytes[0] - chunkCrc[0])%256==0 && (bytes[1] - chunkCrc[1])%256==0 && (bytes[2] - chunkCrc[2])%256==0 && (bytes[3] - chunkCrc[3])%256==0 ){
        std::cout << "correct\n";
    } else  std::cout << "incorrect " <<(int)bytes[0] << " " <<(int)tempcrc[0] << " "<<(int)bytes[1] << " " <<(int)tempcrc[1] << " "<<(int)bytes[2] << " " <<(int)tempcrc[2] << " "<<(int)bytes[3] << " " <<(int)tempcrc[3] << " " << "\n";

}

char PngInterpreter::toLowercase(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }

    return c;
}

bool PngInterpreter::is(char c1, char c2) {
    return toLowercase(c1) == toLowercase(c2);
}

unsigned long PngInterpreter::update_crc(unsigned long crc, unsigned char *buf,
                                         int len){
    unsigned long crc_table[256];
    unsigned long c;
    int n, k;

    for (n = 0; n < 256; n++) {
        c = (unsigned long) n;
        for (k = 0; k < 8; k++) {
            if (c & 1)
                c = 0xedb88320L ^ (c >> 1);
            else
                c = c >> 1;
        }
        crc_table[n] = c;
    }
    c = crc;

    for (n = 0; n < len; n++) {
        c = crc_table[(c ^ buf[n]) & 0xff] ^ (c >> 8);
    }
    return c;
}

int PngInterpreter::libdeflateCode(unsigned char * input, unsigned char *  output, int inputLenth){
    libdeflate_compressor *compressor = libdeflate_alloc_compressor(6);

    size_t max_compressed_size = libdeflate_zlib_compress_bound(compressor, inputLenth);

    size_t compressed_size = libdeflate_zlib_compress(compressor, input, inputLenth, output, max_compressed_size);

    libdeflate_free_compressor(compressor);
    return compressed_size;
}


