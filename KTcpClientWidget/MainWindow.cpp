#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "KTcpClient.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    static KTcpClient client;
    connect(ui->btnTestSendImage,&QPushButton::clicked,this,[=](){
        client.sendImage();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
