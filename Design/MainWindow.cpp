#include "MainWindow.h"
#include "DesignConstants.h"
#include "../GraphicAlgorithms/ColorSpacesInterpreter.h"

#include <fstream>

MainWindow::MainWindow(QWidget *parent ) : QWidget(parent) {

        setFixedSize(DesignConstants::appWidth, DesignConstants::appHeight);

        QMenuBar *menuBar = createMenuBar();
        imageLabel = new QLabel(this);
        layout = new QVBoxLayout(this);
        layout->setMenuBar(menuBar);
        layout->addWidget(imageLabel);
        layout->setContentsMargins(0, 0, 0, 0);

        setLayout(layout);
}

void MainWindow::autocorrectionAction() {

    float ignoreGistogrammNum = autocorrectionIgnore->text().toFloat();

    if (ignoreGistogrammNum >= 0.5 || ignoreGistogrammNum < 0) {
        ignoreGistogrammNum = 0;
    }

    arrBytesRgb = ColorSpacesInterpreter::toAllChanel(arrBytes, arrBytesRgb, width, height);
    if (colorSpace.value != ColorSpace::Value::RGB) {
        arrBytesRgb = ColorSpacesInterpreter::cast(colorSpace, ColorSpace::Value::RGB,
                                                   arrBytesRgb, width, height);
    }
    chanelMode = 0;

    arrBytesRgb = GistagramChanger::gistagrammChange(arrBytesRgb, width, height, ignoreGistogrammNum);


    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

    if (colorSpace.value != ColorSpace::Value::RGB) {
        arrBytes = ColorSpacesInterpreter::cast(ColorSpace::Value::RGB, colorSpace,
                                                arrBytes, width, height);
    }

}

void MainWindow::lineDrawActionPressed() {

    float lineThicknessf = lineThickness->text().toFloat();
    float lineTransparency = lineTrancparency->text().toFloat();
    int sx = linex1->text().toInt();
    int sy = liney1->text().toInt();
    int tx = linex2->text().toInt();
    int ty = liney2->text().toInt();
    int lineChanel1 = lineCh1->text().toInt();
    int lineChanel2 = lineCh2->text().toInt();
    int lineChanel3 = lineCh3->text().toInt();

    if (lineTransparency <= 0 || lineTransparency > 1) lineTransparency = 1;
    if (lineThicknessf <= 0 || lineThicknessf > height || lineThicknessf > width) lineThicknessf = height / 10;


    if (sx < 0) sx = 0;
    if (sy < 0) sy = 0;
    if (tx < 0) tx = 0;
    if (ty < 0) ty = 0;

    if (sx > width) sx = width;
    if (tx > width) tx = width;
    if (sy > height) sy = height;
    if (ty > height) ty = height;


    if (colorSpace.value == ColorSpace::Value::HSV) {
        UltimateColor newColor = ColorSpacesInterpreter::hsvToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    } else if (colorSpace.value == ColorSpace::Value::HSL) {
        UltimateColor newColor = ColorSpacesInterpreter::hslToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    } else if (colorSpace.value == ColorSpace::Value::YCoCg) {
        UltimateColor newColor = ColorSpacesInterpreter::yCoCgToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    } else if (colorSpace.value == ColorSpace::Value::YCbCr601) {
        UltimateColor newColor = ColorSpacesInterpreter::yCoCgToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    } else if (colorSpace.value == ColorSpace::Value::YCbCr709) {
        UltimateColor newColor = ColorSpacesInterpreter::yCgCr709ToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    } else if (colorSpace.value == ColorSpace::Value::CMY) {
        UltimateColor newColor = ColorSpacesInterpreter::cmyToRgb(
                UltimateColor(lineChanel1, lineChanel2, lineChanel3));
        arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, newColor.x, newColor.y,
                                           newColor.z, lineThicknessf, lineTransparency,
                                           arrBytesRgb, width, height, chanelMode);
    }
    arrBytesRgb = LineDrawer::drawLine(sx, sy, tx, ty, lineChanel1, lineChanel2,
                                       lineChanel3, lineThicknessf, lineTransparency,
                                       arrBytesRgb, width, height, chanelMode);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();


};

