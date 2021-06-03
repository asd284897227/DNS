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
        //如果按名字打开互斥量失败
        if ((hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, "Mutex.Test")) == NULL) {
            // 创建互斥量，若失败
            if ((hMutex = CreateMutex(NULL, FALSE, "Mutex.Test")) == NULL) {
                //打印文字，提示创建互斥量失败
                ExecutionUtil::fatalError("无法创建互斥锁！");
            }
                // 成功创建互斥量
            else {
                ExecutionUtil::log("成功创建互斥锁！");
            }
        }
    }

    /**
     * 获取集合大小（线程不安全）
     * @return
     */
    int getSize() {
        return caches.size();
    }


    /**
     * 在头部添加元素
     * @param node
     */
    void insertFirst(DNSNode &node) {
        //互斥量加锁
        WaitForSingleObject(hMutex, INFINITE);
        caches.insert(caches.begin(), node);
        //释放互斥量
        ReleaseMutex(hMutex);
    }

    /**
     * 删除末尾的元素
     */
    DNSNode removeLastNode() {
        //互斥量加锁
        WaitForSingleObject(hMutex, INFINITE);
        // 业务
        DNSNode node = caches.back();
        caches.pop_back();
        //释放互斥量
        ReleaseMutex(hMutex);
        return node;
    }

    /**
     * 根据url获取DNSNode
     * @param url
     * @return
     */
    DNSNode &getNodeByUrl(string &url) {
        //互斥量加锁
        WaitForSingleObject(hMutex, INFINITE);
        auto n = caches.begin();
        // 业务
        for (DNSNode &node : caches) {
            if (node.getUrl() == url) {
                //释放互斥量
                ReleaseMutex(hMutex);
                return node;
            }
        }
        //释放互斥量
        DNSNode newNode = DNSNode();
        return newNode;
    }


    /**
     * 删除节点
     * @param node
     * @return
     */
    bool removeNode(DNSNode &node) {
        //互斥量加锁
        WaitForSingleObject(hMutex, INFINITE);
        list<DNSNode>::iterator ite;
        for (ite = caches.begin(); ite != caches.end(); ++ite) {
            if (ite->getUrl() == node.getUrl()) {
                caches.erase(ite);
                return true;
            }
        }
        //释放互斥量
        DNSNode newNode = DNSNode();
        return false;
    }


private:
    list<DNSNode> caches;
    HANDLE hMutex;
};

#endif //DNS_DNSLRU_H
