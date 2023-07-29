#ifndef KTCPCLIENT_H
#define KTCPCLIENT_H

#include <unistd.h>
#include <vector>
#include <string>
#include <shared_mutex>
#include <pthread.h>

class KTcpClient
{
public:
    KTcpClient();
    ~KTcpClient();
    void threadSend();
    void threadReceive();
    void sendImage();
    std::vector<std::string> searchDirectory(const std::string &directory, const std::string &suffix);
private:
    void transmissionImageDatas();
    void creatSendData();
    void creatSendData(char *pointer , size_t size);
    void imagePackage();
    size_t Send_Thread_Safety(int __fd, const void *__buf, size_t __n, int __flags);

private :
    int m_socketFd = -1;
    struct sendData
    {
        char *dataPointer;
        size_t dataSize;
    };
    sendData m_sendData;
    mutable std::shared_timed_mutex m_mutex;
};

#endif // KTCPCLIENT_H
