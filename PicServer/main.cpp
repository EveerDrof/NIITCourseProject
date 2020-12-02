#include <iostream>
#include <fstream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <thread>
#include <getopt.h>
#include <Loader.h>
#include <Image.h>
#include <ImgStreamer.h>
#include <PixelPainter.h>
#include <Drawer.h>
using namespace boost::asio;
using namespace std;
void help()
{
    cout<<"Usage : nothing for default";
    cout<<"or : ipaddress port";
    cout<<"Example 127.0.0.1 9000";
    cout<<"-h for help";
}
void serveClient(ip::tcp::socket *sock)
{
    Loader::operation oper;
    Loader::receiveVar(oper,*sock);
    Image *img=Loader::downloadImage(*sock);
    switch (oper) {
    case MAKE_WHITE_BLACK:
    {
        img->processArea(PixelPainter::makeGrey);
        break;
    }
    case REDUCE_TONES:
    {
        unsigned int tonesNumber;
        Loader::receiveVar(tonesNumber,*sock);
        Drawer::paintTones(img,tonesNumber);
        break;
    }
    case WB_EXCEPT_NEAR:
    {
        pixel pix;
        Loader::receiveVar(pix,*sock);
        unsigned short maxDIff;
        Loader::receiveVar(maxDIff,*sock);
        Drawer::processExcept(*img,pix,maxDIff);
        break;
    }
    }
    Loader::uploadData(*img,*sock);
    sock->close();
    delete sock;
}
int main(int argc,char **argv)
{
    if(getopt(argc,argv,"h")=='h')
    {
        help();
        return 0;
    }

    boost::system::error_code err;
    io_context iocon;
    unsigned short port;
    if(argc==2)
    {
        port=atoi(argv[2]);
    }
    else
    {
        port=10000;
    }
    char *ip;
    if(argc>1)
    {
        ip=argv[1];
    }
    else
    {
        ip=new char[3*4];
        sprintf(ip,"%s","127.0.0.1");
    }
    ip::tcp::endpoint endpoint(ip::make_address(ip,err),port);
    ip::tcp::acceptor acc(iocon,endpoint);
    vector<thread*> threads;
    while(true)
    {
        ip::tcp::socket *sock=new ip::tcp::socket(iocon);
        acc.accept(*sock,err);
        //cout<<"Server : Connection accepted\n";
        if(!(!err))
        {
            cout<<"Server : There is an accept error\n";
            continue;
        }
        threads.push_back(new thread(serveClient,sock));
    }
    for(auto thrd : threads )
    {
        thrd->join();
    }
    cout<<"Server : Finish\n";
    return 0;
}
