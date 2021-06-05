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
     * ��ȡ���ϴ�С���̲߳���ȫ��
     * @return
     */
    int getSize() {
        return caches.size();
    }

    /**
    * ɾ��ĩβ��Ԫ�أ��̲߳���ȫ��
    */
    DNSNode removeLastNode() {
        // ҵ��
        DNSNode clone = caches.back();
        caches.pop_back();
        return clone;
    }

    /**
     * ��ͷ�����Ԫ�أ��̲߳���ȫ��
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
     * ��ӡcache���̲߳���ȫ��
     */
    void printCache() {
        ExecutionUtil::log("------------------------------------��ӡcache------------------------------------");
        for (DNSNode &node : caches) {
            cout << node.getUrl() << "\t" << node.getIpv4() << "\t" << node.getIpv6() << endl;
        }
    }

public:


    /**
     * ��ͷ�����Ԫ��
     * @param node
     */
    void insertFirst(DNSNode &node) {
        // ��д��
        mutex.lock();
        if (getSize() >= MAX_LRU_SIZE) {
            removeLastNode();
        }
        // insert before
        caches.insert(caches.begin(), node);
        printCache();
        // �ͷ�д��
        mutex.unlock();
    }


    /**
     * �Ƶ���λ
     * @param node
     */
    void putToFirst(DNSNode &node) {
        // ��д��
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
        // �ͷ�д��
        mutex.unlock();
    }

    /**
     * ����url��ȡDNSNode
     * @param url
     * @return
     */
    DNSNode getNodeByUrl(string &url) {
        // �Ӷ���
        mutex.lock_shared();
        // ҵ��
        for (DNSNode &node : caches) {
            if (node.getUrl() == url) {
                DNSNode clone = node;
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
