#pragma once
#include "Image.h"
class PixelPainter
{
public:
    //static void paintPixel(pixel *pix);
    struct pixelPos{
        pos y;
        pos x;
    };
    static void makeGrey(pixel *pix,void *data);
};
