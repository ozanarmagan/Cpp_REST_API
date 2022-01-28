#include "HTTPServer.h"


void HTTPServer::init()
{

#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(0x202,&wsaData);

    if((mSock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) == INVALID_SOCKET)
#else
    if((mSock = socket(AF_INET,SOCK_STREAM,0)) < 0)
#endif
    {
        std::cout << "Error while initilazing socket\n";
    }

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_port = htons(mPort);
#ifdef _WIN32
    mServerAddr.sin_addr.S_un.S_addr = INADDR_ANY;
#else
    mServerAddr.sin_addr.S_addr = INADDR_ANY;
#endif

    if(bind(mSock, (struct sockaddr*) &mServerAddr, sizeof(mServerAddr)) < 0)
    {
        std::cout << "Error while binding socket\n";
    }

    if(listen(mSock, SOMAXCONN))
    {
        std::cout << "Error while listening socket\n";
    }

}


const string HTTPServer::fRecieveNext(uint64_t socket)
{

    int64_t recieveLength = 0,totalRecieved = 0; 
    int64_t recieveLengthBeforeBody = -1;
    int contentLength = 0;
    string rawData;
    while(1)
    {
        memset(mBuffer,'\0',8192);

        if((recieveLength = recv(socket,mBuffer,8192, 0)) == SOCKET_ERROR)
        {
            std::cout << "Error while receiving data from socket\n";
            std::cout << WSAGetLastError() << std::endl;
        }


        totalRecieved += recieveLength; 
        rawData += string(mBuffer,recieveLength);

        if(totalRecieved > 32)
        {
            string temp = string(mBuffer);
            if(temp.find("Content-Length") == string::npos && contentLength == 0)
                break;
            else
            {
                if(contentLength == 0)
                {
                    auto pos = temp.find("Content-Length:");
                    auto lengthStr = temp.substr(pos + 16, temp.find_first_of("\r",pos) - pos - 16);
                    contentLength = std::stoi(lengthStr);
                }
                if(temp.find("\r\n\r\n") != string::npos && recieveLengthBeforeBody == -1)
                {
                    recieveLengthBeforeBody = temp.find("\r\n\r\n") + 4;
                }

                if(totalRecieved >= contentLength + recieveLengthBeforeBody)
                    break;
            }
        }
    }

    return rawData;
}


void HTTPServer::fRun()
{
    mAcceptSocket = socket(AF_INET,SOCK_STREAM,0);
    while(1)
    {
        
        int size = sizeof(mClientAddr);
#ifdef _WIN32
        if((mAcceptSocket = accept(mSock,(struct sockaddr*) &mClientAddr,&size)) == INVALID_SOCKET)
#else
        if((mAcceptSocket = accept(mSock,(struct sockaddr*) &mClientAddr,&size))) < 0)
#endif
    {
        std::cout << "Error while initilazing accept socket\n";
    }


    thread t1([this] { this->fOnRequest(mAcceptSocket); });
    t1.join();

    }
}


void HTTPServer::fOnRequest(uint64_t socket)
{
    const string reqData = fRecieveNext(mAcceptSocket);
    const HTTPResponse res = fProcessRequest(reqData);
    fSendResponse(res,socket);
}

 const HTTPResponse HTTPServer::fProcessRequest(const string& rawData)
{   
    HTTPRequest* req = new HTTPRequest(rawData);
    HTTPResponse* res = new HTTPResponse();
    res->fSetHTTPVersion(req->fGetHTTPVersion());
    res->fSetStatus(200);
    res->fAddHeader("Server","RESTC++");
    time_t currTime;
    time(&currTime);
    string timeStr = ctime(&currTime);
    std::replace(timeStr.begin(),timeStr.end(),'\n','\0');
    timeStr = timeStr.substr(0, timeStr.find('\0'));
    res->fAddHeader("Date",timeStr);
    res->fAddHeader("Connection","Close");

    if(mRouter != nullptr)
    {
        auto pos1 = req->fGetPath().find_first_of("/");
        auto pos2 = req->fGetPath().find_last_of("/");
        string URLPath = req->fGetPath().substr(pos1, pos2 - pos1 + 1);
        string fileName = req->fGetPath().substr(pos2 + 1);
        bool foundStatic = false;
        for(auto& [key,value] : mRouter->mStaticRoutes)
        {
            if(URLPath == key)
            {
                foundStatic = true;
                
                ifstream file(value + fileName);
                bool checkFile = file.good();
                file.close();
                if(file.good())
                    res->fSetBodyFile(value + fileName);
                else
                {
                    res->fSetStatus(404);
                    res->fSetBody("<html><h3>404</h3><br/>File Could Not Found</html>");
                }
                break;
            }
        }

        if(foundStatic)
        {
            delete req;
            return *res;
        }

        for(auto& route : mRouter->mRoutes)
        {
            if(std::get<0>(route) == (URLPath + fileName) && std::get<1>(route) == req->fGetMethod())
            {
                std::get<2>(route)(*req,*res);
                break;
            }
        }
    }

    delete req;
    return *res;
}


void HTTPServer::fSendResponse(const HTTPResponse& response,const uint64_t socket)
{
    string resStr = response.fSerializeResponse();
    size_t sent = 0,totalSent = 0;
    char* buffer = &resStr[0];

    while(totalSent < resStr.length())
    {
        if((sent = send(socket, buffer, resStr.length() - totalSent,0)) < 0)
        {
            std::cout << "Error while sending response" << std::endl;
        }

        
        buffer += sent;
        totalSent += sent;
    }

    delete &response;
    closesocket(socket);
}

