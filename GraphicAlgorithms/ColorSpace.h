#ifndef GRAPHICEDITORUP_COLORSPACE_H
#define GRAPHICEDITORUP_COLORSPACE_H

#include <iomanip>
#include <cstdint>

class ColorSpace {
public:
    enum Value : uint8_t {
        RGB = 0,
        HSV = 1,
        HSL = 2,
        YCbCr601 = 3,
        YCbCr709 = 4,
        YCoCg = 5,
        CMY = 6,
    };
    Value value;

    ColorSpace(ColorSpace::Value value1) {
        value = value1;
    }

    std::string toString() {
        if (value == ColorSpace::Value::RGB) return "RGB";
        if (value == ColorSpace::Value::HSV) return "HSV";
        if (value == ColorSpace::Value::HSL) return "HSL";
        if (value == ColorSpace::Value::YCbCr601) return "YCbCr601";
        if (value == ColorSpace::Value::YCbCr709) return "YCbCr709";
        if (value == ColorSpace::Value::YCoCg) return "YCoCg";
        if (value == ColorSpace::Value::CMY) return "CMY";
    }


};

#endif //GRAPHICEDITORUP_COLORSPACE_H
