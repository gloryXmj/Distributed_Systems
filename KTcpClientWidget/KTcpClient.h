#ifndef KTCPCLIENT_H
#define KTCPCLIENT_H

#include <unistd.h>

class KTcpClient
{
public:
    KTcpClient();
    ~KTcpClient();
    void threadSend();
    void threadReceive();
    void sendImage();
private:
    void transmissionImageDatas();
    void creatSendData();
    void creatSendData(char *pointer , size_t size);
    void imagePackage();
private :
    int m_socketFd = -1;
    struct sendData
    {
        char *dataPointer;
        size_t dataSize;
    };
    sendData m_sendData;
};

#endif // KTCPCLIENT_H
