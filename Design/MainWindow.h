#ifndef GRAPHICEDITORUP_MAINWINDOW_H
#define GRAPHICEDITORUP_MAINWINDOW_H

#include <windows.h>
#include <Commdlg.h>
#include <fstream>
#include <QtWidgets>
#include <iostream>
#include "../GraphicAlgorithms/ColorSpace.h"
#include "../BasicAlgorithms/FileManager.h"
#include "../GraphicAlgorithms/ColorSpacesInterpreter.h"
#include "../GraphicAlgorithms/GammaChanger.h"
#include "../GraphicAlgorithms/Filtrator.h"
#include "../GraphicAlgorithms/Disiring.h"
#include "../GraphicAlgorithms/Scaler.h"
#include "../GraphicAlgorithms/GistagramChanger.h"
#include "../GraphicAlgorithms/LineDrawer.h"
#include "DesignConstants.h"


class MainWindow : public QWidget
{
Q_OBJECT
public:

    MainWindow(QWidget *parent = nullptr) ;

private

    slots :

    void autocorrectionAction() ;

    void lineDrawActionPressed() ;

    void scaleNNPressed() ;
    void scaleBPressed();
    void scaleLPressed();
    void scaleBCPressed() ;

    void disiringActionOrderPressed();
    void disiringActionRandomPressed();
    void disiringActionFSPressed();
    void disiringActionAtkinsonPressed();
    void disiringGradientPressed() ;


    void filtrationActionThreshHoldPressed();
    void filtrationActionThreshHoldOcyPressed() ;
    void filtrationActionMedianPressed() ;
    void filtrationActionGaussPressed() ;
    void filtrationActionLinearPressed() ;
    void filtrationActionUnshapedPressed() ;
    void filtrationActionShapedPressed() ;
    void filtrationActionSobelPressed();
    void filtrationActionCannyPressed();
    void fileOpenPressed();


    void gammaAssignedPressed() ;
    void gammaConvertPressed() ;

    void colorSpacePressed(int i) ;
    void channelCountPressed(int i) ;
    void channelAllPressed() ;

    void fileSavePnmPressed() ;
    void fileSavePngPressed() ;


private:

    QLineEdit *inputField;
    QLineEdit *inputFieldGamma;
    QLineEdit *disiringBitness;
    QLineEdit *gradientW;
    QLineEdit *gradientH;
    QLineEdit *filteringTreshholdTreshhold;
    QLineEdit *filteringTreshholdOcyTreshhold;
    QLineEdit *filteringTreshholdRange1;
    QLineEdit *filteringTreshholdRange2;
    QLineEdit *filteringMedianRadius;
    QLineEdit *filteringLinearRadius;
    QLineEdit *filteringGaussSigma;
    QLineEdit *filteringUnsharpAmount;
    QLineEdit *filteringUnsharpSigma;
    QLineEdit *filteringUnsharpTreshhold;
    QLineEdit *filteringUnsharpSharpness;
    QLineEdit *scaleW;
    QLineEdit *scaleH;
    QLineEdit *scaleX;
    QLineEdit *scaleY;
    QLineEdit *scaleB;
    QLineEdit *scaleC;
    QLineEdit *autocorrectionIgnore;
    QLineEdit *lineThickness;
    QLineEdit *lineCh1;
    QLineEdit *lineCh2;
    QLineEdit *lineCh3;
    QLineEdit *lineTrancparency;
    QLineEdit *linex1;
    QLineEdit *liney1;
    QLineEdit *linex2;
    QLineEdit *liney2;


    QVBoxLayout *layout;
    QLabel *imageLabel;

    ColorSpace colorSpace = ColorSpace(ColorSpace::Value::RGB);
    int chanelMode = 0;
    float curGamma = 1.0;

    std::ifstream F;
    unsigned char *arrBytes;
    unsigned char *arrBytesRgb;
    int width = 0, height = 0, extraNum = 0;

    void copyArrBytesRgbToArrBytes() ;

    void copyArrBytesToArrBytesRgb() ;

    void updateImage(unsigned char *arr) ;

    QMenuBar *createMenuBar() ;

};

#endif //GRAPHICEDITORUP_MAINWINDOW_H
