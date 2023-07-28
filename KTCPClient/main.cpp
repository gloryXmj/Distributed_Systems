#include <QCoreApplication>
#include "KTcpClient.h"
#include <opencv2/opencv.hpp>

const int readImageCount = 0;

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    KTcpClient client1;
//    KTcpClient client2;
//    KTcpClient client3;
//    KTcpClient client4;
//    KTcpClient client5;

    return a.exec();
}
