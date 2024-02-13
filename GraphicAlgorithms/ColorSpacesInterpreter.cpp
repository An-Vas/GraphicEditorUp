
#include <algorithm>
#include <iostream>
#include <cmath>
#include "ColorSpacesInterpreter.h"


UltimateColor ColorSpacesInterpreter::rgbToHsv(UltimateColor color) {

    float r = (float) color.x / 255, g = (float) color.y / 255, b = (float) color.z / 255;

    if (r > 1) r = 1;
    if (g > 1) g = 1;
    if (b > 1) b = 1;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    float temp = std::max(r, g);
    float max = std::max(temp, b);
    temp = std::min(r, g);
    float min = std::min(temp, b);

    float h, s, v;

    if (min == max) {
        h = 0;
    } else if (max == r && g >= b) {
        h = 60.0f * (((g - b) / (max - min)));
    } else if (max == r && g < b) {
        h = 60 * ((g - b) / (max - min)) + 360;
    } else if (max == g) {
        h = 60 * ((b - r) / (max - min)) + 120;
    } else if (max == b) {
        h = 60 * ((r - g) / (max - min)) + 240;
    }

    if (max == 0) {
        s = 0;
    } else {
        s = (int) ((float) (1 - (float) ((float) min / (float) max)) * 100);
    }

    v = max * 100;

    if (std::round(h) == -1) h = 0;
    if (std::round(s) == -1) s = 0;
    if (std::round(v) == -1) v = 0;

    if (std::round(h) == 361) h = 360;
    if (std::round(s) > 100) s = 100;
    if (std::round(v) > 100) v = 100;

    UltimateColor res = UltimateColor(std::round(((float) ((float) h / 360) * 255)),
                                      std::round(((float) ((float) s / 100) * 255)), ((float) ((float) v / 100) * 255));

    return res;
}

UltimateColor ColorSpacesInterpreter::hsvToRgb(UltimateColor color) {
    float h = (((float) color.x / 255) * 360), s = (((float) color.y / 255)), v = (((float) color.z / 255));

    if (s > 1) s = 1;
    if (v > 1) v = 1;

    if (h < 0) h = 0;
    if (s < 0) s = 0;
    if (v < 0) v = 0;

    float r, g, b;

    int temp = (int) std::floor(h / 60) % 6;
    float f = (float) (h / 60) - std::floor(h / 60);
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    if (temp == 0) {
        r = v;
        g = t;
        b = p;
    } else if (temp == 1) {
        r = q;
        g = v;
        b = p;
    } else if (temp == 2) {
        r = p;
        g = v;
        b = t;
    } else if (temp == 3) {
        r = p;
        g = q;
        b = v;
    } else if (temp == 4) {
        r = t;
        g = p;
        b = v;
    } else if (temp == 5) {
        r = v;
        g = p;
        b = q;
    }
    if (std::round(r) == -1) r = 0;
    if (std::round(g) == -1) g = 0;
    if (std::round(b) == -1) b = 0;

    if (std::round(r) == 256) r = 255;
    if (std::round(g) == 256) g = 255;
    if (std::round(b) == 256) b = 255;

    UltimateColor res = UltimateColor(std::round(r * 255), std::round(g * 255), std::round(b * 255));
    return res;
}

UltimateColor ColorSpacesInterpreter::rgbToHsl(UltimateColor color) {
    float r = (float) color.x / 255, g = (float) color.y / 255, b = (float) color.z / 255;


    float temp = std::max(r, g);
    float max = std::max(temp, b);
    temp = std::min(r, g);
    float min = std::min(temp, b);

    float h, s, l, diff = max - min;

    l = 0.5f * (max + min);

    if (max == min) {
        h = 0;
    } else if (max == r && g >= b) {
        h = 60 * ((float) (g - b) / (diff));
    } else if (max == r && g < b) {
        h = 60 * ((float) (g - b) / (diff)) + 360;
    } else if (max == g) {
        h = 60 * ((float) (b - r) / (diff)) + 120;
    } else if (max == b) {
        h = 60 * ((float) (r - g) / (diff)) + 240;
    }

    if (max == min || l == 0) {
        s = 0;
    } else if (l > 0 && l <= 0.5f) {
        s = (float) (diff) / (2.0f * l);
    } else if (l > 0.5f && l <= 1) {
        s = (float) (diff) / (2.0f - 2.0f * l);
    }

    return UltimateColor(std::round((float) h / 360 * 255), std::round(s * 255), std::round(l * 255));
}

