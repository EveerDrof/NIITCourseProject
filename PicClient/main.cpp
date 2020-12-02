#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <getopt.h>

#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include <Loader.h>
using namespace boost::asio;
using namespace std;

void help()
{
    cout<<"Usage : serverIp port inputfile.bmp args outputfile.bmp\n";
    cout<<"127.0.0.1 10000 --wb inputfile.bmp outputfile.bmp";
    cout<<"127.0.0.1 10000 --reduce inputfile.bmp 10 outputfile.bmp";
    cout<<"127.0.0.1 10000 --wbExcept inputfile.bmp 10 10 10 10 outputfile.bmp";
}

int main(int argc,char** argv)
{
    if(argc < 4)
    {
        help();
        return 0;
    }
    char const * const args="wb:";
    int opt_index=0;
    option options[]={
        {"wb",required_argument,0,MAKE_WHITE_BLACK},
        {"reduce",required_argument,0,REDUCE_TONES},
        {"wbExcept",required_argument,0,WB_EXCEPT_NEAR},
        {0,0,0,0}
    };
    Loader::operation oper=getopt_long(argc,argv,args,options,&opt_index);
    if(oper==-1)
    {
        cout<<"Client :Returning\n";
        return 0;
    }
    boost::system::error_code err;
    char *serverIp=argv[1];
    unsigned short port=atoi(argv[2]);
    ip::tcp::endpoint endpoint(ip::make_address(serverIp,err),port);
    io_context iocon;
    ip::tcp::socket sock(iocon);
    sock.connect(endpoint,err);
    if(!err)
    {
        //cout<<"Client : Connected\n";
    }
    else
    {
        cout<<"Client : Connect error : "<<err.message()<<'\n';
        return -1;
    }
    Loader::sendVar(oper,sock);
    Loader::uploadData(argv[4],sock);
    switch (oper) {
    case REDUCE_TONES:
    {
        unsigned int numberOfTones=atoi(argv[5]);
        Loader::sendVar(numberOfTones,sock);
        break;
    }
    case WB_EXCEPT_NEAR:
    {
        pixel pix;
        pix.r=atoi(argv[5]);
        pix.g=atoi(argv[6]);
        pix.b=atoi(argv[7]);
        unsigned short maxDIff=atoi(argv[8]);
        Loader::sendVar(pix,sock);
        Loader::sendVar(maxDIff,sock);
    }
    }
    Loader::downloadAndWrite(argv[argc-1],sock);
    sock.close();
    //cout<<"Client : Finish";
    return 0;
}
