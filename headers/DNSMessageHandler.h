//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_DNSMESSAGEHANDLER_H
#define DNS_DNSMESSAGEHANDLER_H

#include "../allLibs/allLibs.h"
#include "DNSMessage.h"
#include "LocalDnsServer.h"


extern class LocalDnsServer;

class DNSMessageHandler {
public:
    char reqMsg[BUFFER_SIZE];
    char *reqPtr;
    int reqLen = 0;

    char responseMsg[BUFFER_SIZE];
    char *response;
    int resLen = 0;


    DNSHeader header;
    DNSQuestion questions[MAX_QUESTION_ACCOUNT];
    SOCKET localDNSServerSocket;
    SOCKADDR_IN clientAddr;
    LocalDnsServer *localDnsServer;

    DNSMessageHandler(char *msg, int len, SOCKET &localDNSServerSocket, SOCKADDR_IN &clientAddr,
                      LocalDnsServer *localDnsServer) {
        memcpy(this->reqMsg, msg, BUFFER_SIZE);
        reqPtr = msg;
        this->reqLen = len;
        this->localDNSServerSocket = localDNSServerSocket;
        this->clientAddr = clientAddr;
        this->localDnsServer = localDnsServer;
        // 处理报文
        handleMessage();
    }

    void handleMessage() {
        ExecutionUtil::log("准备解析报文");
        processHeaderID_SETP1();
        processQRRow_STEP2();
        processCount_STEP3();
        processQuestions_STEP4();
        processAnswer_STEP5();
        processAuthority_STEP6();
        processAdditional_STEP7();
    }

    /**
     * 处理header id字段（2字节）
     */
    void processHeaderID_SETP1() {
        // todo
        header.id = (unsigned short) reqPtr;
        reqPtr += 2;
    }

    /**
     * 处理header 第二行
     */
    void processQRRow_STEP2() {
        header.row2 = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;
    }

    /**
     * 处理4个account
     */
    void processCount_STEP3() {
        header.qdCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.anCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.nsCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.anCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;
    }

    /**
     * 解析questions
     */
    void processQuestions_STEP4() {
        for (int i = 0; i < header.qdCount; i++) {
            DNSQuestion q;
            q.convertQnameToStdUrl(reqPtr);
            q.setQTypeFromPtr(reqPtr);
            q.setQClassFromPtr(reqPtr);
            questions[i] = q;
        }
    }

    /**
     * 处理answer（无需进行）
     */
    void processAnswer_STEP5() {
        // to do
    }

    /**
     * 处理授权应答（无需进行）
     */
    void processAuthority_STEP6() {
        // to do
    }

    /**
     * 处理附加信息（无需进行）
     */
    void processAdditional_STEP7() {
        // todo
    }

    bool tryToHandleDNSCore(string url) {
        for (int i = 0; i < header.anCount; i++) {

        }
    }

private:

};

#endif //DNS_DNSMESSAGEHANDLER_H
