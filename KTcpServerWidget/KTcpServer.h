#ifndef KTCPSERVER_H
#define KTCPSERVER_H

#include <QObject>
#include <QThread>
#include <opencv2/opencv.hpp>

#define TCP_Listening_Port 9001

class KTcpServer : public QObject
{
    Q_OBJECT
public:
    KTcpServer();
signals:
    void signalClientConnected(int Fd);
    void signalClientDisconnected(int Fd);
    void signalReceImage(int Fd,const cv::Mat &image);
private :
    std::map<int,std::vector<char>> m_imageDatas;
};

#endif // KTCPSERVER_H
