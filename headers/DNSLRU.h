//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSLRU_H
#define DNS_DNSLRU_H

#include "ExecutionUtil.h"
#include "DNSNode.h"
#include "../allLibs/allLibs.h"

class DNSLRU {
public:
    DNSLRU() {
        //��������ִ򿪻�����ʧ��
        if ((hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex.Test")) == NULL) {
            // ��������������ʧ��
            if ((hMutex = CreateMutex(NULL, FALSE, "Mutex.Test")) == NULL) {
                //��ӡ���֣���ʾ����������ʧ��
                ExecutionUtil::fatalError("�޷�������������");
            }
                // �ɹ�����������
            else {
                ExecutionUtil::log("�ɹ�������������");
            }
        }
    }

    /**
     * ��ȡ���ϴ�С���̲߳���ȫ��
     * @return
     */
    int getSize() {
        return caches.size();
    }


    /**
     * ��ͷ�����Ԫ��
     * @param node
     */
    void insertFirst(DNSNode &node) {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        if (getSize() >= MAX_LRU_SIZE) {
            removeLastNode();
        }
        // insert before
        caches.insert(caches.begin(), node);
        //�ͷŻ�����
        ReleaseMutex(hMutex);
    }


    /**
     * ɾ��ĩβ��Ԫ�أ��̲߳���ȫ��
     */
    DNSNode &removeLastNode() {
        // ҵ��
        DNSNode node = caches.back();
        caches.pop_back();
        return node;
    }

    void putToFirst(DNSNode &node) {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        for (list<DNSNode>::iterator ite = caches.begin(); ite != caches.end(); ite++) {
            if (node.getUrl() == ite->getUrl()) {
                insertFirst(node);
                removeNode(ite.operator*());
            }
        }
        //�ͷŻ�����
        ReleaseMutex(hMutex);
    }

    /**
     * ����url��ȡDNSNode
     * @param url
     * @return
     */
    DNSNode &getNodeByUrl(string &url) {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        // ҵ��
        for (DNSNode &node : caches) {
            if (node.getUrl() == url) {
                //�ͷŻ�����
                ReleaseMutex(hMutex);
                return node;
            }
        }
        //�ͷŻ�����
        ReleaseMutex(hMutex);
        DNSNode emptyNode;
        return emptyNode;
    }


    /**
     * ɾ���ڵ�
     * @param node
     * @return
     */
    bool removeNode(DNSNode &node) {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        list<DNSNode>::iterator ite;
        for (ite = caches.begin(); ite != caches.end(); ++ite) {
            if (ite->getUrl() == node.getUrl()) {
                caches.erase(ite);
                //�ͷŻ�����
                ReleaseMutex(hMutex);
                return true;
            }
        }
        //�ͷŻ�����
        ReleaseMutex(hMutex);
        return false;
    }


private:
    list<DNSNode> caches;
    HANDLE hMutex;
};

#endif //DNS_DNSLRU_H
