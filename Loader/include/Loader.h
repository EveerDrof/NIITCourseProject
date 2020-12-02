#pragma once
#include <boost/asio.hpp>
#include <Image.h>
#include <ImgStreamer.h>
#include <fstream>
#include <iostream>

#define MAKE_WHITE_BLACK 1
#define REDUCE_TONES 2
#define WB_EXCEPT_NEAR 3
#define SET_SERVER_ADDR 126

using namespace boost::asio;
class Loader
{
public:
    typedef unsigned long bytesNumber;
    typedef char operation;
    static void uploadData(void *data,bytesNumber fileSize,ip::tcp::socket &sock);
    static void uploadData(Image &img,ip::tcp::socket &sock,bool withoutPadding=false);
    static void uploadData(std::ifstream &inputFile,bytesNumber fileSize,ip::tcp::socket &sock);
    static void uploadData(std::string fileName,ip::tcp::socket &sock);
    static void *downloadData(bytesNumber &fileSize,ip::tcp::socket &sock);
    static Image *downloadImage(ip::tcp::socket &sock,bool withoutPadding=false);
    static void downloadAndWrite(std::string fileName,ip::tcp::socket &sock);
    template<class T>
    static void sendVar(T &var,ip::tcp::socket &sock);
    template<class T>
    static void receiveVar(T &var,ip::tcp::socket &sock);

    static void writeDataToSock(char *buf,unsigned int length,void *additionalData);
    static void loadDataFromSock(char *buf,unsigned int length,void *additionalData);
private:

    static bytesNumber getFileSize(std::string filename);
};



template<class T>
void Loader::sendVar(T &var,ip::tcp::socket &sock)
{
    write(sock,buffer(&var,sizeof (var)));
}

template<class T>
void Loader::receiveVar(T &var,ip::tcp::socket &sock)
{
    read(sock,buffer(&var,sizeof (var)));
}
