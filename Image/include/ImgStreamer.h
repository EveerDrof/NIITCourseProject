#pragma once

#include <iostream>

#include "Image.h"
using namespace std;
class ImgStreamer
{
public:
    static void write(Image *img,string name);
    static void writeImageDataInStream(char *data,unsigned int length,void *additionalInfo);
    static Image *load(string name);
    static void writeImage(Image *img,void (*writeData)(char *data,unsigned int length,void *additionalInfo),void *additionalInfo,bool withoutPadding=false);
    static Image *loadImage(void (*loadData)(char *buf,unsigned int length,void *additionalData),void *additionalData,bool withoutPadding=false);
    static void loadData(char *buf,unsigned int length,void *additionalData);
};
