#include "include/Image.h"
Image::Image(len width, len height)
{
    head = new wholeHeader;
    head->inf.height = height;
    head->inf.width = width;
    head->inf.imageSize = (width * height)*sizeof(pixel);
    head->head.fileSize = sizeof(head) + (head->inf.imageSize);
    head->head.dataOffset = sizeof(head);
    pixels = new pixel[head->inf.imageSize];
}

Image::~Image()
{
    delete head;
    delete[] pixels;
}
void Image::reset(len width = 0, len height = 0)
{
    delete[] pixels;
    Image(width, height);
}

len Image::getHeight()
{
    return head->inf.height;
}
len Image::getWidth()
{
    return head->inf.width;
}
pixel *Image::getPixel(pos y, pos x)
{
    if(y>=0 &&y<getHeight() && x<getWidth() && x >=0)
    {
        return &(pixels[(y *getWidth()) + x]);
    }
    else
    {
        throw "Invalid pixel";
    }
}
pixel* Image::getPixels()
{
    return  pixels;
}
void Image::processArea(
        void(*painterFunc)(pixel *pix,void *data),
        void *data,
        pos startY,
        pos startX,
        len height,
        len width)
{
    pos maxY=startY+height;
    if(maxY>getHeight() || maxY == 0)
    {
        maxY=getHeight();
    }
    pos maxX=startX+width;
    if(maxX>getWidth() || maxX == 0)
    {
        maxX=getWidth();
    }
    pixelPos *pixPos;
    if(data!=nullptr)
    {
        pixPos=static_cast<pixelPos *>(data);
    }
    else
    {
        pixPos=new pixelPos;
    }
    for(pos i=startY;i<maxY;i++)
    {
        for(pos j=startX;j<maxX;j++)
        {
            pixPos->y=i;
            pixPos->x=j;
            painterFunc(&(pixels[(i*getWidth())+j]),data);
        }
    }
}
len Image::getPixelsSize()
{
    return getHeight()*getWidth()*sizeof (pixel);
}
Image::wholeHeader *Image::getHeader()
{
    return head;
}
Image::Image(wholeHeader *head,pixel *pixels)
{
    this->head=head;
    this->pixels=pixels;
}
void Image::setPixels(pixel *pixels)
{
    this->pixels=pixels;
}
len Image::getTotalSize()
{
    return getPixelsSize() + sizeof (*head);
}
len Image::getTotalFileSize()
{
    len paddingSize=(4 - (( getWidth() * sizeof (pixel))%4));
    if(paddingSize==4)
    {
        paddingSize=0;
    }
    else
    {
        paddingSize=getHeight()*paddingSize;
    }
    return getTotalSize() + paddingSize;
}
len Image::getPixelsNumber()
{
    return getWidth()*getHeight();
}
