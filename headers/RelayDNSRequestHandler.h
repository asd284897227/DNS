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
    SOCKET &localDNSSocket;
    SOCKADDR_IN &clientAddr;

    // �м���Ϣָ��ͱ��ĳ���
    char (&msg)[BUFFER_SIZE];
    int length;

    string &url;
    int type;
    DNSLRU &lru;

    /**
     *
     * @param msg ���Ļ������׵�ַ
     * @param length ���ĳ���
     */
    RelayDNSRequestHandler(char (&msg)[BUFFER_SIZE], int length,
                           string &url, int type,
                           DNSLRU &lru,
                           SOCKET &localDNSSocket, SOCKADDR_IN &clientAddr)
            : msg(msg), length(length),
              url(url), type(type),
              lru(lru),
              localDNSSocket(localDNSSocket), clientAddr(clientAddr) {
        createRelaySocket();
        sendMessageToExternDNSServer();
        waitForMessageFromExternDNSServer();
    }


    /**
     * ���������ϲ�dns������Socket���׽���
     * @return
     */
    void createRelaySocket() {
        // �����׽���
        externSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//�����ⲿ�׽���
        if (externSocket < 0) {
            ExecutionUtil::warning("������ʱSocket�׽��֣������ϲ�DNS��������ʧ�ܣ�\n");
        }
        long long timeout = 1000;
        if (setsockopt(externSocket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
            ExecutionUtil::warning("�����ϲ�DNS������Socket�׽��֣����ö�ȡ��ʱʧ��!\n");
        }
        if (setsockopt(externSocket, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
            ExecutionUtil::warning("�����ϲ�DNS������Socket�׽��֣����÷��ͳ�ʱʧ��!\n");
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
        int lenOfExtern = recvfrom(externSocket, (char *) msg, BUFFER_SIZE, 0, (struct sockaddr *) &externAddr,
                                   &addrLen);
        if (lenOfExtern < 0) {
            ExecutionUtil::log("�����ⲿ����������\n");
            return;
        } else {
            if (type == TYPE_IPV4 || type == TYPE_IPV6) {
                lru.mutex.lock();
                DNSCacheNode &node = lru.getNodeByUrlAndCreate(url);
                if (type == TYPE_IPV4) {
                    node.setIpV4(msg, lenOfExtern);
                } else if (type == TYPE_IPV6) {
                    node.setIpV6(msg, lenOfExtern);
                }
                lru.mutex.unlock();
            }
            sendto(localDNSSocket, msg, lenOfExtern, 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
            ExecutionUtil::log("���ɹ���תһ��DNS���󣡡�" + to_string(*((unsigned short *) msg)));
        }
    }
};

#endif //DNS_RELAYDNSREQUESTHANDLER_H