void MainWindow::disiringActionOrderPressed() {
    int bitness = disiringBitness->text().toInt();

    if (bitness <= 0 || bitness > 256) {
        bitness = 8;
    }
    arrBytesRgb = Disiring::orderAlorithm(bitness, arrBytesRgb, width, height, curGamma);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();
};


void MainWindow::scaleNNPressed() {

    int newWidth = scaleW->text().toInt();
    int newHeight = scaleH->text().toInt();

    if (newWidth > 3000 || newWidth <= 0) newWidth = 800;
    if (newHeight > 3000 || newHeight <= 0) newHeight = 800;


    float offsetX = scaleX->text().toFloat() - floor(scaleX->text().toFloat());
    float offsetY = scaleY->text().toFloat() - floor(scaleY->text().toFloat());

    arrBytesRgb = Scaler::nearlestNeigboughAlorithm(arrBytesRgb, width, height, newWidth,
                                                    newHeight, offsetX, offsetY);

    width = newWidth;
    height = newHeight;

    arrBytes = new unsigned char[width * height * 3];
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();
};

void MainWindow::scaleBPressed() {

    int newWidth = scaleW->text().toInt();
    int newHeight = scaleH->text().toInt();

    if (newWidth > 3000 || newWidth <= 0) newWidth = 800;
    if (newHeight > 3000 || newHeight <= 0) newHeight = 800;


    float offsetX = scaleX->text().toFloat() - floor(scaleX->text().toFloat());
    float offsetY = scaleY->text().toFloat() - floor(scaleY->text().toFloat());

    arrBytesRgb = Scaler::bilinearAlorithm(arrBytesRgb, width, height, newWidth, newHeight,
                                           offsetX,
                                           offsetY);

    width = newWidth;
    height = newHeight;

    arrBytes = new unsigned char[width * height * 3];
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();
};

void MainWindow::scaleLPressed() {

    int newWidth = scaleW->text().toInt();
    int newHeight = scaleH->text().toInt();

    if (newWidth > 3000 || newWidth <= 0) newWidth = 800;
    if (newHeight > 3000 || newHeight <= 0) newHeight = 800;


    float offsetX = scaleX->text().toFloat() - floor(scaleX->text().toFloat());
    float offsetY = scaleY->text().toFloat() - floor(scaleY->text().toFloat());

    arrBytesRgb = Scaler::lancosAlorithm(arrBytesRgb, width, height, newWidth, newHeight,
                                         offsetX,
                                         offsetY);

    width = newWidth;
    height = newHeight;

    arrBytes = new unsigned char[width * height * 3];
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();
};

void MainWindow::scaleBCPressed() {
    int newWidth = scaleW->text().toInt();
    int newHeight = scaleH->text().toInt();

    if (newWidth > 3000 || newWidth <= 0) newWidth = 800;
    if (newHeight > 3000 || newHeight <= 0) newHeight = 800;


    float offsetX = scaleX->text().toFloat() - floor(scaleX->text().toFloat());
    float offsetY = scaleY->text().toFloat() - floor(scaleY->text().toFloat());

    float b = scaleB->text().toInt();
    float c = scaleC->text().toInt();
    if (b > 1 || b < 0) b = 0;
    if (c > 1 || c < 0) c = 0.5;

    arrBytesRgb = Scaler::bcSplainAlorithm(arrBytesRgb, width, height, newWidth, newHeight, b,
                                           c, offsetX, offsetY);

    width = newWidth;
    height = newHeight;

    arrBytes = new unsigned char[width * height * 3];
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();
};

void MainWindow::disiringActionRandomPressed() {
    int bitness = disiringBitness->text().toInt();

    if (bitness <= 0 || bitness > 256) {
        bitness = 8;
    }
    arrBytesRgb = Disiring::randomAlorithm(bitness, arrBytesRgb, width, height, curGamma);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

};

void MainWindow::disiringActionFSPressed() {

    int bitness = disiringBitness->text().toInt();

    if (bitness <= 0 || bitness > 256) {
        bitness = 8;
    }
    arrBytesRgb = Disiring::FloydSteinsbergAlorithm(bitness, arrBytesRgb, width, height, curGamma);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

};


void MainWindow::disiringActionAtkinsonPressed() {

    int bitness = disiringBitness->text().toInt();

    if (bitness <= 0 || bitness > 256) {
        bitness = 8;
    }
    arrBytesRgb = Disiring::AtkinsonAlorithm(bitness, arrBytesRgb, width, height, curGamma);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

};

