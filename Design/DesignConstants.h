#ifndef GRAPHICEDITORUP_DESIGNCONSTANTS_H
#define GRAPHICEDITORUP_DESIGNCONSTANTS_H

#include <QColor>

class DesignConstants{
public:
    static const int appHeight = 1300;
    static const int appImageHeight = 1200;
    static const int appWidth = 2200;

    static const int spaceBetweenButtons = 10;
    static const inline float leftMarginForButtons = 0.0f;
    static const inline float upMarginForButtons = 0.0f;
    static const int buttonHeight = 30;
    static const int colorSpaceButtonWidth = 200;
    static const int gammaButtonWidth = 250;
    static const int diseringButtonWidth = 250;
    static const int linesButtonWidth = 300;
    static inline const QColor buttonLightBackground = QColor(255, 248, 220);
    static inline const QColor buttonPressedBackground = QColor(255, 222, 173);
    static inline const QColor buttonLightBackground2 = QColor(220, 248, 220);
    static inline const QColor buttonPressedBackground2_1 = QColor(150, 190, 140);
    static inline const QColor buttonPressedBackground2 = QColor(190, 222, 173);
    static inline const QColor textBlack = QColor(0, 0, 0);
};

#endif //GRAPHICEDITORUP_DESIGNCONSTANTS_H
