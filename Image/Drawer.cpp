#include "include/Drawer.h"
void Drawer::drawEllipse(Image *image,pos x,pos y,len height,len width)
{
    len powHeight=height*height;
    len powWidth=width*width;
    circleData *data=new circleData;
    data->j=x;
    data->i=y;
    data->powHeight=powHeight;
    data->powWidth=powWidth;
    image->processArea(Drawer::fillCirclePixel,data,y-height,x-width,height*2,width*2);
}
void Drawer::fillCirclePixel(pixel *pix,void *data)
{
    circleData *info=static_cast<circleData*>(data);
    double x = pow(long (info->pixPos.x) - long(info->j),2) / info->powWidth;
    double y = pow(long(info->pixPos.y) - long(info->i),2) / info->powHeight;
    if((x+y)<=1)
    {
        pix->b=100;
    }
}
void Drawer::paintTones(Image *image,unsigned int tonesNumber,pos x,pos y,len height,len width)
{
    if(height==0)
    {
        height=image->getHeight();
    }
    if(width==0)
    {
        width=image->getWidth();
    }
    colUsageData *colorsUsageNumbers=initColUsgDataArr();
    toneProcessorData *data=new toneProcessorData{Image::pixelPos{0,0},colorsUsageNumbers};
    image->processArea(incrColNumbUsage,data);
    qsort(colorsUsageNumbers,256*256*256,sizeof (*colorsUsageNumbers),cmpfunc);
    int lowerColor=0;
    while(colorsUsageNumbers[lowerColor].usgNum>0)
    {
        lowerColor++;
    }
    chNeColData *CNCDData=new chNeColData;
    CNCDData->lowerCol=lowerColor;
    CNCDData->tpData=data;
    CNCDData->changePixFlag=false;
    for(int i=lowerColor;i>tonesNumber;i--)
    {
        CNCDData->lowerCol=i-1;
        chooseNearestColor(&(colorsUsageNumbers[i].pix),CNCDData);
        colorsUsageNumbers[CNCDData->retVal].usgNum+=colorsUsageNumbers[i].usgNum;
        colorsUsageNumbers[i].usgNum=0;
        for(int k=CNCDData->retVal;k>0;k--)
        {
            if(colorsUsageNumbers[k].usgNum>colorsUsageNumbers[k-1].usgNum)
            {
                swap(colorsUsageNumbers[k],colorsUsageNumbers[k-1]);
            }
            else
            {
                break;
            }
        }
    }
    CNCDData->lowerCol=tonesNumber;
    CNCDData->changePixFlag=true;
    image->processArea(chooseNearestColor,CNCDData,y,x);
}
void Drawer::incrColNumbUsage(pixel *pix,void *data)
{
    toneProcessorData *tpData=static_cast<toneProcessorData*>(data);
    tpData->data[getPixCode(*pix)].usgNum++;
}
colLocation Drawer::getPixCode(pixel &pix)
{
    return (pix.r*256*256)+(pix.g*256)+pix.b;
}
Drawer::colUsageData *Drawer::initColUsgDataArr()
{
    colUsageData *colorsUsageData=new colUsageData[256*256*256];
    int maxCol=256;
    for(int r=0;r<maxCol;r++)
    {
        for(int g=0;g<maxCol;g++)
        {
            for(int b=0;b<maxCol;b++)
            {
                pixel pix={(colbyte)r,(colbyte)g,(colbyte)b};
                colLocation pixCode=getPixCode(pix);
                colorsUsageData[pixCode].pix=pix;
                colorsUsageData[pixCode].usgNum=0;
            }
        }
    }
    return colorsUsageData;
}
int Drawer::cmpfunc (const void * a, const void * b) {
    colUsageData *dataA=(colUsageData*)(a);
    colUsageData *dataB=(colUsageData*)(b);
    return (  dataB->usgNum - dataA->usgNum );
}
Drawer::colDiff Drawer::pixDiff(pixel &a,pixel &b)
{
    colDiff diff=abs(static_cast<short>(a.b)-static_cast<short>(b.b));
    diff+=abs(static_cast<short>(a.g)-static_cast<short>(b.g));
    diff+=abs(static_cast<short>(a.r)-static_cast<short>(b.r));
    return diff;
}
void Drawer::chooseNearestColor(pixel *pix,void *data)
{
    chNeColData *CNCDData=static_cast<chNeColData*>(data);
    colDiff smallestDiff=256*3;
    unsigned int minDiffIndex=0;
    for(unsigned int i=0;(CNCDData->tpData->data[i].usgNum>0) && (i<CNCDData->lowerCol);i++)
    {
        colDiff diff = pixDiff(*pix,CNCDData->tpData->data[i].pix);
        if(diff < smallestDiff)
        {
            smallestDiff = diff;
            minDiffIndex = i;
        }
    }
    if(CNCDData->changePixFlag)
    {
        (*pix)=CNCDData->tpData->data[minDiffIndex].pix;
    }
    CNCDData->retVal= minDiffIndex;
}
void Drawer::processExcept(Image &img,pixel pix,colDiff maxDiff)
{
    paintWBExceptData *data=new paintWBExceptData;
    data->pix=pix;
    data->maxDiff=maxDiff;
    img.processArea(paintWhiteBlackExcept,data);
}
void Drawer::paintWhiteBlackExcept(pixel *pix,void *data)
{
    paintWBExceptData *pwbdata=static_cast<paintWBExceptData*>(data);
    if(pixDiff(*pix,pwbdata->pix)>(pwbdata->maxDiff))
    {
        PixelPainter::makeGrey(pix,nullptr);
    }
}
