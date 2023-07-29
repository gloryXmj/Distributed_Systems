#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "gui/KImageDispPanel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(initWidget());
    m_tcpServer = new KTcpServer();
    connect(m_tcpServer,&KTcpServer::signalClientConnected,this,&MainWindow::addDisplayWidget,Qt::QueuedConnection);
    connect(m_tcpServer,&KTcpServer::signalClientDisconnected,this,&MainWindow::removeDisplayWidget,Qt::QueuedConnection);
    connect(m_tcpServer,&KTcpServer::signalReceImage,this,&MainWindow::updateDisplayImage);
    connect(&m_timerFrame,&QTimer::timeout,this,[=](){
        for(auto iter  = m_frameCurRatio.begin();iter != m_frameCurRatio.end(); iter++)
        {
            int index = (*iter).first;
            std::cout << "Recv " << index << " Image Count : "  <<  (*iter).second  <<std::endl;
            //   std::cout << index << " frame ratio " << (*iter).second - m_frameLastRatio[index] <<std::endl;
            m_displayLabelMap[index]->setText(QString::number((*iter).second - m_frameLastRatio[index]));
            m_frameLastRatio[index] = m_frameCurRatio[index];
        }
    });
    m_timerFrame.start(10000);
}

MainWindow::~MainWindow()
{
    delete ui;
}


QWidget* MainWindow::initWidget()
{
    QWidget *wgtMain = new QWidget(this);
    m_mainLayout = new QGridLayout(wgtMain);
    //m_mainLayout->addLayout(createDisplay(0),0,0,1,1);
    wgtMain->setLayout(m_mainLayout);
    return wgtMain;
}

QLayout *MainWindow::createDisplay(int mapedFd)
{
    QVBoxLayout *vlytDisplay =  new QVBoxLayout();
    KImageDispPanel *displaySence = new KImageDispPanel();
    vlytDisplay->addWidget(displaySence);
    QHBoxLayout *hlytDisplay =  new QHBoxLayout();
    QLabel *label = new QLabel("frame ratio :",this);
    QLabel *frameRatio = new QLabel("00",this);
    hlytDisplay->addWidget(label);
    hlytDisplay->addWidget(frameRatio);
    vlytDisplay->addLayout(hlytDisplay);
    m_displayLabelMap[mapedFd] = frameRatio;
    m_displayWidgetMap[mapedFd] = displaySence;
    return vlytDisplay;
}

void MainWindow::addDisplayWidget(int Fd)
{
    QLayout *lay = createDisplay(Fd);
    int count = m_mainLayout->count();
    if(count < 6 )
    {
        m_mainLayout->addLayout(lay, count /2 , count % 2 , 1 , 1);
    }
    else
    {
        m_mainLayout->addLayout(lay, count /4 , count % 4 , 1 , 1);
    }
    m_frameCurRatio[Fd] = 0;
    m_frameLastRatio[Fd] = 0;
    this->update();
}

void MainWindow::removeDisplayWidget(int Fd)
{

}

void MainWindow::updateDisplayImage(int Fd, const cv::Mat &image)
{
    m_curImage = image;
    KImageDispPanel *panel = dynamic_cast<KImageDispPanel*>(m_displayWidgetMap[Fd]);
    if(panel)
    {
        panel->dispImage(m_curImage.data,m_curImage.cols,m_curImage.rows);
        m_frameCurRatio[Fd] += 1;
    }
}

