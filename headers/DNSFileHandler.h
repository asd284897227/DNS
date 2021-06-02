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
public:
    map<string, DNSNode> dnsMap;


    /**
     * 从本地文件中读取dns规则（可以反复读取多个文件，最后一次记录生效）
     * @param filePath
     */
    void readLocalIpv4DNSItem(string filePath) {
        ifstream file(filePath);
        string ip, url;
        while (file >> ip >> url) {
            // 处理ipv4
            if (getTypeOfIp(ip) == TYPE_IPV4) {
                DNSNode &node = getDnsNodeByUrl(url);
                node.setIpv4(ip);
                cout << "获取ipv4：" << ip << "\t" << url << endl;
            }
                // 处理ipv6
            else {
                DNSNode &node = getDnsNodeByUrl(url);
                node.setIpv6(ip);
                cout << "获取ipv6：" << ip << "\t" << url << endl;
            }

        }
        file.close();
    }

    /**
     * 根据url获取upv4
     * @param url
     * @return
     */
    string getIpv4ByUrl(string url) {
        return getDnsNodeByUrl(url).getIpv4();
    }

    /**
     * 根据url获取upv6
     * @param url
     * @return
     */
    string getIpv6ByUrl(string url) {
        return getDnsNodeByUrl(url).getIpv6();
    }

    /**
    * 获取ip类型
    * @param ip
    * @return
    */
    int getTypeOfIp(string &ip) {
        if (ip.length() <= 15)
            return TYPE_IPV4;
        else
            return TYPE_IPV6;
    }

private:
    /**
     * 根据url获取DNSNode实例，若dnsMap中不存在url键，则创建并返回一个DNSNode实例
     * @param url 
     * @return 
     */
    DNSNode &getDnsNodeByUrl(string url) {
        if (dnsMap.contains(url)) {
            return dnsMap[url];
        } else {
            DNSNode &node = dnsMap[url];
            return node;
        }
    }
};

#endif //DNS_DNSFILEHANDLER_H
