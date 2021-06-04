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
private:
    map<string, DNSNode> dnsMap;
    DNSNode emptyNode;
    mutable std::shared_mutex mutex;

    /**
     * �ӱ����ļ��ж�ȡdns���򣨿��Է�����ȡ����ļ������һ�μ�¼��Ч��
     */
    void readLocalIpv4DNSItem() {
        ifstream file(IPV4_RULES_FILE_PATH);
        string ip, url;
        mutex.lock();
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreateWithoutLock(url);
            node.setIpv4(ip);
            cout << "��ȡipv4��" << ip << "\t" << url << endl;
        }
        mutex.unlock();
        file.close();
    }


    /**
     * �ӱ����ļ��ж�ȡdns���򣨿��Է�����ȡ����ļ������һ�μ�¼��Ч��
     * @param filePath
     */
    void readLocalIpv6DNSItem() {
        ifstream file(IPV6_RULES_FILE_PATH);
        string ip, url;
        mutex.lock();
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreateWithoutLock(url);
            node.setIpv6(ip);
            cout << "��ȡipv6��" << ip << "\t" << url << endl;
        }
        mutex.unlock();
        file.close();
    }

    /**
     * ��������
     */
    void updateTask() {
        while (1) {
            Sleep(LOCAL_DNS_RULES_TTL * 60 * 1000);
            readLocalIpv4DNSItem();
            readLocalIpv6DNSItem();
            cout << "**********************************���±���DNS������ɣ�**********************************" << endl;
        }
    }

    /**
     * ����url��ȡDNSNodeʵ������dnsMap�в�����url�����򴴽�������һ��DNSNodeʵ�������̰߳�ȫ��
     * @param url
     * @return
     */
    DNSNode &getDnsNodeByUrlAndCreateWithoutLock(string &url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return dnsMap[url];
        } else {
            DNSNode node;
            node.setUrl(url);
            dnsMap[url] = node;
            return dnsMap[url];
        }
    }

public:
    DNSFileHandler() {
        readLocalIpv4DNSItem();
        readLocalIpv6DNSItem();
        // ��ʱˢ��DNS����
        thread t1(&DNSFileHandler::updateTask, this);
        t1.detach();
    }


    /**
     * ����url��ȡ�ڵ㣬��������url���򷵻ؿսڵ�
     * @param url
     * @return
     */
    DNSNode getNodeByUrl(string url) {
        // �Ӷ���
        mutex.lock_shared();
        if (dnsMap.find(url) != dnsMap.end()) {
            DNSNode clone = dnsMap[url];
            // ȡ������
            mutex.unlock_shared();
            return clone;
        } else {
            // ȡ������
            mutex.unlock_shared();
            return emptyNode;
        }
    }

    /**
     * ����url��ȡipv4
     * @param url
     * @return
     */
    string getIpv4ByUrl(string &url) {
        return getNodeByUrl(url).getIpv4();
    }

    /**
    * ����url��ȡipv6
    * @param url
    * @return
    */
    string getIpv6ByUrl(string &url) {
        return getNodeByUrl(url).getIpv6();
    }
};

#endif //DNS_DNSFILEHANDLER_H
