#include "include/ImgStreamer.h"
void ImgStreamer::write(Image *img,string name)
{
    ofstream stream(name, ios::binary);
    writeImage(img,writeImageDataInStream,&stream);
    stream.close();
}
void ImgStreamer::writeImage(Image *img,void (*writeData)(char *data,unsigned int length,void *additionalInfo),void *additionalInfo,bool withoutPadding)
{
    writeData((char *)img->getHeader(),sizeof (*img->getHeader()),additionalInfo);
    pixel *pixels=img->getPixels();
    len width=img->getWidth();
    len height=img->getHeight();
    len padding=4 - ((width*sizeof(pixel))%4);
    if((padding == 4) || withoutPadding)
    {
        writeData((char*)pixels,img->getPixelsSize(),additionalInfo);
    }
    else
    {
        char *padBuf=new char[padding];
        for(len row=0;row<height;row++)
        {
            writeData((char *)&pixels[row*width],width*sizeof(pixel),additionalInfo);
            writeData(padBuf,padding,additionalInfo);
        }
    }
}
Image *ImgStreamer::load(string name)
{
    ifstream stream;
    stream.open(name, ios::binary);
    Image *img=loadImage(loadData,&stream,false);
    stream.close();
    return img;
}
Image *ImgStreamer::loadImage(void (*loadData)(char *buf,unsigned int length,void *additionalData),void *additionalData,bool withoutPadding)
{
    Image::wholeHeader *head=new Image::wholeHeader;
    loadData((char*)head, sizeof(*head),additionalData);
    Image *img=new Image(head);
    pixel *pixels=(pixel*)new colbyte[head->inf.imageSize];
    len width=img->getWidth();
    len height=img->getHeight();
    len padding=4 - ((width*sizeof(pixel))%4);
    if((padding == 4) || withoutPadding)
    {
        loadData((char*)pixels, img->getPixelsSize(),additionalData);
    }
    else
    {
        char *padBuf=new char[padding];
        for(len row=0;row<height;row++)
        {
            loadData((char *)&pixels[row*width], width*sizeof(pixel),additionalData);
            loadData(padBuf, padding,additionalData);
        }
    }
    img->setPixels(pixels);
    return img;
}
void ImgStreamer::writeImageDataInStream(char *data,unsigned int length,void *additionalInfo)
{
    ofstream *stream=static_cast<ofstream*>(additionalInfo);
    stream->write(data,length);
}
void ImgStreamer::loadData(char *buf,unsigned int length,void *additionalData)
{
    ifstream *stream=static_cast<ifstream*>(additionalData);
    stream->read(buf,length);
}
