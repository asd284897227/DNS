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
     * ��ȡ���ϴ�С���̲߳���ȫ��
     * @return
     */
    int getSize() {
        return caches.size();
    }

    /**
    * ɾ��ĩβ��Ԫ�أ��̲߳���ȫ��
    */
    DNSCacheNode removeLastNode() {
        // ҵ��
        DNSCacheNode clone = caches.back();
        caches.pop_back();
        return clone;
    }

    /**
     * ��ͷ�����Ԫ�أ��̲߳���ȫ��
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
     * ��ӡcache���̲߳���ȫ��
     */
    void printCache() {
        ExecutionUtil::log("------------------------------------��ӡcache------------------------------------");
        for (DNSCacheNode &node : caches) {
            cout << node.getUrl() << "\t" << node.getTypeOfNode() << endl;
        }
    }


public:
    mutable std::shared_mutex mutex;



/**
 * �Ƶ���λ
 * @param node
 */
    void putToFirst(DNSCacheNode &node) {
        // ��д��
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
        // �ͷ�д��
        mutex.unlock();
    }

/**
 *
 * @param url ����url��ȡ�ڵ㣬��������url���򴴽��½ڵ㣨�̲߳���ȫ��
 * @return
 */
    DNSCacheNode &getNodeByUrlAndCreate(string &url) {
        // ҵ��
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
 * ����url��ȡDNSCacheNode
 * @param url
 * @return
 */
    DNSCacheNode getNodeByUrl(string &url) {
        // �Ӷ���
        mutex.lock_shared();
        // ҵ��
        for (DNSCacheNode &node : caches) {
            if (node.getUrl() == url) {
                DNSCacheNode clone = node;
                // �ͷŶ���
                mutex.unlock_shared();
                printCache();
                return clone;
            }
        }
        //�ͷŻ�����
        mutex.unlock_shared();
        return emptyNode;
    }

};

#endif //DNS_DNSLRU_H
