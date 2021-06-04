//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSFILEHANDLER_H
#define DNS_DNSFILEHANDLER_H

#include "../allLibs/allLibs.h"
#include "DNSNode.h"


/**
 * DNS��������
 */
class DNSFileHandler {
public:
    map<string, DNSNode> dnsMap;


    /**
     * �ӱ����ļ��ж�ȡdns���򣨿��Է�����ȡ����ļ������һ�μ�¼��Ч��
     * @param filePath
     */
    void readLocalIpv4DNSItem(string filePath) {
        ifstream file(filePath);
        string ip, url;
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreate(url);
            node.setIpv4(ip);
            cout << "��ȡipv4��" << ip << "\t" << url << endl;
        }
        file.close();
    }


    /**
     * �ӱ����ļ��ж�ȡdns���򣨿��Է�����ȡ����ļ������һ�μ�¼��Ч��
     * @param filePath
     */
    void readLocalIpv6DNSItem(string filePath) {
        ifstream file(filePath);
        string ip, url;
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreate(url);
            node.setIpv6(ip);
            cout << "��ȡipv6��" << ip << "\t" << url << endl;
        }
        file.close();
    }


    /**
     * ����url��ȡDNSNodeʵ������dnsMap�в�����url�����򴴽�������һ��DNSNodeʵ��
     * @param url 
     * @return 
     */
    DNSNode &getDnsNodeByUrlAndCreate(string &url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return dnsMap[url];
        } else {
            DNSNode node;
            node.setUrl(url);
            dnsMap[url] = node;
            return dnsMap[url];
        }
    }

    DNSNode &getNodeByUrl(string url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return dnsMap[url];
        } else {
            DNSNode emptyNode;
            return emptyNode;
        }
    }

    bool containsByUrl(string &url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif //DNS_DNSFILEHANDLER_H
