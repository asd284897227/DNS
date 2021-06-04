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
     * ����url��ȡupv4
     * @param url
     * @return
     */
    string getIpv4ByUrl(string url) {
        return getDnsNodeByUrlAndCreate(url).getIpv4();
    }

    /**
     * ����url��ȡupv6
     * @param url
     * @return
     */
    string getIpv6ByUrl(string url) {
        return getDnsNodeByUrlAndCreate(url).getIpv6();
    }

    /**
    * ��ȡip����
    * @param ip
    * @return
    */
    int getTypeOfIp(string &ip) {
        if (ip.length() <= 15)
            return TYPE_IPV4;
        else
            return TYPE_IPV6;
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
