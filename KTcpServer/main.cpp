#include <iostream>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <opencv2/opencv.hpp>


using namespace std;
#define TCP_Listening_Port 9001

int main()
{
    std::vector<char> imageData;
    imageData.resize(192*1600*3);
    const int  EPOLL_EVENT_SIZE = 20;
    char buff[65535];
    int socketFd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    sockaddr_in sockAddr{};
    sockAddr.sin_port = htons(TCP_Listening_Port);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_addr.s_addr = htons(INADDR_ANY);

    if(bind(socketFd,(sockaddr*)&sockAddr,sizeof(sockAddr))==-1){
        std::cout << "bind error" <<std::endl;
        return -1;
    }
    if(listen(socketFd,10)==-1)
    {
        std::cout  << "listen error" <<std::endl;
        return -1;
    }
    int epollFd = epoll_create(1);
    epoll_event epev{};
    epev.events = EPOLLIN;
    epev.data.fd = socketFd;
    epoll_ctl(epollFd,EPOLL_CTL_ADD,socketFd,&epev);
    epoll_event events[EPOLL_EVENT_SIZE];
    while (true) {
        int eNum = epoll_wait(epollFd, events, EPOLL_EVENT_SIZE, -1);

        if (eNum == -1) {
            std::cout << "epoll_wait" << std::endl;
            return -1;
        }
        //遍历所有的事件
        for (int i = 0; i < eNum; i++) {
            //判断这次是不是socket可读(是不是有新的连接)
            if (events[i].data.fd == socketFd) {
                if (events[i].events & EPOLLIN) {
                    sockaddr_in cli_addr{};
                    socklen_t length = sizeof(cli_addr);
                    //接受来自socket连接
                    int fd = accept(socketFd, (sockaddr *) &cli_addr, &length);
                    if (fd > 0) {
                        //设置响应事件,设置可读和边缘(ET)模式
                        //很多人会把可写事件(EPOLLOUT)也注册了,后面会解释
                        epev.events = EPOLLIN | EPOLLET;
                        epev.data.fd = fd;
                        //设置连接为非阻塞模式
                        int flags = fcntl(fd, F_GETFL, 0);
                        if (flags < 0) {
                            std::cout << "set no block error, fd:" << fd << std::endl;
                            continue;
                        }
                        if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0) {
                            std::cout << "set no block error, fd:" << fd << std::endl;
                            continue;
                        }
                        //将新的连接添加到epoll中
                        epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &epev);
                        std::cout << "client on line fd:" << fd << std::endl;
                    }
                }
            } else {//不是socket的响应事件

                //判断是不是断开和连接出错
                //因为连接断开和出错时,也会响应`EPOLLIN`事件
                if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP)
                {
                    //出错时,从epoll中删除对应的连接
                    //第一个是要操作的epoll的描述符
                    //因为是删除,所有event参数天null就可以了
                    epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                    std::cout << "client out fd:" << events[i].data.fd << std::endl;
                    close(events[i].data.fd);
                } else if (events[i].events & EPOLLIN) {//如果是可读事件

                    //如果在windows中,读socket中的数据要用recv()函数
                    int len = read(events[i].data.fd, buff, sizeof(buff));
                    //如果读取数据出错,关闭并从epoll中删除连接
                    if (len == -1) {
                        epoll_ctl(epollFd, EPOLL_CTL_DEL, events[i].data.fd, nullptr);
                        std::cout << "client out fd:" << events[i].data.fd << std::endl;
                        close(events[i].data.fd);
                    }
                    else
                    {
                        //正常读取,打印读到的数据
//                        std::cout << buff << std::endl;
                        //向客户端发数据
                        char a[100];
                        if(len < 1000)
                            sprintf(a,"recv buff : %s",buff);
                        else
                            sprintf(a,"recv buff  size: %05d",len);
                        //如果在windows中,向socket中写数据要用send()函数
                        write(events[i].data.fd, a, sizeof(a));
                        static  int packageSize = 0;
                        if(len > 1000 )
                        {
                            std::cout << "package Size :" <<  len << std::endl;
                            memcpy(imageData.data()+packageSize,buff,len);
                            packageSize += len;
                            std::cout << "Recv package " << packageSize <<std::endl;
                            if(packageSize == 192*1600*3)
                            {
                                cv::Mat image(192,1600,CV_8UC3,imageData.data());
                                cv::imwrite("recv.png",image);
                                cv::imshow("transmissionImage" , image);
                                cv::waitKey();
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    std::cout << "Hello World!" << std::endl;
    return 0;
}
