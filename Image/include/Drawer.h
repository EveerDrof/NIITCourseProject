#pragma once
#include <math.h>
#include <iostream>

#include "Image.h"
#include "PixelPainter.h"
typedef unsigned int radius;
typedef long colUsageNum;
typedef unsigned int colLocation;
class Drawer
{
public:
    struct circleData
    {
        PixelPainter::pixelPos pixPos;
        pos j;
        pos i;
        len powHeight;
        len powWidth;
    };
    struct colUsageData
    {
        colUsageNum usgNum;
        pixel pix;
    };


    static void drawEllipse(Image *image,pos x,pos y,len height,len width);
    static void paintTones(Image *image,unsigned int tonesNumber,pos x=0,pos y=0,len height=0,len width=0);
    typedef unsigned short colDiff;
    static void processExcept(Image &img,pixel pix,colDiff maxDiff);

private:
    static void fillCirclePixel(pixel *pix,void *data);
    struct toneProcessorData{
        Image::pixelPos pos;
        colUsageData *data;
    };
    struct chNeColData{
        Image::pixelPos pos;
        toneProcessorData *tpData;
        unsigned int lowerCol;
        unsigned int retVal;
        bool changePixFlag;
    };
    struct paintWBExceptData{
        Image::pixelPos pos;
        pixel pix;
        colDiff maxDiff;
    };

    static void incrColNumbUsage(pixel *pix,void *data);
    static colLocation getPixCode(pixel &pix);
    static void chooseNearestColor(pixel *pix,void *data);
    static colUsageData *initColUsgDataArr();
    static int cmpfunc (const void * a, const void * b);
    static void paintWhiteBlackExcept(pixel *pix,void *data);

    static colDiff pixDiff(pixel &a,pixel &b);

};
