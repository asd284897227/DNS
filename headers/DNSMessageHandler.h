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
        // ������
        handleMessage();
    }

    void handleMessage() {
        ExecutionUtil::log("׼����������");
        processHeaderID_SETP1();
        processQRRow_STEP2();
        processCount_STEP3();
        processQuestions_STEP4();
        processAnswer_STEP5();
        processAuthority_STEP6();
        processAdditional_STEP7();
    }

    /**
     * ����header id�ֶΣ�2�ֽڣ�
     */
    void processHeaderID_SETP1() {
        // todo
        header.id = (unsigned short) reqPtr;
        reqPtr += 2;
    }

    /**
     * ����header �ڶ���
     */
    void processQRRow_STEP2() {
        header.row2 = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;
    }

    /**
     * ����4��account
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
     * ����questions
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
     * ����answer��������У�
     */
    void processAnswer_STEP5() {
        // to do
    }

    /**
     * ������ȨӦ��������У�
     */
    void processAuthority_STEP6() {
        // to do
    }

    /**
     * ��������Ϣ��������У�
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
