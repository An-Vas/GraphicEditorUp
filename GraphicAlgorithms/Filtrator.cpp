
#include <iostream>
#include <vector>
#include <algorithm>
#include <valarray>
#include "Filtrator.h"


unsigned char *Filtrator::threshold2classesFilter(unsigned char *arrBytes, int width, int height, int threshold) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {
                unsigned char c = arrBytes[curPos(x, y, width) + i];
                if (c > threshold) {
                    arrBytes[curPos(x, y, width) + i] = 255;
                } else {
                    arrBytes[curPos(x, y, width) + i] = 0;
                }

            }
        }
    }
    return arrBytes;
}

unsigned char *
Filtrator::threshold3classesFilter(unsigned char *arrBytes, int width, int height, int threshold1, int threshold2) {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {
                unsigned char c = arrBytes[curPos(x, y, width) + i];
                if (c > threshold2) {
                    arrBytes[curPos(x, y, width) + i] = 255;
                } else if (c < threshold1) {
                    arrBytes[curPos(x, y, width) + i] = 0;
                } else {
                    arrBytes[curPos(x, y, width) + i] = (threshold1 + threshold2) / 2;
                }

            }
        }
    }
    return arrBytes;
}

unsigned char *
Filtrator::thresholdOtsu2classesFilter(unsigned char *arrBytes, int width, int height) {

    int *chanel1count = new int[256]{0};
    int *chanel2count = new int[256]{0};
    int *chanel3count = new int[256]{0};

    for (int i = 0; i < height * width * 3; i += 3) {
        int r = arrBytes[i], g = arrBytes[i + 1], b = arrBytes[i + 2];
        chanel1count[r]++;
        chanel2count[g]++;
        chanel3count[b]++;
    }

    float resSigma = 0;
    int resT = 0;

    for (int t = 0; t < 256; t++){
        float w1 = 0, w2 = 0;
        float  sum = 0;
        for (int i = 0; i < t; i++){
            w1 = w1 + chanel1count[i] + chanel2count[i] + chanel3count[i];
            sum = sum + chanel1count[i] + chanel2count[i] + chanel3count[i];

        }
        for (int i = t; i < 256; i++){
            w2 = w2 + chanel1count[i] + chanel2count[i] + chanel3count[i];
            sum = sum + chanel1count[i] + chanel2count[i] + chanel3count[i];

        }

        w1 /= sum;
        w2 /= sum;
        float sigma1 = 0, sigma2 = 0, sigma = 0;
        for (int i = 0; i < t; i++){
            sigma1 += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
        }
        sigma1/=w1;
        for (int i = t; i < 256; i++){
            sigma2 += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
        }
        sigma2/=w2;
        for (int i = 0; i < 256; i++){
            sigma += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
        }
        float curSigma = w1 * (sigma1  - sigma) * (sigma1  - sigma) + w2 * (sigma2 - sigma) * (sigma2 - sigma);
        if (curSigma > resSigma) {
            resSigma = curSigma;
            resT = t;
        }
    }
    return threshold2classesFilter(arrBytes, width, height, resT);

}


unsigned char *
Filtrator::thresholdOtsu3classesFilter(unsigned char *arrBytes, int width, int height) {

    int *chanel1count = new int[256]{0};
    int *chanel2count = new int[256]{0};
    int *chanel3count = new int[256]{0};

    for (int i = 0; i < height * width * 3; i += 3) {
        int r = arrBytes[i], g = arrBytes[i + 1], b = arrBytes[i + 2];
        chanel1count[r]++;
        chanel2count[g]++;
        chanel3count[b]++;
    }

    float resSigma = 0;
    int resT1 = 0;
    int resT2 = 0;

    for (int t1 = 0; t1 < 256; t1++){
        for (int t2 = t1 + 1; t2 < 256; t2++){
            float w1 = 0, w2 = 0, w3 = 0;
            float  sum = 0;

            for (int i = 0; i < t1; i++){
                w1 = w1 + chanel1count[i] + chanel2count[i] + chanel3count[i];
                sum = sum + chanel1count[i] + chanel2count[i] + chanel3count[i];
            }
            for (int i = t1; i < t2; i++){
                w2 = w2 + chanel1count[i] + chanel2count[i] + chanel3count[i];
                sum = sum + chanel1count[i] + chanel2count[i] + chanel3count[i];
            }
            for (int i = t2; i < 256; i++){
                w3 = w3 + chanel1count[i] + chanel2count[i] + chanel3count[i];
                sum = sum + chanel1count[i] + chanel2count[i] + chanel3count[i];

            }

            w1 /= sum;
            w2 /= sum;
            w3 /= sum;

            float sigma1 = 0, sigma2 = 0, sigma3 = 0, sigma = 0;
            for (int i = 0; i < t1; i++){
                sigma1 += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
            }
            sigma1/=w1;
            for (int i = t1; i < t2; i++){
                sigma2 += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
            }
            sigma2/=w2;
            for (int i = t2; i < 256; i++){
                sigma3 += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
            }
            sigma3/=w3;
            for (int i = 0; i < 256; i++){
                sigma += i * ((float)(chanel1count[i] + chanel2count[i] + chanel3count[i])/sum);
            }
            float curSigma = w1 * (sigma1  - sigma) * (sigma1  - sigma) + w2 * (sigma2 - sigma) * (sigma2 - sigma) + w3 * (sigma3 - sigma) * (sigma3 - sigma);
            if (curSigma > resSigma) {
                resSigma = curSigma;
                resT1 = t1;
                resT2 = t2;
            }
        }
    }
    return threshold3classesFilter(arrBytes, width, height, resT1, resT2);

}
unsigned char * Filtrator::medianFilter(unsigned char * arrBytes, int width, int height, int r){
    unsigned char * resArrBytes = new unsigned char [width*height*3];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {

                std::vector<int> vec;


                for (int a = x - r; a <= x + r; a++){
                    for (int b = y - r; b <= y + r; b++){
                        vec.push_back(arrBytes[curPosExtra(a, b, width, height) + i]);
                    }
                }
                std::sort(vec.begin(), vec.end());

                resArrBytes[curPos(x, y, width) + i] = vec[vec.size()/2];
            }
        }
    }

    delete arrBytes;
    return resArrBytes;
}

