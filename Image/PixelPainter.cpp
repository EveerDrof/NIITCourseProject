#include "include/PixelPainter.h"
void PixelPainter::makeGrey(pixel *pix,void *data)
{
    colbyte maxVal=pix->b;
    if(maxVal<pix->r)
    {
        maxVal=pix->r;
    }
    if(maxVal<pix->g)
    {
        maxVal=pix->g;
    }
    pix->b = pix->g = pix->r = maxVal;
}

