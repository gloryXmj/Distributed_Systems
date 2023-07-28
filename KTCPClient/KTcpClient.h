#ifndef KTCPCLIENT_H
#define KTCPCLIENT_H


class KTcpClient
{
public:
    KTcpClient();
    ~KTcpClient();
    void threadSend();
    void threadReceive();
private :
    int m_socketFd = -1;
};

#endif // KTCPCLIENT_H