UltimateColor ColorSpacesInterpreter::hslToRgb(UltimateColor color) {
    float h = (((float) color.x / 255)), s = (float) color.y / 255, l = (float) color.z / 255;

    float q, p, tr, tg, tb, r, g, b;
    if (l < 0.5f) {
        q = l * (1.0 + s);
    } else {
        q = l + s - (l * s);
    }

    p = 2.0f * l - q;

    tr = h + (float) 1.0 / 3.0;
    tg = h;
    tb = h - (float) 1.0 / 3.0;

    if (tr < 0) tr = tr + 1.0;
    if (tg < 0) tg = tg + 1.0;
    if (tb < 0) tb = tb + 1.0;

    if (tr > 1) tr = tr - 1.0;
    if (tg > 1) tg = tg - 1.0;
    if (tb > 1) tb = tb - 1.0;

    if (tr < (float) 1.0f / 6.0f) {
        r = p + ((q - p) * 6.0f * tr);
    } else if (tr < 0.5f) {
        r = q;
    } else if (tr < (float) 2.0f / 3.0f) {
        r = p + ((q - p) * ((float) 2.0f / 3.0f - tr) * 6.0f);
    } else {
        r = p;
    }

    if (tg < (float) 1.0 / 6.0) {
        g = p + ((q - p) * 6.0 * tg);
    } else if (tg < 0.5f) {
        g = q;
    } else if (tg < (float) 2.0 / 3.0) {
        g = p + ((q - p) * ((float) 2.0 / 3.0 - tg) * 6.0);
    } else {
        g = p;
    }

    if (tb < (float) 1.0f / 6.0f) {
        b = p + ((q - p) * 6.0f * tb);
    } else if (tb < 0.5f) {
        b = q;
    } else if (tb < (float) 2.0f / 3.0f) {
        b = p + ((q - p) * ((float) 2.0f / 3.0f - tb) * 6.0f);
    } else {
        b = p;
    }

    return UltimateColor(std::round((float) r * 255), std::round((float) g * 255), std::round(b * 255));

}

UltimateColor ColorSpacesInterpreter::rgbToYCoCg(UltimateColor color) {

    float r = (float) color.x / 255, g = (float) color.y / 255, b = (float) color.z / 255;

    if (r > 1) r = 1;
    if (g > 1) g = 1;
    if (b > 1) b = 1;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;

    float y = (float) (r / 4) + (float) (g / 2) + (float) (b / 4);
    float co = (float) (r / 2) - (float) (b / 2);
    float cg = -(float) (r / 4) + (float) (g / 2) - (float) (b / 4);

    if (y > 1) y = 1;
    if (y < 0) y = 0;
    if (co > 0.5f) co = 0.5f;
    if (co < -0.5f) co = -0.5f;
    if (cg > 0.5f) cg = 0.5f;
    if (cg < -0.5f) cg = -0.5f;


    if (r == 0.0f && g == 0.0f && b == 1.0f) return UltimateColor(64, 0, 64);

    return UltimateColor(std::round(y * 255), std::round(co * 255) + 127, std::round(cg * 255) + 127);

}

UltimateColor ColorSpacesInterpreter::yCoCgToRgb(UltimateColor color) {
    if (color.x == 64 && color.y == 0 && color.z == 64) { return UltimateColor(0, 0, 255); }
    float y = (float) color.x / 255.0f, co = (float) ((color.y - 127) / 255.0f), cg = (float) ((color.z - 127) /
                                                                                               255.0f);
    if (y > 1) y = 1;
    if (y < 0) y = 0;
    if (co > 0.5f) co = 0.5f;
    if (co < -0.5f) co = -0.5f;
    if (cg > 0.5f) cg = 0.5f;
    if (cg < -0.5f) cg = -0.5f;


    float r = y + co - cg;
    float g = y + cg;
    float b = y - co - cg;

    if (std::round(r * 255) <= -1) r = 0;
    if (std::round(g * 255) <= -1) g = 0;
    if (std::round(b * 255) <= -1) b = 0;

    if (std::round(r * 255) >= 256) r = 1;
    if (std::round(g * 255) >= 256) g = 1;
    if (std::round(b * 255) >= 256) b = 1;

    return UltimateColor(std::round(r * 255), std::round(g * 255), std::round(b * 255));
}

