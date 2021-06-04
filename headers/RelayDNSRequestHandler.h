//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_RELAYDNSREQUESTHANDLER_H
#define DNS_RELAYDNSREQUESTHANDLER_H

#include "ExecutionUtil.h"

class RelayDNSRequestHandler {
public:
    // 外部dns服务器套接字和ip
    SOCKET externSocket;
    SOCKADDR_IN externAddr;


    // 本机dns服务器套接字和ip
    SOCKET localDNSSocket;
    SOCKADDR_IN clientAddr;

    // 中继消息指针和报文长度
    char *msg;
    int length;

    /**
     *
     * @param msg 报文缓冲区首地址
     * @param length 报文长度
     */
    RelayDNSRequestHandler(char msg[], int length, SOCKET &localDNSSocket, SOCKADDR_IN &clientAddr) {
        this->msg = msg;
        this->length = length;
        this->localDNSSocket = localDNSSocket;
        this->clientAddr = clientAddr;
        createRelaySocket();
        sendMessageToExternDNSServer();
        waitForMessageFromExternDNSServer();
    }

    /**
     * 创建连接上层dns服务器Socket的套接字
     * @return
     */
    void createRelaySocket() {
        // 创建套接字
        externSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//创建外部套接字
        if (externSocket < 0) {
            ExecutionUtil::warning("创建临时Socket套接字（连接上层DNS服务器）失败！\n");
        }
        long long timeout = 1000;
        if (setsockopt(externSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
            ExecutionUtil::warning("连接上层DNS服务器Socket套接字，设置读取超时失败!\n");
        }
        if (setsockopt(externSocket, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
            ExecutionUtil::warning("连接上层DNS服务器Socket套接字，设置发送超时失败!\n");
        }
        // 指明连接地址
        externAddr.sin_family = AF_INET;
        externAddr.sin_port = htons(53);
        externAddr.sin_addr.S_un.S_addr = inet_addr(EXTERN_DNS_SERVER_IP);
    }

    /**
     * 向上级DNS服务器发送DNS请求
     * @return
     */
    bool sendMessageToExternDNSServer() {
        // 向外发送请求，顺序在前
        int len = sizeof(clientAddr);
        if (sendto(externSocket, (char *) msg, length, 0, (struct sockaddr *) &externAddr, len) < 0) {
            ExecutionUtil::warning("向上级服务器中转UDP DNS请求时失败！");
            return false;
        }
    }

    /**
     * 等待上级DNS服务器回复，并进行中转
     */
    void waitForMessageFromExternDNSServer() {
        int addrLen = sizeof(clientAddr);
        int lenOfExtern = recvfrom(externSocket, (char *) msg, BUFFER_SIZE, 0, (struct sockaddr *) &externAddr,
                                   &addrLen);
        if (lenOfExtern < 0) {
            ExecutionUtil::log("接收外部服务器出错！\n");
            return;
        } else {
            sendto(localDNSSocket, msg, lenOfExtern, 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
            ExecutionUtil::log("【成功中转一次DNS请求！】" + to_string(*((unsigned short *) msg)));
            free(msg);
        }
    }
};

#endif //DNS_RELAYDNSREQUESTHANDLER_H
