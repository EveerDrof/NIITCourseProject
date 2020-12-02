#pragma once
#include <string>
#include <fstream>
typedef unsigned char colbyte;

typedef unsigned int pos;
typedef unsigned int len;
using namespace std;
struct pixel
{
    /*colbyte r;
    colbyte g;
    colbyte b;*/
    colbyte b;
    colbyte g;
    colbyte r;
};
class Image
{
public:
    Image(len width, len height);
    ~Image();
    void reset(len width, len height);
    pixel* getPixel(pos y,pos x);
    len getHeight();
    len getWidth();
    len getPixelsSize();
    len getPixelsNumber();
    len getTotalSize();
    len getTotalFileSize();
    pixel* getPixels();
    void processArea(
            void(*painterFunc)(pixel *pix,void *data),
            void *data = nullptr,
            pos startY=0,
            pos startX=0,
            len height=0,
            len width=0);
#pragma pack(push, 1)
    struct header
    {
        const char signature[2] = {'B', 'M'};
        len fileSize;
        len reserved = 0;
        len dataOffset;
    };
    struct info
    {
        len size = 40;
        len width;
        len height;
        uint16_t planes = 1;
        uint16_t bitsPerPixel = 24;
        uint32_t compression = 0;
        len imageSize;
        uint32_t xPixelsPerM;
        uint32_t yPixelsPerM;
        uint32_t colorsUsed = 0;
        uint32_t importantColors = 0;
    };
    struct colorTable
    {
        uint32_t red;
        uint32_t green;
        uint32_t blue;
        uint32_t intent;
        uint32_t iccProfileData;
        uint32_t iccProfileSize;
        uint32_t reserved;
    };
    struct wholeHeader
    {
        header head;
        info inf;
        colorTable colTable;
        colbyte filler[40];
    };
#pragma pack(pop)
    struct pixelPos{
        pos y;
        pos x;
    };
    wholeHeader *getHeader();
    Image(wholeHeader *head,pixel *pixels=nullptr);
    void setPixels(pixel *pixels);
private:
    wholeHeader *head;
    pixel *pixels;
};
