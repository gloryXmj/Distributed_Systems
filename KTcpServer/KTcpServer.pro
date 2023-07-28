#在windows平台上使用IOCP
#在linux平台下使用epoll 
TEMPLATE = app
CONFIG += c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


unix{
    CONFIG      += link_pkgconfig
    PKGCONFIG   += opencv4
}
