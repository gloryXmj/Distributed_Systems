#include "KTcpProtocol.h"

KTcpProtocol::KTcpProtocol(char *header)
{
    if(header)
        m_headerData.assign(std::begin(header) , std::end(header));
    else
        m_headerData.resize(K_Tcp_Custom_Head_Length, 0);
}

void KTcpProtocol::setSendType(int type)
{

}

int KTcpProtocol::sendType()
{

}

void KTcpProtocol::setLastPackage(bool lastone)
{

}

bool KTcpProtocol::lastPackage()
{

}

void KTcpProtocol::setPackageIndex(size_t index)
{

}

size_t KTcpProtocol::packageIndex()
{

}
