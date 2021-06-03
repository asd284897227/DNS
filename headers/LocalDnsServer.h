//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_LOCALDNSSERVER_H
#define DNS_LOCALDNSSERVER_H

#include "DNSMessageHandler.h"

class TheadTaskParams {
    SOCKET &localDnsServerSocket;
    SOCKADDR_IN &clientAddr;
    int len;
};

class LocalDnsServer {
public:
    DNSFileHandler handler;
    DNSLRU lru;
    SOCKET serverSocket;

    LocalDnsServer() {
        // 读取本地dns规则
        handler.readLocalIpv4DNSItem("C:\\Users\\wangwei\\CLionProjects\\DNS\\dnsrelay.txt");
        // 创建本地dns server socket
        createSocket();
        // 准备接受dns请求
        while (true) {
            waitMessageFromClient();
        }
    }

    /**
     * 创建本地dns server socket
     * @return
     */
    bool createSocket() {
        WSADATA wsaData;
        // 初始化Socket
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {                                        //初始化was
            ExecutionUtil::fatalError("WSAStartup初始化失败！");
        }

        // 创建socket
        serverSocket = socket(AF_INET, SOCK_DGRAM, 0);//创建本地套接字
        if (serverSocket < 0) {
            ExecutionUtil::fatalError("创建本地DNS Server Socket套接字失败！");
        }

        // 指明地址
        SOCKADDR_IN localDnsAddr;
        localDnsAddr.sin_family = AF_INET;
        localDnsAddr.sin_addr.s_addr = INADDR_ANY;
        localDnsAddr.sin_port = htons(53);

        // Server Socket监听53号端口
        if (bind(serverSocket, (struct sockaddr *) &localDnsAddr, sizeof(localDnsAddr)))//连接套接字与端口，开始收信息
        {
            ExecutionUtil::fatalError("本地DNS Server Socket绑定53端口失败！");
        }

        ExecutionUtil::log("本地DNS Server Socket创建成功！");
        return true;
    }

    /**
     * 等待客户端发送DNS请求
     */
    void waitMessageFromClient() {
        // 创建通信地址
        SOCKADDR_IN clientAddr;
        // 创建报文缓冲区
        char msg[BUFFER_SIZE];

        // 尝试读取UDP报文（阻塞）
        int client_length = sizeof(SOCKADDR_IN);
        int length = recvfrom(serverSocket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddr, &client_length);
        if (length == -1) {
            ExecutionUtil::warning("Local DNS Server Socket读取UDP请求报文失败！");
        } else {
            ExecutionUtil::log("接到udp报文");
            DNSMessageHandler *clientMessageHandler = new DNSMessageHandler(msg, length, serverSocket, clientAddr, this);
        }
    }

};

#endif //DNS_LOCALDNSSERVER_H
