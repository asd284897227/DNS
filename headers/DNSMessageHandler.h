//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_DNSMESSAGEHANDLER_H
#define DNS_DNSMESSAGEHANDLER_H

#include "../allLibs/allLibs.h"
#include "DNSMessage.h"
#include "LocalDnsServer.h"

class LocalDnsServer;

class DNSMessageHandler {
public:
    char msg[BUFFER_SIZE];
    char *ptr;
    int len;

    DNSHeader header;
    DNSQuestion questions[MAX_QUESTION_ACCOUNT];
    SOCKET localDNSServerSocket;
    SOCKADDR_IN clientAddr;
    LocalDnsServer *localDnsServer;

    DNSMessageHandler(char *msg, int len, SOCKET &localDNSServerSocket, SOCKADDR_IN &clientAddr,
                      LocalDnsServer *localDnsServer) {
        memcpy(this->msg, msg, BUFFER_SIZE);
        ptr = msg;
        this->len = len;
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
    }

    /**
     * ����header id�ֶΣ�2�ֽڣ�
     */
    void processHeaderID_SETP1() {
        // todo
        header.id = (unsigned short) ptr;
        ptr += 2;
    }

    /**
     * ����header �ڶ���
     */
    void processQRRow_STEP2() {
        header.row2 = ntohs(*(unsigned short *) ptr);
        ptr += 2;
    }

    /**
     * ����4��account
     */
    void processCount_STEP3() {
        header.qdCount = ntohs(*(unsigned short *) ptr);
        ptr += 2;

        header.anCount = ntohs(*(unsigned short *) ptr);
        ptr += 2;

        header.nsCount = ntohs(*(unsigned short *) ptr);
        ptr += 2;

        header.anCount = ntohs(*(unsigned short *) ptr);
        ptr += 2;
    }

    void processQuestions_STEP4() {
        for (int i = 0; i < header.qdCount; i++) {
            DNSQuestion q;
            q.convertQnameToStdUrl(ptr);
            q.setQTypeFromPtr(ptr);
        }
    }

private:

};

#endif //DNS_DNSMESSAGEHANDLER_H
