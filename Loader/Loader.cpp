#include "include/Loader.h"
void Loader::uploadData(void *data,bytesNumber fileSize,ip::tcp::socket &sock)
{
    sendVar(fileSize,sock);
    write(sock,buffer(data,fileSize));
}

void *Loader::downloadData(bytesNumber &fileSize,ip::tcp::socket &sock)
{
    receiveVar(fileSize,sock);
    void *data=new char[fileSize];
    read(sock,buffer(data,fileSize));
    return data;
}
Image *Loader::downloadImage(ip::tcp::socket &sock,bool withoutPadding)
{
    bytesNumber fileSize;
    receiveVar(fileSize,sock);
    //cout<<"Data size to download : "<<fileSize<<'\n';
    Image *img=ImgStreamer::loadImage(loadDataFromSock,&sock,withoutPadding);
    return img;
}
void Loader::uploadData(Image &img,ip::tcp::socket &sock,bool withoutPadding)
{
    bytesNumber fileSize;
    if(withoutPadding)
    {
        fileSize=img.getTotalSize();
    }
    else
    {
        fileSize=img.getTotalFileSize();
    }
    sendVar(fileSize,sock);
    ImgStreamer::writeImage(&img,writeDataToSock,&sock,withoutPadding);
}
void Loader::writeDataToSock(char *buf,unsigned int length,void *additionalData)
{
    ip::tcp::socket *sock=static_cast<ip::tcp::socket *>(additionalData);
    write(*sock,buffer(buf,length));
}
void  Loader::loadDataFromSock(char *buf,unsigned int length,void *additionalData)
{
    ip::tcp::socket *sock=static_cast<ip::tcp::socket *>(additionalData);
    read(*sock,buffer(buf,length));
}
void Loader::uploadData(std::ifstream &inputFile,bytesNumber fileSize,ip::tcp::socket &sock)
{
    sendVar(fileSize,sock);
    bytesNumber bufSize=10000;
    char *buf=new char[bufSize];
    unsigned int bytesSent=0;
    do
    {
        inputFile.read(buf,bufSize);
        write(sock,buffer(buf,inputFile.gcount()));
        bytesSent+=inputFile.gcount();
    }
    while(inputFile.gcount()==bufSize);
    //std::cout<<"Data is sent\n";
}
void Loader::uploadData(std::string fileName,ip::tcp::socket &sock)
{
    cout<<"Opening "<<fileName<<'\n';
    ifstream inputFile(fileName,ifstream::binary);
    if(inputFile.fail()==true)
    {
        std::cout<<"Opening error\n";
    }
    uploadData(inputFile,getFileSize(fileName),sock);
    inputFile.close();
}
void Loader::downloadAndWrite(std::string fileName,ip::tcp::socket &sock)
{
    bytesNumber fileSize;
    receiveVar(fileSize,sock);
    //cout<<"File size"<<fileSize<<'\n';
    ofstream outputFile(fileName,ofstream::binary);
    bytesNumber bufSize=10000;
    char *buf=new char[bufSize];
    bytesNumber bytesRed=0;
    do
    {
        bytesNumber bytesToRead=fileSize;
        if(bytesToRead>bufSize)
        {
            bytesToRead=bufSize;
        }
        bytesRed=read(sock,buffer(buf,bytesToRead));
        outputFile.write(buf,bytesRed);
        fileSize-=bytesRed;
    }
    while(fileSize>0);
    outputFile.close();
}
Loader::bytesNumber Loader:: getFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}
