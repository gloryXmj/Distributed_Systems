#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QMap>
#include <QTimer>
#include <QLabel>
#include <opencv2/opencv.hpp>
#include "KTcpServer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QWidget* initWidget();
    QLayout *createDisplay(int mapedFd);

private slots:
    void addDisplayWidget(int Fd);
    void removeDisplayWidget(int Fd);
    void updateDisplayImage(int Fd, const cv::Mat &image);
private:
    Ui::MainWindow *ui;
    QMap<int,QWidget *> m_displayWidgetMap;
    QMap<int,QLabel *> m_displayLabelMap;

    QGridLayout *m_mainLayout = nullptr;
    KTcpServer *m_tcpServer = nullptr;
    cv::Mat m_curImage;
    QTimer m_timerFrame;
    std::map<int,int> m_frameCurRatio , m_frameLastRatio;
};

#endif // MAINWINDOW_H
