//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSLRU_H
#define DNS_DNSLRU_H

#include "ExecutionUtil.h"
#include "DNSNode.h"
#include "../allLibs/allLibs.h"

class DNSLRU {

private:
    list<DNSNode> caches;
    mutable std::shared_mutex mutex;
    DNSNode emptyNode;


    /**
     * 获取集合大小（线程不安全）
     * @return
     */
    int getSize() {
        return caches.size();
    }

    /**
    * 删除末尾的元素（线程不安全）
    */
    DNSNode removeLastNode() {
        // 业务
        DNSNode clone = caches.back();
        caches.pop_back();
        return clone;
    }

    /**
     * 在头部添加元素（线程不安全）
     * @param node
     */
    void insertFirstWithoutLock(DNSNode &node) {
        if (getSize() >= MAX_LRU_SIZE) {
            removeLastNode();
        }
        // insert before
        caches.insert(caches.begin(), node);
    }

    /**
     * 打印cache（线程不安全）
     */
    void printCache() {
        ExecutionUtil::log("------------------------------------打印cache------------------------------------");
        for (DNSNode &node : caches) {
            cout << node.getUrl() << "\t" << node.getIpv4() << "\t" << node.getIpv6() << endl;
        }
    }

public:


    /**
     * 在头部添加元素
     * @param node
     */
    void insertFirst(DNSNode &node) {
        // 加写锁
        mutex.lock();
        if (getSize() >= MAX_LRU_SIZE) {
            removeLastNode();
        }
        // insert before
        caches.insert(caches.begin(), node);
        printCache();
        // 释放写锁
        mutex.unlock();
    }


    /**
     * 移到首位
     * @param node
     */
    void putToFirst(DNSNode &node) {
        // 加写锁
        mutex.lock();
        DNSNode clone;
        for (list<DNSNode>::iterator ite = caches.begin(); ite != caches.end(); ite++) {
            if (node.getUrl() == ite->getUrl()) {
                clone = ite.operator*();
                caches.erase(ite);
                break;
            }
        }
        insertFirstWithoutLock(clone);
        printCache();
        // 释放写锁
        mutex.unlock();
    }

    /**
     * 根据url获取DNSNode
     * @param url
     * @return
     */
    DNSNode getNodeByUrl(string &url) {
        // 加读锁
        mutex.lock_shared();
        // 业务
        for (DNSNode &node : caches) {
            if (node.getUrl() == url) {
                DNSNode clone = node;
                // 释放读锁
                mutex.unlock_shared();
                printCache();
                return clone;
            }
        }
        //释放互斥量
        mutex.unlock_shared();
        return emptyNode;
    }
};

#endif //DNS_DNSLRU_H
