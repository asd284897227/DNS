//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSLRU_H
#define DNS_DNSLRU_H

#include "ExecutionUtil.h"
#include "DNSNode.h"

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
        caches.insert(caches.begin(), node);
        //�ͷŻ�����
        ReleaseMutex(hMutex);
    }

    /**
     * ɾ��ĩβ��Ԫ��
     */
    DNSNode removeLastNode() {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        // ҵ��
        DNSNode node = caches.back();
        caches.pop_back();
        //�ͷŻ�����
        ReleaseMutex(hMutex);
        return node;
    }

    /**
     * ����url��ȡDNSNode
     * @param url
     * @return
     */
    DNSNode &getNodeByUrl(string &url) {
        //����������
        WaitForSingleObject(hMutex, INFINITE);
        auto n = caches.begin();
        // ҵ��
        for (DNSNode &node : caches) {
            if (node.getUrl() == url) {
                //�ͷŻ�����
                ReleaseMutex(hMutex);
                return node;
            }
        }
        //�ͷŻ�����
        DNSNode newNode = DNSNode();
        return newNode;
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
                return true;
            }
        }
        //�ͷŻ�����
        DNSNode newNode = DNSNode();
        return false;
    }


private:
    list<DNSNode> caches;
    HANDLE hMutex;
};

#endif //DNS_DNSLRU_H