UltimateColor ColorSpacesInterpreter::rgbToYCgCr601(UltimateColor color) {

    float r = (float) color.x, g = (float) color.y, b = (float) color.z;

    float kr = 0.299, kg = 0.587, kb = 0.114;

    float y = kr * r + kg * g + kb * b;
    float cb = (-0.5f * (float) (kr / (1 - kb)) * r - 0.5f * (float) (kg / (1 - kb)) * g + 0.5f * b);
    float cr = (0.5f * r - 0.5f * (float) (kg / (1 - kr)) * g - 0.5f * (float) (kb / (1 - kr)) * b);


    if (std::round(y) == 256) r = 255;
    if (std::round(cb) == 128) g = 127;
    if (std::round(cr) == 128) b = 127;


    return UltimateColor(std::round(y), std::round(cb + 127), std::round(cr + 127));
}

UltimateColor ColorSpacesInterpreter::yCgCr601ToRgb(UltimateColor color) {

    float y = (float) color.x, cb = (float) (color.y - 127), cr = (float) (color.z) - 127;

    float kr = 0.299, kg = 0.587, kb = 0.114;

    float r = y + (2 - 2 * kr) * cr;
    float b = y + (2 - 2 * kb) * (cb);
    float g = (float) ((-kr * r - kb * b + y) / (kg));

    if (std::round(r) == -1) r = 0;
    if (std::round(g) == -1) g = 0;
    if (std::round(b) == -1) b = 0;

    if (std::round(r) == 256) r = 255;
    if (std::round(g) == 256) g = 255;
    if (std::round(b) == 256) b = 255;


    return UltimateColor(std::round(r), std::round(g), std::round(b));
}

UltimateColor ColorSpacesInterpreter::rgbToYCgCr709(UltimateColor color) {
    float r = (float) color.x, g = (float) color.y, b = (float) color.z;

    float kr = 0.2126, kg = 0.7151, kb = 0.0722;

    float y = kr * r + kg * g + kb * b;
    float cb = (-0.5f * (float) (kr / (1 - kb)) * r - 0.5f * (float) (kg / (1 - kb)) * g + 0.5f * b);
    float cr = (0.5f * r - 0.5f * (float) (kg / (1 - kr)) * g - 0.5f * (float) (kb / (1 - kr)) * b);

    return UltimateColor(std::round(y), std::round(cb + 127), std::round(cr + 127));
}

UltimateColor ColorSpacesInterpreter::yCgCr709ToRgb(UltimateColor color) {
    float kr = 0.2126, kg = 0.7151, kb = 0.0722;
    float y = (float) color.x, cb = (float) (color.y - 127), cr = (float) (color.z) - 127;

    float r = y + (2 - 2 * kr) * cr;
    float b = y + (2 - 2 * kb) * (cb);
    float g = (float) ((-kr * r - kb * b + y) / (kg));


    if (std::round(r) == -1) r = 0;
    if (std::round(g) == -1) g = 0;
    if (std::round(b) == -1) b = 0;

    if (std::round(r) == 256) r = 255;
    if (std::round(g) == 256) g = 255;
    if (std::round(b) == 256) b = 255;

    return UltimateColor(std::round(r), std::round(g), std::round(b));
}


UltimateColor ColorSpacesInterpreter::rgbToCmy(UltimateColor color) {
    float r = (float) color.x / 255, g = (float) color.y / 255, b = (float) color.z / 255;

    float c = 1 - r;
    float m = 1 - g;
    float y = 1 - b;

    return UltimateColor(std::round(c * 255), std::round(m * 255), std::round(y * 255));
}

