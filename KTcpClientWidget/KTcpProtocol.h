#ifndef KTCPPROTOCOL_H
#define KTCPPROTOCOL_H
#include <unistd.h>
#include <vector>


/*
 * 4B  check data
 * 1B  send type
 * 2B  packageIndex
 * 1B  last package index
 */

#define K_Tcp_Custom_Head_Length 24
enum Type_Flags
{
    Type_Begin,
    Type_Message = Type_Begin ,
    Type_Transmission_Files,
    Type_Execute_Cmd,
    Type_End,
};




class KTcpProtocol
{
public:
    KTcpProtocol(char * header = nullptr);
    void setSendType(int type);
    int sendType();
    void setLastPackage(bool lastone);
    bool lastPackage();
    void setPackageIndex(size_t index);
    size_t packageIndex();
private:
    std::vector<char> m_headerData;
};

#endif // KTCPPROTOCOL_H