unsigned char * Filtrator::gaussFilter(unsigned char * arrBytes, int width, int height, float s){
    int r = s * 3;
    if (r==0) r = 1;
    unsigned char * resArrBytes = new unsigned char [width*height*3];
    std::vector<std::vector<double>> matrix (r*2+1, std::vector<double>(r*2+1, 0));

    for (int a = 0; a <=  r; a++){
        for (int b = 0; b <= r; b++){
            double resGauss = (double)((double)(1.0)/(double)((double)2*(double)3.14*s*s))*std::exp((-(double)(a*a+b*b)/(double)((double)2.0*s*s)));
            matrix[r + a][r + b]= resGauss;
            matrix[r - a][r - b]= resGauss;
            matrix[r - a][r + b]= resGauss;
            matrix[r + a][r - b]= resGauss;
        }
    }


    float sum = 0;
    for (int a = 0; a < r*2+1; a++){
        for (int b = 0; b < r*2+1; b++){
            sum+=matrix[a][b];
        }
    }



    for (int a = 0; a < r*2+1; a++){
        for (int b = 0; b < r*2+1; b++){
            matrix[a][b]/=sum;
        }
    }


    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {
                float pixelSum = 0;
                std::vector<std::vector<int>> vec (r*2+1, std::vector<int>(r*2+1, 0));
                for (int mx = 0; mx < r*2+1; mx++){
                    for (int my = 0; my < r*2+1; my++){
                        pixelSum += arrBytes[curPosExtra((x-r+mx), (y-r+my), width, height)+i] * matrix[mx][my];
                    }
                }

                resArrBytes[curPos(x, y, width) + i] = pixelSum;
            }
        }
    }

    delete arrBytes;
    return resArrBytes;
}

unsigned char * Filtrator::boxBlurFilter(unsigned char * arrBytes, int width, int height, float r){

    unsigned char * resArrBytes = new unsigned char [width*height*3];
    std::vector<std::vector<double>> matrix (r*2+1, std::vector<double>(r*2+1, 0));

    for (int a = 0; a <=  r; a++){
        for (int b = 0; b <= r; b++){
            double res = 1;
            matrix[r + a][r + b]= res;
            matrix[r - a][r - b]= res;
            matrix[r - a][r + b]= res;
            matrix[r + a][r - b]= res;
        }
    }


    float sum = 0;
    for (int a = 0; a < r*2+1; a++){
        for (int b = 0; b < r*2+1; b++){
            sum+=matrix[a][b];
        }
    }



    for (int a = 0; a < r*2+1; a++){
        for (int b = 0; b < r*2+1; b++){
            matrix[a][b]/=sum;
        }
    }


    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {
                float pixelSum = 0;
                std::vector<std::vector<int>> vec (r*2+1, std::vector<int>(r*2+1, 0));
                for (int mx = 0; mx < r*2+1; mx++){
                    for (int my = 0; my < r*2+1; my++){
                        pixelSum += arrBytes[curPosExtra((x-r+mx), (y-r+my), width, height)+i] * matrix[mx][my];
                    }
                }

                resArrBytes[curPos(x, y, width) + i] = pixelSum;
            }
        }
    }

    delete arrBytes;
    return resArrBytes;
}