UltimateColor ColorSpacesInterpreter::cmyToRgb(UltimateColor color) {
    float c = (float) color.x / 255, m = (float) color.y / 255, y = (float) color.z / 255;

    float r = 1 - c;
    float g = 1 - m;
    float b = 1 - y;

    return UltimateColor(std::round(r * 255), std::round(g * 255), std::round(b * 255));
}

unsigned char *
ColorSpacesInterpreter::cast(ColorSpace original, ColorSpace goal, unsigned char *arrBytes, int width, int height) {
    if (goal.value == original.value)
        return arrBytes;

    if (goal.value == ColorSpace::Value::RGB) {
        arrBytes = castToRgb(original, arrBytes, width, height);
        return arrBytes;
    } else if (original.value != ColorSpace::Value::RGB) {
        arrBytes = castToRgb(original, arrBytes, width, height);
    }

    UltimateColor newColor;


    int curPos = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            UltimateColor originalColor = UltimateColor(arrBytes[curPos], arrBytes[curPos + 1], arrBytes[curPos + 2]);
            if (goal.value == ColorSpace::Value::HSV) {
                newColor = rgbToHsv(originalColor);

            } else if (goal.value == ColorSpace::Value::HSL) {
                newColor = rgbToHsl(originalColor);

            } else if (goal.value == ColorSpace::Value::YCbCr601) {
                newColor = rgbToYCgCr601(originalColor);

            } else if (goal.value == ColorSpace::Value::YCoCg) {
                newColor = rgbToYCoCg(originalColor);

            } else if (goal.value == ColorSpace::Value::YCbCr709) {
                newColor = rgbToYCgCr709(originalColor);

            } else if (goal.value == ColorSpace::Value::CMY) {
                newColor = rgbToCmy(originalColor);
            }

            arrBytes[curPos] = newColor.x;
            arrBytes[curPos + 1] = newColor.y;
            arrBytes[curPos + 2] = newColor.z;

            curPos += 3;
        }
    }

    return arrBytes;
};

unsigned char *ColorSpacesInterpreter::castToRgb(ColorSpace original, unsigned char *arrBytes, int width, int height) {
    int curPos = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            UltimateColor originalColor = UltimateColor(arrBytes[curPos], arrBytes[curPos + 1], arrBytes[curPos + 2]);
            UltimateColor originalToRGB;


            if (original.value == ColorSpace::Value::HSV) {
                originalToRGB = hsvToRgb(originalColor);

            } else if (original.value == ColorSpace::Value::HSL) {
                originalToRGB = hslToRgb(originalColor);

            } else if (original.value == ColorSpace::Value::YCoCg) {
                originalToRGB = yCoCgToRgb(originalColor);

            } else if (original.value == ColorSpace::Value::YCbCr601) {
                originalToRGB = yCgCr601ToRgb(originalColor);

            } else if (original.value == ColorSpace::Value::YCbCr709) {
                originalToRGB = yCgCr709ToRgb(originalColor);

            } else if (original.value == ColorSpace::Value::CMY) {
                originalToRGB = cmyToRgb(originalColor);
            }


            arrBytes[curPos] = originalToRGB.x;
            arrBytes[curPos + 1] = originalToRGB.y;
            arrBytes[curPos + 2] = originalToRGB.z;

            curPos += 3;

        }
    }

    return arrBytes;
}


unsigned char *
ColorSpacesInterpreter::toOneChanel(int chanel, unsigned char *arrBytes, unsigned char *resArrBytes, int width,
                                    int height) {

    for (int i = 0; i < height * width * 3; i += 3) {
        char Sample = arrBytes[i + chanel - 1];
        resArrBytes[i + 2] = Sample;
        resArrBytes[i + 1] = Sample;
        resArrBytes[i] = Sample;
    }
    return resArrBytes;
}

unsigned char *
ColorSpacesInterpreter::toAllChanel(unsigned char *arrBytes, unsigned char *resArrBytes, int width, int height) {

    for (int i = 0; i < height * width * 3; i++) {
        resArrBytes[i] = arrBytes[i];
    }
    return resArrBytes;
}