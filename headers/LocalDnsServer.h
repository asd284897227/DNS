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
        // ��ʼ��Socket
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {                                        //��ʼ��was
            printf("WSAStartup��ʼ��ʧ�ܣ�%d\n", GetLastError());
            exit(-1);
        }

        if (serverSocket < 0) {
            printf("��������DNS Server Socket�׽���ʧ��\n");
            exit(1);
        }

        SOCKADDR_IN localDnsAddr;
        localDnsAddr.sin_family = AF_INET;            /* Set the family as AF_INET (TCP/IP)��AF_INET����IPV4internet protocols */
        localDnsAddr.sin_addr.s_addr = INADDR_ANY;    /* Set to any ��������ĵ�ַ��53�˿�*/
        localDnsAddr.sin_port = htons(53);           /* Set the port as DNS port (53)��dnsר�õĶ˿� */
        //htons�����������ͱ����������ֽ�˳��ת��������ֽ�˳��
        //�����ֽ�˳����TCP/IP�й涨�õ�һ�����ݱ�ʾ��ʽ����������CPU���͡�����ϵͳ���޹أ�
        //�Ӷ����Ա�֤�����ڲ�ͬ����֮�䴫��ʱ�ܹ�����ȷ���ͣ������ֽ�˳�����big-endian����ʽ


        // Server Socket����53�Ŷ˿�
        if (bind(serverSocket, (struct sockaddr *) &localDnsAddr, sizeof(localDnsAddr)))//�����׽�����˿ڣ���ʼ����Ϣ
        {
            printf("Bind Port 53 ʧ��. %d\n", GetLastError());
            exit(1);
        }
    }

private:
    SOCKET serverSocket;

};

#endif //DNS_LOCALDNSSERVER_H