void MainWindow::disiringGradientPressed() {


    int w = gradientW->text().toInt();
    int h = gradientH->text().toInt();

    if (w <= 0) width = 200; else width = 0;
    if (h <= 0) height = 200; else height = 0;


    arrBytes = new unsigned char[width * height * 3];
    arrBytesRgb = new unsigned char[width * height * 3];


    int curColor = 0;
    int curPos = 0;

    float colorChangerConst = (float) width / 255;


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            curColor = (float) j / colorChangerConst;
            for (int g = 0; g < 3; g++) {
                arrBytesRgb[curPos] = curColor;
                arrBytes[curPos] = curColor;
                curPos++;
            }
        }
    }


    updateImage(arrBytesRgb);
};


void MainWindow::filtrationActionThreshHoldPressed() {
    int mode = filteringTreshholdTreshhold->text().toInt();

    if (mode != 1 && mode != 2) {
        mode = 1;
    }
    if (mode == 1) {

        int threshold1 = filteringTreshholdRange1->text().toInt();
        if (threshold1 > 255 || threshold1 < 0) {
            threshold1 = 127;

        }
        arrBytesRgb = Filtrator::threshold2classesFilter(arrBytesRgb, width, height, threshold1);
    }
    if (mode == 2) {
        int threshold1 = filteringTreshholdRange1->text().toInt();
        int threshold2 = filteringTreshholdRange2->text().toInt();

        if (threshold1 > 255 || threshold1 < 0) {
            threshold1 = 100;
        }

        if (threshold2 > 255 || threshold2 < 0) {
            threshold2 = 200;

        }
        if (threshold2 < threshold1) std::swap(threshold2, threshold2);
        arrBytesRgb = Filtrator::threshold3classesFilter(arrBytesRgb, width, height, threshold1,
                                                         threshold2);
    }

    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionThreshHoldOcyPressed() {

    int mode = filteringTreshholdOcyTreshhold->text().toInt();
    if (mode != 1 && mode != 2) {
        mode = 1;

    }
    if (mode == 1) {
        arrBytesRgb = Filtrator::thresholdOtsu2classesFilter(arrBytesRgb, width, height);
    }
    if (mode == 2) {
        arrBytesRgb = Filtrator::thresholdOtsu3classesFilter(arrBytesRgb, width, height);
    }


    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}


void MainWindow::filtrationActionMedianPressed() {
    int r = filteringMedianRadius->text().toInt();
    if (r < 1) {
        r = 2;
    }
    arrBytesRgb = Filtrator::medianFilter(arrBytesRgb, width, height, r);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionGaussPressed() {
    float s = filteringGaussSigma->text().toFloat();
    if (s > 12 || s < 0.1) {
        s = 1;

    }
    arrBytesRgb = Filtrator::gaussFilter(arrBytesRgb, width, height, s);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionLinearPressed() {
    int r = filteringLinearRadius->text().toInt();
    if (r < 1) {
        r = 2;
    }
    arrBytesRgb = Filtrator::boxBlurFilter(arrBytesRgb, width, height, r);
    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionUnshapedPressed() {
    float s = filteringUnsharpSigma->text().toFloat();
    float amount = filteringUnsharpAmount->text().toFloat();
    float t = filteringUnsharpTreshhold->text().toFloat();


    if (t > 255 || t < 0) {
        t = 80;

    }
    if (amount > 5 || amount < 0) {
        amount = 1;

    }
    if (s > 12 || s < 0.1) {
        s = 1;

    }

    arrBytesRgb = Filtrator::unsharpedFilter(arrBytesRgb, width, height, amount, s, t);

    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionShapedPressed() {

    float sh = filteringUnsharpSharpness->text().toFloat();
    if (sh > 1 || sh < 0) {
        sh = 0;
    }

    arrBytesRgb = Filtrator::sharpedFilter(arrBytesRgb, width, height, sh);

    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionSobelPressed() {

    arrBytesRgb = Filtrator::sobelFilter(arrBytesRgb, width, height);

    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::filtrationActionCannyPressed() {

    arrBytesRgb = Filtrator::cannyFilter(arrBytesRgb, width, height);

    updateImage(arrBytesRgb);
    copyArrBytesRgbToArrBytes();

}

void MainWindow::fileOpenPressed() {
    FileManager::chooseAndOpenFile(&arrBytes, &width, &height, &extraNum, chanelMode, colorSpace, &curGamma);
    updateImage(arrBytes);
    arrBytesRgb = new unsigned char[width * height * 3];
    copyArrBytesToArrBytesRgb();

}



void MainWindow::gammaAssignedPressed() {

    if (!colorSpace.value == ColorSpace::Value::RGB) {
        return;
    }

    float newGamma = inputFieldGamma->text().toFloat();

    if (newGamma < 0) {
        return;
    }

    arrBytesRgb = GammaChanger::setNewGamma(curGamma, newGamma, arrBytesRgb, width, height);
    curGamma = newGamma;

    updateImage(arrBytesRgb);

}

void MainWindow::gammaConvertPressed() {
    if (!colorSpace.value == ColorSpace::Value::RGB) return;
    float newGamma = inputFieldGamma->text().toFloat();
    if (newGamma < 0) {
        return;
    }
    arrBytes = GammaChanger::setNewGamma(curGamma, newGamma, arrBytes, width, height);
    curGamma = newGamma;
}


void MainWindow::colorSpacePressed(int i) {
    ColorSpacesInterpreter::cast(colorSpace, static_cast<ColorSpace::Value>(i),
                                 arrBytes, width,
                                 height);
    colorSpace = static_cast<ColorSpace::Value>(i);
}


void MainWindow::channelCountPressed(int i) {
    arrBytesRgb = ColorSpacesInterpreter::toOneChanel(i, arrBytes, arrBytesRgb, width,
                                                      height);
    chanelMode = i;
    updateImage(arrBytesRgb);
}

void MainWindow::channelAllPressed() {
    arrBytesRgb = ColorSpacesInterpreter::toAllChanel(arrBytes, arrBytesRgb, width, height);
    if (colorSpace.value != ColorSpace::Value::RGB) {
        arrBytesRgb = ColorSpacesInterpreter::cast(colorSpace, ColorSpace::Value::RGB,
                                                   arrBytesRgb, width, height);
    }
    chanelMode = 0;
    updateImage(arrBytesRgb);
}

void MainWindow::fileSavePnmPressed() {
    FileManager::savePnmFile(arrBytesRgb, width, height, extraNum, chanelMode, colorSpace);
}

void MainWindow::fileSavePngPressed() {
    FileManager::savePngFile(arrBytesRgb, width, extraNum, height, chanelMode, colorSpace, curGamma);
}

void MainWindow::copyArrBytesRgbToArrBytes() {
    for (int i = 0; i < width * height * 3; i++) {
        arrBytes[i] = arrBytesRgb[i];
    }
}

void MainWindow::copyArrBytesToArrBytesRgb() {
    for (int i = 0; i < width * height * 3; i++) {
        arrBytesRgb[i] = arrBytes[i];
    }
}

void MainWindow::updateImage(unsigned char *arr) {

    QImage *image = new QImage(width, height, QImage::Format_RGB888);

    int curPos = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image->setPixelColor(x, y, QColor(arr[curPos + 0], arr[curPos + 1], arr[curPos + 2]));
            curPos += 3;
        }
    }

    int displayHeight = DesignConstants::appImageHeight;
    int displayWidth = ((float) ((float) displayHeight / height) * width);
    if (displayWidth > DesignConstants::appWidth) {
        displayWidth = DesignConstants::appWidth;
        displayHeight = ((float) ((float) displayWidth / width) * height);
    }

    if (height >= width) {
        imageLabel->setPixmap(QPixmap::fromImage(*image).scaledToHeight(displayHeight));
    } else {
        imageLabel->setPixmap(QPixmap::fromImage(*image).scaledToWidth(displayWidth));
    }

    layout->addWidget(imageLabel);

}

QMenuBar *MainWindow::createMenuBar() {
    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *fileActionOpen = fileMenu->addAction("Open");
    QAction *fileActionSavePnm = fileMenu->addAction("Save as pnm");
    QAction *fileActionSavePng = fileMenu->addAction("Save as png");
    connect(fileActionOpen, &QAction::triggered, this, &MainWindow::fileOpenPressed);
    connect(fileActionSavePnm, &QAction::triggered, this, &MainWindow::fileSavePnmPressed);
    connect(fileActionSavePng, &QAction::triggered, this, &MainWindow::fileSavePngPressed);

    QMenu *colorSpacesMenu = menuBar->addMenu("ColorSpaces");

    QAction *colorSpacesAction[7];

    colorSpacesAction[0] = colorSpacesMenu->addAction("RGB");
    colorSpacesAction[1] = colorSpacesMenu->addAction("HSV");
    colorSpacesAction[2] = colorSpacesMenu->addAction("HSL");
    colorSpacesAction[3] = colorSpacesMenu->addAction("YCbCr601");
    colorSpacesAction[4] = colorSpacesMenu->addAction("YCbCr709");
    colorSpacesAction[5] = colorSpacesMenu->addAction("YCoCg");
    colorSpacesAction[6] = colorSpacesMenu->addAction("CMY");

    for (int i = 0; i < 7; i++) {
        connect(colorSpacesAction[i], &QAction::triggered, this, [this, i]() { colorSpacePressed(i); });
    }

    QMenu *countChannelsMenu = menuBar->addMenu("Channels");
    QAction *countChannelsAction[4];

    countChannelsAction[0] = countChannelsMenu->addAction("Show all channels");
    countChannelsAction[1] = countChannelsMenu->addAction("Show channel 1");
    countChannelsAction[2] = countChannelsMenu->addAction("Show channel 2");
    countChannelsAction[3] = countChannelsMenu->addAction("Show channel 3");


    for (int i = 1; i <= 3; i++) {
        connect(countChannelsAction[i], &QAction::triggered, this, [this, i]() { channelCountPressed(i); });
    }
    connect(countChannelsAction[0], &QAction::triggered, this, &MainWindow::channelAllPressed);

    QMenu *gammaMenu = menuBar->addMenu("Gamma");
    QAction *gammaActionAssign = gammaMenu->addAction("assign");
    QAction *gammaActionConvert = gammaMenu->addAction("convert");


    auto *widgetg = new QWidget();
    auto *layoutg = new QHBoxLayout(widgetg);
    auto *labelg = new QLabel("New gamma:", widgetg);
    layoutg->addWidget(labelg);
    inputFieldGamma = new QLineEdit(this);
    layoutg->addWidget(inputFieldGamma);
    auto *widgetActiong = new QWidgetAction(this);
    widgetActiong->setDefaultWidget(widgetg);
    gammaMenu->addAction(widgetActiong);

    connect(gammaActionAssign, &QAction::triggered, this, &MainWindow::gammaAssignedPressed);
    connect(gammaActionConvert, &QAction::triggered, this, &MainWindow::gammaConvertPressed);


    QMenu *disiringMenu = menuBar->addMenu("Disiring");
    QAction *disiringActionOrder = disiringMenu->addAction("Order");
    QAction *disiringActionRandom = disiringMenu->addAction("Random");
    QAction *disiringActionFS = disiringMenu->addAction("Floyd-Steinsberg");
    QAction *disiringActionAtkinson = disiringMenu->addAction("Atkinson");


    auto *widgetdis1 = new QWidget();
    auto *layoutdis1 = new QHBoxLayout(widgetdis1);
    auto *labeldis1 = new QLabel("Bitness:", widgetdis1);
    layoutdis1->addWidget(labeldis1);
    disiringBitness = new QLineEdit(this);
    layoutdis1->addWidget(disiringBitness);
    auto *widgetActiondis1 = new QWidgetAction(this);
    widgetActiondis1->setDefaultWidget(widgetdis1);
    disiringMenu->addAction(widgetActiondis1);


    auto *widgetdis2 = new QWidget();
    auto *layoutdis2 = new QHBoxLayout(widgetdis2);
    auto *labeldis2 = new QLabel("Gradient width:", widgetdis2);
    layoutdis2->addWidget(labeldis2);
    gradientW = new QLineEdit(this);
    layoutdis2->addWidget(gradientW);
    auto *widgetActiondis2 = new QWidgetAction(this);
    widgetActiondis2->setDefaultWidget(widgetdis2);
    disiringMenu->addAction(widgetActiondis2);

    auto *widgetdis3 = new QWidget();
    auto *layoutdis3 = new QHBoxLayout(widgetdis3);
    auto *labeldis3 = new QLabel("Gradient height:", widgetdis3);
    layoutdis3->addWidget(labeldis3);
    gradientH = new QLineEdit(this);
    layoutdis3->addWidget(gradientH);
    auto *widgetActiondis3 = new QWidgetAction(this);
    widgetActiondis3->setDefaultWidget(widgetdis3);
    disiringMenu->addAction(widgetActiondis3);


    QAction *disiringGradient = disiringMenu->addAction("Make Gradient");

    connect(disiringActionOrder, &QAction::triggered, this, &MainWindow::disiringActionOrderPressed);
    connect(disiringActionRandom, &QAction::triggered, this, &MainWindow::disiringActionRandomPressed);
    connect(disiringActionFS, &QAction::triggered, this, &MainWindow::disiringActionFSPressed);
    connect(disiringActionAtkinson, &QAction::triggered, this, &MainWindow::disiringActionAtkinsonPressed);
    connect(disiringGradient, &QAction::triggered, this, &MainWindow::disiringGradientPressed);


    QMenu *scaleMenu = menuBar->addMenu("Scale");

    auto *widgets1 = new QWidget();
    auto *layouts1 = new QHBoxLayout(widgets1);
    auto *labels1 = new QLabel("New width:", widgets1);
    layouts1->addWidget(labels1);
    scaleW = new QLineEdit(this);
    layouts1->addWidget(scaleW);
    auto *widgetActions1 = new QWidgetAction(this);
    widgetActions1->setDefaultWidget(widgets1);
    scaleMenu->addAction(widgetActions1);

    auto *widgets2 = new QWidget();
    auto *layouts2 = new QHBoxLayout(widgets2);
    auto *labels2 = new QLabel("New height:", widgets2);
    layouts2->addWidget(labels2);
    scaleH = new QLineEdit(this);
    layouts2->addWidget(scaleH);
    auto *widgetActions2 = new QWidgetAction(this);
    widgetActions2->setDefaultWidget(widgets2);
    scaleMenu->addAction(widgetActions2);


    auto *widgets3 = new QWidget();
    auto *layouts3 = new QHBoxLayout(widgets3);
    auto *labels3 = new QLabel("X:", widgets3);
    layouts3->addWidget(labels3);
    scaleX = new QLineEdit(this);
    layouts3->addWidget(scaleX);
    auto *widgetActions3 = new QWidgetAction(this);
    widgetActions3->setDefaultWidget(widgets3);
    scaleMenu->addAction(widgetActions3);

    auto *widgets4 = new QWidget();
    auto *layouts4 = new QHBoxLayout(widgets4);
    auto *labels4 = new QLabel("Y:", widgets4);
    layouts4->addWidget(labels4);
    scaleY = new QLineEdit(this);
    layouts4->addWidget(scaleY);
    auto *widgetActions4 = new QWidgetAction(this);
    widgetActions4->setDefaultWidget(widgets4);
    scaleMenu->addAction(widgetActions4);


    QAction *scaleActionNN = scaleMenu->addAction("Nearest Neighbour algo");
    QAction *scaleActionBilinear = scaleMenu->addAction("Bilinear algo");
    QAction *scaleActionLanczos3 = scaleMenu->addAction("Lanczos3 algo");
    QAction *scaleActionBC = scaleMenu->addAction("BC-spline algo");

    connect(scaleActionNN, &QAction::triggered, this, &MainWindow::scaleNNPressed);
    connect(scaleActionBilinear, &QAction::triggered, this, &MainWindow::scaleBPressed);
    connect(scaleActionLanczos3, &QAction::triggered, this, &MainWindow::scaleLPressed);
    connect(scaleActionBC, &QAction::triggered, this, &MainWindow::scaleBCPressed);

    auto *widgets5 = new QWidget();
    auto *layouts5 = new QHBoxLayout(widgets5);
    auto *labels5 = new QLabel("B:", widgets5);
    layouts5->addWidget(labels5);
    scaleB = new QLineEdit(this);
    layouts5->addWidget(scaleB);
    auto *widgetActions5 = new QWidgetAction(this);
    widgetActions5->setDefaultWidget(widgets5);
    scaleMenu->addAction(widgetActions5);


    auto *widgets6 = new QWidget();
    auto *layouts6 = new QHBoxLayout(widgets6);
    auto *labels6 = new QLabel("C:", widgets6);
    layouts6->addWidget(labels6);
    scaleC = new QLineEdit(this);
    layouts6->addWidget(scaleC);
    auto *widgetActions6 = new QWidgetAction(this);
    widgetActions6->setDefaultWidget(widgets6);
    scaleMenu->addAction(widgetActions6);


    QMenu *autocorrectionMenu = menuBar->addMenu("AutoCorrection");
    QAction *autocorrectionAction = autocorrectionMenu->addAction("Autocorrection");
    connect(autocorrectionAction, &QAction::triggered, this, &MainWindow::autocorrectionAction);

    auto *widgeta = new QWidget();
    auto *layouta = new QHBoxLayout(widgeta);
    auto *labela = new QLabel("Ignored part:", widgeta);
    layouta->addWidget(labela);
    autocorrectionIgnore = new QLineEdit(this);
    layouta->addWidget(autocorrectionIgnore);
    auto *widgetActiona = new QWidgetAction(this);
    widgetActiona->setDefaultWidget(widgeta);
    autocorrectionMenu->addAction(widgetActiona);


    QMenu *lineMenu = menuBar->addMenu("Line");
    QAction *drawLineAction = lineMenu->addAction("Draw");


    auto *widgetl = new QWidget();
    auto *layoutl = new QVBoxLayout(widgetl);

    auto *labell1 = new QLabel("Thickness:", widgetl);
    layoutl->addWidget(labell1);
    lineThickness = new QLineEdit(this);
    layoutl->addWidget(lineThickness);

    auto *labell2 = new QLabel("Channel 1:", widgetl);
    layoutl->addWidget(labell2);
    lineCh1 = new QLineEdit(this);
    layoutl->addWidget(lineCh1);

    auto *labell3 = new QLabel("Channel 2:", widgetl);
    layoutl->addWidget(labell3);
    lineCh2 = new QLineEdit(this);
    layoutl->addWidget(lineCh2);

    auto *labell4 = new QLabel("Channel 3:", widgetl);
    layoutl->addWidget(labell4);
    lineCh3 = new QLineEdit(this);
    layoutl->addWidget(lineCh3);


    auto *labell5 = new QLabel("Transparency:", widgetl);
    layoutl->addWidget(labell5);
    lineTrancparency = new QLineEdit(this);
    layoutl->addWidget(lineTrancparency);

    auto *labell6 = new QLabel("Point 1 x:", widgetl);
    layoutl->addWidget(labell6);
    linex1 = new QLineEdit(this);
    layoutl->addWidget(linex1);


    auto *labell7 = new QLabel("Point 1 y:", widgetl);
    layoutl->addWidget(labell7);
    liney1 = new QLineEdit(this);
    layoutl->addWidget(liney1);

    auto *labell8 = new QLabel("Point 2 x:", widgetl);
    layoutl->addWidget(labell8);
    linex2 = new QLineEdit(this);
    layoutl->addWidget(linex2);

    auto *labell9 = new QLabel("Point 2 y:", widgetl);
    layoutl->addWidget(labell9);
    liney2 = new QLineEdit(this);
    layoutl->addWidget(liney2);


    auto *widgetActionl = new QWidgetAction(this);
    widgetActionl->setDefaultWidget(widgetl);
    lineMenu->addAction(widgetActionl);

    connect(drawLineAction, &QAction::triggered, this, &MainWindow::lineDrawActionPressed);


    QMenu *filtrationMenu = menuBar->addMenu("Filters");


    QAction *filtrationActionThreshold = filtrationMenu->addAction("Apply Threshold filter");
    QAction *filtrationActionThresholdOcy = filtrationMenu->addAction("Apply Threshold Ocy filter");
    QAction *filtrationActionMedian = filtrationMenu->addAction("Apply Median filter");
    QAction *filtrationActionGauss = filtrationMenu->addAction("Apply Gauss filter");
    QAction *filtrationActionLinear = filtrationMenu->addAction("Apply Linear filter");
    QAction *filtrationActionUnshaped = filtrationMenu->addAction("Apply Unshaped masking filter");
    QAction *filtrationActionCAS = filtrationMenu->addAction("Apply CAS filter");
    QAction *filtrationActionSobel = filtrationMenu->addAction("Apply Sobel filter");
    QAction *filtrationActionCanny = filtrationMenu->addAction("Apply Canny filter");


    auto *widgetf = new QWidget();
    auto *layoutf = new QVBoxLayout(widgetf);

    auto *labelf11 = new QLabel("Threshold filter, Threshold count:", widgetf);
    layoutf->addWidget(labelf11);
    filteringTreshholdTreshhold = new QLineEdit(this);
    layoutf->addWidget(filteringTreshholdTreshhold);

    auto *labelf12 = new QLabel("Threshold filter, Range 1:", widgetf);
    layoutf->addWidget(labelf12);
    filteringTreshholdRange1 = new QLineEdit(this);
    layoutf->addWidget(filteringTreshholdRange1);

    auto *labelf13 = new QLabel("Threshold filter, Range 2:", widgetf);
    layoutf->addWidget(labelf13);
    filteringTreshholdRange2 = new QLineEdit(this);
    layoutf->addWidget(filteringTreshholdRange2);

    auto *labelf21 = new QLabel("Threshold Ocy filter, Threshold count:", widgetf);
    layoutf->addWidget(labelf21);
    filteringTreshholdOcyTreshhold = new QLineEdit(this);
    layoutf->addWidget(filteringTreshholdOcyTreshhold);


    auto *labelf31 = new QLabel("Median filter, Core radius:", widgetf);
    layoutf->addWidget(labelf31);
    filteringMedianRadius = new QLineEdit(this);
    layoutf->addWidget(filteringMedianRadius);

    auto *labelf41 = new QLabel("Gauss filter, Sigma:", widgetf);
    layoutf->addWidget(labelf41);
    filteringGaussSigma = new QLineEdit(this);
    layoutf->addWidget(filteringGaussSigma);


    auto *labelf51 = new QLabel("Linear filter, Core radius:", widgetf);
    layoutf->addWidget(labelf51);
    filteringLinearRadius = new QLineEdit(this);
    layoutf->addWidget(filteringLinearRadius);

    auto *labelf61 = new QLabel("Unshaped masking filter, Amount:", widgetf);
    layoutf->addWidget(labelf61);
    filteringUnsharpAmount = new QLineEdit(this);
    layoutf->addWidget(filteringUnsharpAmount);

    auto *labelf62 = new QLabel("Unshaped masking filter, Sigma:", widgetf);
    layoutf->addWidget(labelf62);
    filteringUnsharpAmount = new QLineEdit(this);
    layoutf->addWidget(filteringUnsharpAmount);

    auto *labelf63 = new QLabel("Unshaped masking filter, Threshold count:", widgetf);
    layoutf->addWidget(labelf63);
    filteringUnsharpSigma = new QLineEdit(this);
    layoutf->addWidget(filteringUnsharpSigma);

    auto *labelf71 = new QLabel("CAS filter, Sharpness:", widgetf);
    layoutf->addWidget(labelf71);
    filteringUnsharpSharpness = new QLineEdit(this);
    layoutf->addWidget(filteringUnsharpSharpness);


    auto *widgetActionf = new QWidgetAction(this);
    widgetActionf->setDefaultWidget(widgetf);
    filtrationMenu->addAction(widgetActionf);


    connect(filtrationActionThreshold, &QAction::triggered, this, &MainWindow::filtrationActionThreshHoldPressed);
    connect(filtrationActionThresholdOcy, &QAction::triggered, this,
            &MainWindow::filtrationActionThreshHoldOcyPressed);
    connect(filtrationActionMedian, &QAction::triggered, this, &MainWindow::filtrationActionMedianPressed);
    connect(filtrationActionGauss, &QAction::triggered, this, &MainWindow::filtrationActionGaussPressed);
    connect(filtrationActionLinear, &QAction::triggered, this, &MainWindow::filtrationActionLinearPressed);
    connect(filtrationActionUnshaped, &QAction::triggered, this, &MainWindow::filtrationActionUnshapedPressed);
    connect(filtrationActionCAS, &QAction::triggered, this, &MainWindow::filtrationActionShapedPressed);
    connect(filtrationActionSobel, &QAction::triggered, this, &MainWindow::filtrationActionSobelPressed);
    connect(filtrationActionCanny, &QAction::triggered, this, &MainWindow::filtrationActionCannyPressed);


    return menuBar;
}
