#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "KTcpClient.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    static KTcpClient client;
    static KTcpClient client1;
    static KTcpClient client2;
    static KTcpClient client3;

    connect(ui->btnTestSendImage,&QPushButton::clicked,this,[=](){
        client.sendImage();
        client1.sendImage();
        client2.sendImage();
        client3.sendImage();

    },Qt::QueuedConnection);
}

MainWindow::~MainWindow()
{
    delete ui;
}
