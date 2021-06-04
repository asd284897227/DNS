//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSFILEHANDLER_H
#define DNS_DNSFILEHANDLER_H

#include "../allLibs/allLibs.h"
#include "DNSNode.h"


/**
 * DNS规则处理器
 */
class DNSFileHandler {
private:
    map<string, DNSNode> dnsMap;
    DNSNode emptyNode;
public:

    /**
     * 从本地文件中读取dns规则（可以反复读取多个文件，最后一次记录生效）
     * @param filePath
     */
    void readLocalIpv4DNSItem(string filePath) {
        ifstream file(filePath);
        string ip, url;
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreate(url);
            node.setIpv4(ip);
            cout << "获取ipv4：" << ip << "\t" << url << endl;
        }
        file.close();
    }


    /**
     * 从本地文件中读取dns规则（可以反复读取多个文件，最后一次记录生效）
     * @param filePath
     */
    void readLocalIpv6DNSItem(string filePath) {
        ifstream file(filePath);
        string ip, url;
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreate(url);
            node.setIpv6(ip);
            cout << "获取ipv6：" << ip << "\t" << url << endl;
        }
        file.close();
    }


    void updateTask() {
        while (1) {
            Sleep(LOCAL_DNS_RULES_TTL);

        }
    }

    /**
     * 根据url获取DNSNode实例，若dnsMap中不存在url键，则创建并返回一个DNSNode实例
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

    /**
     * 根据url获取节点，若不存在url键则返回空节点
     * @param url
     * @return
     */
    DNSNode &getNodeByUrl(string url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return dnsMap[url];
        } else {
            return emptyNode;
        }
    }

    /**
     * 返回当前记录中是否存在url的节点
     * @param url
     * @return
     */
    bool containsByUrl(string &url) {
        if (dnsMap.find(url) != dnsMap.end()) {
            return true;
        } else {
            return false;
        }
    }
};

#endif //DNS_DNSFILEHANDLER_H
