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
        // ��ȡ����dns����
        handler.readLocalIpv4DNSItem("C:\\Users\\wangwei\\CLionProjects\\DNS\\dnsrelay.txt");
        // ��������dns server socket
        createSocket();
        // ׼������dns����
        while (true) {
            waitMessageFromClient();
        }
    }

    /**
     * ��������dns server socket
     * @return
     */
    bool createSocket() {
        WSADATA wsaData;
        // ��ʼ��Socket
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {                                        //��ʼ��was
            ExecutionUtil::fatalError("WSAStartup��ʼ��ʧ�ܣ�");
        }

        // ����socket
        serverSocket = socket(AF_INET, SOCK_DGRAM, 0);//���������׽���
        if (serverSocket < 0) {
            ExecutionUtil::fatalError("��������DNS Server Socket�׽���ʧ�ܣ�");
        }

        // ָ����ַ
        SOCKADDR_IN localDnsAddr;
        localDnsAddr.sin_family = AF_INET;
        localDnsAddr.sin_addr.s_addr = INADDR_ANY;
        localDnsAddr.sin_port = htons(53);

        // Server Socket����53�Ŷ˿�
        if (bind(serverSocket, (struct sockaddr *) &localDnsAddr, sizeof(localDnsAddr)))//�����׽�����˿ڣ���ʼ����Ϣ
        {
            ExecutionUtil::fatalError("����DNS Server Socket��53�˿�ʧ�ܣ�");
        }

        ExecutionUtil::log("����DNS Server Socket�����ɹ���");
        return true;
    }

    /**
     * �ȴ��ͻ��˷���DNS����
     */
    void waitMessageFromClient() {
        // ����ͨ�ŵ�ַ
        SOCKADDR_IN clientAddr;
        // �������Ļ�����
        char msg[BUFFER_SIZE];

        // ���Զ�ȡUDP���ģ�������
        int client_length = sizeof(SOCKADDR_IN);
        int length = recvfrom(serverSocket, msg, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddr, &client_length);
        if (length == -1) {
            ExecutionUtil::warning("Local DNS Server Socket��ȡUDP������ʧ�ܣ�");
        } else {
            ExecutionUtil::log("�ӵ�udp����");
            DNSMessageHandler *clientMessageHandler = new DNSMessageHandler(msg, length, serverSocket, clientAddr, this);
        }
    }

};

#endif //DNS_LOCALDNSSERVER_H
