//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_RELAYDNSREQUESTHANDLER_H
#define DNS_RELAYDNSREQUESTHANDLER_H

#include "ExecutionUtil.h"

class RelayDNSRequestHandler {
public:
    // �ⲿdns�������׽��ֺ�ip
    SOCKET externSocket;
    SOCKADDR_IN externAddr;


    // ����dns�������׽��ֺ�ip
    SOCKET localDNSSocket;
    SOCKADDR_IN clientAddr;

    // �м���Ϣָ��ͱ��ĳ���
    char *msg;
    int length;

    /**
     *
     * @param msg ���Ļ������׵�ַ
     * @param length ���ĳ���
     */
    RelayDNSRequestHandler(char msg[], int length, SOCKET &localDNSSocket, SOCKADDR_IN &clientAddr) {
        this->msg = msg;
        this->length = length;
        this->localDNSSocket = localDNSSocket;
        this->clientAddr = clientAddr;
    }

    /**
     * ���������ϲ�dns������Socket���׽���
     * @return
     */
    SOCKET createRelaySocket() {
        // �����׽���
        externSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//�����ⲿ�׽���
        if (externSocket < 0) {
            ExecutionUtil::warning("������ʱSocket�׽��֣������ϲ�DNS��������ʧ�ܣ�\n");
        }
        // ���ó�ʱʱ��
        struct timeval timeOut;
        timeOut.tv_sec = 5;                 //����5s��ʱ
        timeOut.tv_usec = 0;
        if (setsockopt(externSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeOut, sizeof(timeOut)) < 0) {
            ExecutionUtil::warning("�����ϲ�DNS������Socket�׽��֣����ó�ʱʧ��!\n");
        }
        // ָ�����ӵ�ַ
        externAddr.sin_family = AF_INET;
        externAddr.sin_port = htons(53);
        externAddr.sin_addr.S_un.S_addr = inet_addr(EXTERN_DNS_SERVER_IP);
    }

    /**
     * ���ϼ�DNS����������DNS����
     * @return
     */
    bool sendMessageToExternDNSServer() {
        // ���ⷢ������˳����ǰ
        int len = sizeof(clientAddr);
        if (sendto(externSocket, (char *) msg, length, 0, (struct sockaddr *) &externAddr, len) < 0) {
            ExecutionUtil::warning("���ϼ���������תUDP DNS����ʱʧ�ܣ�");
            return false;
        }
    }

    /**
     * �ȴ��ϼ�DNS�������ظ�����������ת
     */
    void waitForMessageFromExternDNSServer() {
        int addrLen = sizeof(clientAddr);
        int lenOfExtern = recvfrom(externSocket, (char *) msg, BUFFER_SIZE, 0, (struct sockaddr *) &externAddr, &addrLen);
        if (lenOfExtern < 0) {
            printf("�����ⲿ������������%d\n", GetLastError());
            return;
        } else {
            sendto(localDNSSocket, msg, lenOfExtern, 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
            printf("���ɹ���תһ��DNS���󣡡�\n");
            free(msg);
        }
    }
};

#endif //DNS_RELAYDNSREQUESTHANDLER_H