unsigned char * Filtrator::unsharpedFilter(unsigned char * arrBytes, int width, int height, float amount, float s, int threshold){
    unsigned char * gaussArrBytes = new unsigned char [width*height*3];
    char * diff = new char [width*height*3];
    for (int i = 0; i < width*height*3; i++){
        gaussArrBytes[i] = arrBytes[i];
    }
    gaussArrBytes = Filtrator::gaussFilter(gaussArrBytes, width, height, s);
    for (int i = 0; i < width*height*3; i++){
        diff[i] = gaussArrBytes[i]-arrBytes[i];
    }

    for (int i = 0; i < width*height*3; i++){

        if (diff[i]>threshold){
            int newPixel = arrBytes[i] - arrBytes[i] * amount * ((float)diff[i]/255);
            if (newPixel < 0) newPixel = 0;
            if (newPixel> 255) newPixel = 255;
            arrBytes[i] = newPixel;

        }
    }
    delete gaussArrBytes;
    return arrBytes;

}

unsigned char * Filtrator::sharpedFilter(unsigned char * arrBytes, int width, int height, float sharpness){

    unsigned char * resArrBytes = new unsigned char [width*height*3];

    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {

                std::vector<std::vector<int>> filter (3, std::vector<int>(3, 0));
                int a, b, c, d, e, min, max;
                a = arrBytes[curPosExtra(x, y-1, width, height) + i];
                b = arrBytes[curPosExtra(x-1, y, width, height) + i];
                c = arrBytes[curPosExtra(x, y, width, height) + i];
                d = arrBytes[curPosExtra(x+1, y, width, height) + i];
                e = arrBytes[curPosExtra(x, y+1, width, height) + i];
                if (a >= b && a >= c && a >= d && a >= e){
                    max = a;
                }
                if (b >= a && b >= c && b >= d && b >= e){
                    max = b;
                }
                if (c >= b && c >= a && c >= d && c >= e){
                    max = c;
                }
                if (d >= b && d >= c && d >= a && d >= e){
                    max = d;
                }
                if (e >= b && e >= c && e >= d && e >= a){
                    max = e;
                }

                if (a <= b && a <= c && a <= d && a <= e){
                    min = a;
                }
                if (b <= a && b <= c && b <= d && b <= e){
                    min = b;
                }
                if (c <= b && c <= a && c <= d && c <= e){
                    min = c;
                }
                if (d <= b && d <= c && d <= a && d <= e){
                    min = d;
                }
                if (e <= b && e <= c && e <= d && e <= a){
                    min = e;
                }

                int d_min_g = min;
                int d_max_g = 255 - max;
                int res_max_g;
                if (d_min_g < d_max_g){
                    res_max_g = d_min_g;
                } else {
                    res_max_g = d_max_g;
                }

                float sharperingAmount = (float)((float)res_max_g/max);

                sharperingAmount = std::sqrt(sharperingAmount);
                float developer_maximum = - 0.125 * (1 - sharpness) - 0.2 * sharpness;

                float w = sharperingAmount * developer_maximum;

                int resPixel = (float) ((float)((float)(w*a + w*b + c + w*d + w*e)) / (w * 4 + 1.0));


                if (resPixel > 255) resPixel = 255;
                if (resPixel < 0) resPixel = 0;
                resArrBytes[curPos(x,y,width)+i]=(resPixel);
            }
        }
    }

    delete arrBytes;
    return resArrBytes;
}

