//
// Created by 55044 on 2021/6/2.
//
#include "../allLibs/allLibs.h"

#ifndef DNS_LOCALDNSSERVER_H
#define DNS_LOCALDNSSERVER_H

class LocalDnsServer {
public:
    bool createSocket() {
        WSADATA wsaData;
        // 初始化Socket
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {                                        //初始化was
            printf("WSAStartup初始化失败！%d\n", GetLastError());
            exit(-1);
        }

        if (serverSocket < 0) {
            printf("创建本地DNS Server Socket套接字失败\n");
            exit(1);
        }

        SOCKADDR_IN localDnsAddr;
        localDnsAddr.sin_family = AF_INET;            /* Set the family as AF_INET (TCP/IP)，AF_INET代表IPV4internet protocols */
        localDnsAddr.sin_addr.s_addr = INADDR_ANY;    /* Set to any 监听任意的地址的53端口*/
        localDnsAddr.sin_port = htons(53);           /* Set the port as DNS port (53)，dns专用的端口 */
        //htons函数：将整型变量从主机字节顺序转变成网络字节顺序
        //网络字节顺序是TCP/IP中规定好的一种数据表示格式，它与具体的CPU类型、操作系统等无关，
        //从而可以保证数据在不同主机之间传输时能够被正确解释，网络字节顺序采用big-endian排序方式


        // Server Socket监听53号端口
        if (bind(serverSocket, (struct sockaddr *) &localDnsAddr, sizeof(localDnsAddr)))//连接套接字与端口，开始收信息
        {
            printf("Bind Port 53 失败. %d\n", GetLastError());
            exit(1);
        }
    }

private:
    SOCKET serverSocket;

};

#endif //DNS_LOCALDNSSERVER_H
