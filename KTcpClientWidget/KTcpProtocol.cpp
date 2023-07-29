#include "KTcpProtocol.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

enum Oper_Address
{
    Oper_Address_Checkdata  = 0 ,            // 4B  check data
    Oper_Address_SendType   = 4 ,            // 1B  send type
    Oper_Address_PackageIndex = 5,           // 2B  packageIndex
    Oper_Address_LastPackageFlag = 7,        // 1B  last package index
};

KTcpProtocol::KTcpProtocol(char *header)
{
    m_headerData.resize(K_Tcp_Custom_Head_Length, 0);
    if(header)
        memcpy(m_headerData.data(),header,K_Tcp_Custom_Head_Length);
}

void KTcpProtocol::setSendType(int type)
{
    if(type <  Type_Begin || type >= Type_End)
        perror("set Send type error");
    m_headerData[Oper_Address_SendType] = type &0xFF;
}

int KTcpProtocol::sendType()
{
    return m_headerData[Oper_Address_SendType]&0xFF;
}

void KTcpProtocol::setLastPackage(bool lastone)
{
    m_headerData[Oper_Address_LastPackageFlag] = lastone &0x01;
}

bool KTcpProtocol::lastPackage()
{
    return  m_headerData[Oper_Address_LastPackageFlag]&0x01;
}

void KTcpProtocol::setPackageIndex(size_t index)
{
    m_headerData[Oper_Address_PackageIndex] = index &0xFFFF;
}

size_t KTcpProtocol::packageIndex()
{
    return m_headerData[Oper_Address_PackageIndex] & 0xFFFF;
}
