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
    mutable std::shared_mutex mutex;

    /**
     * 从本地文件中读取dns规则（可以反复读取多个文件，最后一次记录生效）
     */
    void readLocalIpv4DNSItem() {
        ifstream file(IPV4_RULES_FILE_PATH);
        string ip, url;
        mutex.lock();
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreateWithoutLock(url);
            node.setIpv4(ip);
            cout << "获取ipv4：" << ip << "\t" << url << endl;
        }
        mutex.unlock();
        file.close();
    }


    /**
     * 从本地文件中读取dns规则（可以反复读取多个文件，最后一次记录生效）
     * @param filePath
     */
    void readLocalIpv6DNSItem() {
        ifstream file(IPV6_RULES_FILE_PATH);
        string ip, url;
        mutex.lock();
        while (file >> ip >> url) {
            DNSNode &node = getDnsNodeByUrlAndCreateWithoutLock(url);
            node.setIpv6(ip);
            cout << "获取ipv6：" << ip << "\t" << url << endl;
        }
        mutex.unlock();
        file.close();
    }

    /**
     * 更新任务
     */
    void updateTask() {
        while (1) {
            Sleep(LOCAL_DNS_RULES_TTL * 60 * 1000);
            readLocalIpv4DNSItem();
            readLocalIpv6DNSItem();
            cout << "**********************************更新本地DNS规则完成！**********************************" << endl;
        }
    }

    /**
     * 根据url获取DNSNode实例，若dnsMap中不存在url键，则创建并返回一个DNSNode实例（非线程安全）
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
        // 定时刷新DNS规则
        thread t1(&DNSFileHandler::updateTask, this);
        t1.detach();
    }


    /**
     * 根据url获取节点，若不存在url键则返回空节点
     * @param url
     * @return
     */
    DNSNode getNodeByUrl(string url) {
        // 加读锁
        mutex.lock_shared();
        if (dnsMap.find(url) != dnsMap.end()) {
            DNSNode clone = dnsMap[url];
            // 取消读锁
            mutex.unlock_shared();
            return clone;
        } else {
            // 取消读锁
            mutex.unlock_shared();
            return emptyNode;
        }
    }

    /**
     * 根据url获取ipv4
     * @param url
     * @return
     */
    string getIpv4ByUrl(string &url) {
        return getNodeByUrl(url).getIpv4();
    }

    /**
    * 根据url获取ipv6
    * @param url
    * @return
    */
    string getIpv6ByUrl(string &url) {
        return getNodeByUrl(url).getIpv6();
    }
};

#endif //DNS_DNSFILEHANDLER_H
