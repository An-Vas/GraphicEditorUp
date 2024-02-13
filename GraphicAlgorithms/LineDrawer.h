#ifndef GRAPHICEDITORUP_LINEDRAWER_H
#define GRAPHICEDITORUP_LINEDRAWER_H


class LineDrawer {
public:
    static unsigned char * drawLine(int sx, int sy, int tx, int ty, int ch1, int ch2, int ch3, float thickness, float transparency, unsigned char * arrBytes, int width, int height, int chanel);
private:
    static int curPos(int x, int y, int width);
    static void squareUsingDrawing(int curPos, int ch1, int ch2, int ch3, float transparency, float s, unsigned char * arrBytes, int chanel);
    static float findSquare(float y1, float y2, float thickness);
};



#endif //GRAPHICEDITORUP_LINEDRAWER_H