unsigned char * Filtrator::cannyFilter(unsigned char * arrBytes, int width, int height){
    std::vector<std::vector<int>> matrix1 = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    std::vector<std::vector<int>> matrix2 = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    unsigned char * moduleSobel = new unsigned char [width * height * 3];
    int * angleSobel = new int [width * height * 3];
    unsigned char * Gx = new unsigned char [width*height*3];
    unsigned char * Gy = new unsigned char [width*height*3];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {

                std::vector<std::vector<int>> vec1 (3, std::vector<int>(3, 0));
                std::vector<std::vector<int>> vec2 (3, std::vector<int>(3, 0));
                std::vector<std::vector<int>> vecFin (3, std::vector<int>(3, 0));

                int sumX=0, sumY=0;
                for (int a = x - 1; a <= x + 1; a++){
                    for (int b = y - 1; b <= y + 1; b++){
                        int mx = a - (x - 1);
                        int my = b - (y - 1);
                        vec1[mx][my]=arrBytes[curPosExtra(a, b, width, height) + i] * matrix1[mx][my];
                        vec2[mx][my]=arrBytes[curPosExtra(a, b, width, height) + i] * matrix2[mx][my];
                        sumX+=vec1[mx][my];
                        sumY+=vec2[mx][my];
                    }
                }
                if (sumX > 255) sumX = 255;
                if (sumX < -255) sumX = 255;
                if (sumX < 0) sumX = 0;
                if (sumY > 255) sumY = 255;
                if (sumY < -255) sumY = 255;
                if (sumY < 0) sumY = 0;
                Gx[curPos(x, y, width) + i] = sumX;
                Gy[curPos(x, y, width) + i] = sumY;
            }
        }
    }
    for (int i = 0; i < height * width * 3; i++){
        int res = std::sqrt(Gx[i]*Gx[i]+Gy[i]*Gy[i]);
        if (res > 255) res = 255;
        moduleSobel[i] = res;
        if (res < 50) moduleSobel[i] = 0;
        angleSobel[i] = (float)(std::atan2(Gy[i],Gx[i])*180)/3.14;
    }



    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {
                if (abs(angleSobel[curPos(x,y,width)+i]-0)<45){
                    if (moduleSobel[curPos(x,y,width)+i] <= moduleSobel[curPos(x-1,y,width)+i] || moduleSobel[curPos(x,y,width)+i] <= moduleSobel[curPos(x+1,y,width)+i]) {
                        moduleSobel[curPos(x,y,width)+i] = 0;
                    }
                }
                if (abs(angleSobel[curPos(x,y,width)+i]-90)<45){
                    if (moduleSobel[curPos(x,y,width)+i] <= moduleSobel[curPos(x,y-1,width)+i] || moduleSobel[curPos(x,y,width)+i] <= moduleSobel[curPos(x,y+1,width)+i]) {
                        moduleSobel[curPos(x,y,width)+i] = 0;
                    }
                }
            }
        }
    }

    delete arrBytes;
    delete Gx;
    delete Gy;
    delete angleSobel;
    return moduleSobel;
}

unsigned char * Filtrator::sobelFilter(unsigned char * arrBytes, int width, int height){
    std::vector<std::vector<int>> matrix1 = {{1, 2, 1}, {0, 0, 0}, {-1, -2, -1}};
    std::vector<std::vector<int>> matrix2 = {{1, 0, -1}, {2, 0, -2}, {1, 0, -1}};
    unsigned char * resArrBytes = new unsigned char [width*height*3];
    unsigned char * Gx = new unsigned char [width*height*3];
    unsigned char * Gy = new unsigned char [width*height*3];
    for (int x = 0; x < width; x++) {
        for (int y = 0; y <= height; y++) {
            for (int i = 0; i < 3; i++) {

                std::vector<std::vector<int>> vec1 (3, std::vector<int>(3, 0));
                std::vector<std::vector<int>> vec2 (3, std::vector<int>(3, 0));
                std::vector<std::vector<int>> vecFin (3, std::vector<int>(3, 0));

                int sumX=0, sumY=0;
                for (int a = x - 1; a <= x + 1; a++){
                    for (int b = y - 1; b <= y + 1; b++){
                        int mx = a - (x - 1);
                        int my = b - (y - 1);
                        vec1[mx][my]=arrBytes[curPosExtra(a, b, width, height) + i] * matrix1[mx][my];
                        vec2[mx][my]=arrBytes[curPosExtra(a, b, width, height) + i] * matrix2[mx][my];
                        sumX+=vec1[mx][my];
                        sumY+=vec2[mx][my];
                    }
                }
                if (sumX > 255) sumX = 255;
                if (sumX < -255) sumX = 255;
                if (sumX < 0) sumX = 0;
                if (sumY > 255) sumY = 255;
                if (sumY < -255) sumY = 255;
                if (sumY < 0) sumY = 0;
                Gx[curPos(x, y, width) + i] = sumX;
                Gy[curPos(x, y, width) + i] = sumY;
            }
        }
    }
    for (int i = 0; i < height * width * 3; i++){
        int res = std::sqrt(Gx[i]*Gx[i]+Gy[i]*Gy[i]);
        if (res > 255) res = 255;
        resArrBytes[i] = res;
    }

    delete arrBytes;
    delete Gx;
    delete Gy;
    return resArrBytes;
}

int Filtrator::curPos(int x, int y, int width) {
    if (y > 0)
        return ((y - 1) * width * 3 + x * 3);
    else return x * 3;
}

int Filtrator::curPosExtra(int x, int y, int width, int height) {


    if (x < 0){
        x = 0;
    }
    if (x >= width){
        x = width - 1;
    }
    if (y < 0){
        y = 0;
    }
    if (y >= height){
        y = height - 1;
    }


    if (y > 0)
        return ((y - 1) * width * 3 + x * 3);
    else return x * 3;

}
