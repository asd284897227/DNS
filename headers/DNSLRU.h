//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSLRU_H
#define DNS_DNSLRU_H

#include "ExecutionUtil.h"
#include "DNSCacheNode.h"
#include "../allLibs/allLibs.h"

class DNSLRU {
private:
    list<DNSCacheNode> caches;
    DNSCacheNode emptyNode;


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
    DNSCacheNode removeLastNode() {
        // 业务
        DNSCacheNode clone = caches.back();
        caches.pop_back();
        return clone;
    }

    /**
     * 在头部添加元素（线程不安全）
     * @param node
     */
    void insertFirstWithoutLock(DNSCacheNode &node) {
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
        for (DNSCacheNode &node : caches) {
            cout << node.getUrl() << "\t" << node.getTypeOfNode() << endl;
        }
    }


public:
    mutable std::shared_mutex mutex;



/**
 * 移到首位
 * @param node
 */
    void putToFirst(DNSCacheNode &node) {
        // 加写锁
        mutex.lock();
        DNSCacheNode clone(node.getUrl());
        for (list<DNSCacheNode>::iterator ite = caches.begin(); ite != caches.end(); ite++) {
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
 *
 * @param url 根据url获取节点，若不存在url键则创建新节点（线程不安全）
 * @return
 */
    DNSCacheNode &getNodeByUrlAndCreate(string &url) {
        // 业务
        for (DNSCacheNode &node : caches) {
            if (node.getUrl() == url) {
                return node;
            }
        }
        if (getSize() >= MAX_LRU_SIZE) {
            removeLastNode();
        }
        DNSCacheNode newNode(url);
        // insert before
        caches.insert(caches.begin(), newNode);
        return caches.begin().operator*();
    }

/**
 * 根据url获取DNSCacheNode
 * @param url
 * @return
 */
    DNSCacheNode getNodeByUrl(string &url) {
        // 加读锁
        mutex.lock_shared();
        // 业务
        for (DNSCacheNode &node : caches) {
            if (node.getUrl() == url) {
                DNSCacheNode clone = node;
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
