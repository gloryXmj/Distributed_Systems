#include "KTcpClient.h"
#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <chrono>
#include <opencv2/opencv.hpp>

static int  clientId = 0;
const int MAX_EVENTS = 1024;
KTcpClient::KTcpClient()
{
    /// socket的建立
    m_socketFd = socket(AF_INET , SOCK_STREAM ,IPPROTO_TCP);

    if (m_socketFd == -1){
        printf("Fail to create a socket.");
        return ;
    }
    //socket的連線

    //    sprintf(buffer,"start %2d",clientId);
    //    send(m_socketFd, buffer, sizeof(buffer),MSG_DONTWAIT);
    auto func =  [=](){

        struct sockaddr_in info;
        bzero(&info,sizeof(info));
        info.sin_family = PF_INET;

        //localhost test
        info.sin_addr.s_addr = inet_addr("192.168.129.101");
        //    info.sin_addr.s_addr = inet_addr("127.0.0.1");
        info.sin_port = htons(9001);

        int epollFd = epoll_create1(0);
        if(epollFd==-1)
        {
            perror("epoll_create1(0)");
            return;
        }
        epoll_event event,events[MAX_EVENTS];
        event.events = EPOLLOUT |EPOLLIN | EPOLLET;
        event.data.fd = m_socketFd;

        int err = connect(m_socketFd,(struct sockaddr *)&info,sizeof(info));
        if(err==-1){
            printf("Connection error");
            return ;
        }

        int flags = fcntl(m_socketFd,F_GETFL,0);
        fcntl(m_socketFd,F_SETFL,flags|O_NONBLOCK);

        if(epoll_ctl(epollFd,EPOLL_CTL_ADD,m_socketFd,&event) == -1)
        {
            perror("epoll_ctl(EPOLL_CTL_ADD) failed");
            return ;
        }
        char buffer[100];
        memset(buffer,0,sizeof(buffer));
        clientId++;
        int curClinetId = clientId;
        int messageIndex = 0;
        while(true)
        {
            int nfds = epoll_wait(epollFd,events,MAX_EVENTS,-1);
            if(nfds == -1)
            {
                std::cout << "epoll_wait" << std::endl;
                return ;
            }
            for(int i = 0; i < nfds; i++) {
                int sockfd = events[i].data.fd;
                if ((events[i].events & EPOLLOUT) && (sockfd == m_socketFd)) {
                    messageIndex++;
                    sprintf(buffer,"this is client  %02d message Index %02d \0",curClinetId, messageIndex);
                    ssize_t len = send(sockfd, buffer, sizeof(buffer),0);
                    if (len == -1) {
                        perror("send() failed");
                        continue;
                    }
                    memset(buffer, 0, sizeof(buffer));
                }
                if ((events[i].events & EPOLLIN) && (sockfd == m_socketFd)) {
                    ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);
                    if (len == -1) {
                        perror("recv() failed");
                        continue;
                    }
                    std::cout << "Client Recv Message : "<< buffer << std::endl;
                }
            }
        }
    };
    std::thread epollThread(func);
    epollThread.detach();
    std::thread sender(&KTcpClient::threadSend ,this);
    sender.detach();
    //    std::thread recv(&KTcpClient::threadReceive ,this);
    //    recv.detach();
}

KTcpClient::~KTcpClient()
{
    //    close(epollfd);
    close(m_socketFd);
    printf("close Socket\n");
}

void KTcpClient::threadSend()
{
    cv::Mat imageRead =  cv::imread("111.png");

    //Send a message to server
    char message[64];
    sprintf(message,"Actively send %02d \0",clientId);
    int packageSize = (9000 - 20 - 20);
    int imageDataSize = imageRead.cols*imageRead.rows*imageRead.dims;
    int packageCount = imageDataSize /packageSize + 1; /// MTU 9000 IP_header 20  TCP_header 20

    while(true)
    {
        for(int packageIndex = 0 ; packageIndex < packageCount;packageIndex ++ )
        {
            auto pre = std::chrono::high_resolution_clock::now();
            std::cout << send(m_socketFd,imageRead.data+packageSize*packageIndex,packageSize,MSG_DONTWAIT) <<std::endl;
            usleep(100000);
            auto now =  std::chrono::high_resolution_clock::now();
            std::cout << "send Time  "<< std::chrono::duration_cast<std::chrono::microseconds>(now-pre).count() << std::endl;
        }
    }
}

void KTcpClient::threadReceive()
{
    while ( true) {
        char receiveMessage[100] = {};
        std::cout << recv(m_socketFd,receiveMessage,sizeof(receiveMessage),MSG_DONTWAIT) <<std::endl;
        std::cout << receiveMessage << std::endl;
        usleep(100);
    }
}
