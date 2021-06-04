//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_DNSMESSAGEHANDLER_H
#define DNS_DNSMESSAGEHANDLER_H

#include "../allLibs/allLibs.h"
#include "DNSMessage.h"
#include "LocalDnsServer.h"
#include "DNSFileHandler.h"
#include "DNSLRU.h"
#include "RelayDNSRequestHandler.h"
#include "ThreadPool.h"


class DNSMessageHandler {
public:
    char reqMsg[BUFFER_SIZE];
    char *reqPtr;
    int reqLen = 0;

    char resMsg[BUFFER_SIZE];
    unsigned short *qdCount;
    char *resPtr;
    int resLen = 0;

    DNSHeader header;
    DNSQuestion questions[MAX_QUESTION_ACCOUNT];
    SOCKET &localDNSServerSocket;
    SOCKADDR_IN &clientAddr;

    DNSFileHandler localDnsFileHandler;
    DNSLRU lru;

    DNSMessageHandler(char msg[BUFFER_SIZE], int len,
                      SOCKET &localDNSServerSocket, SOCKADDR_IN &clientAddr,
                      DNSFileHandler &localDNSFileHandler, DNSLRU &lru)
            : localDNSServerSocket(localDNSServerSocket), clientAddr(clientAddr) {
        memcpy(this->reqMsg, msg, BUFFER_SIZE);
        memcpy(this->resMsg, msg, BUFFER_SIZE);
        reqPtr = msg;
        resPtr = resMsg;
        this->reqLen = len;
        this->localDnsFileHandler = localDNSFileHandler;
        this->lru = lru;
        handleMessage();
    }

    void handleMessage() {
        ExecutionUtil::log("解析DNS：" + to_string(*((unsigned short *) reqMsg)));
        processHeaderID_SETP1();
        processQRRow_STEP2();
        processCount_STEP3();
        processQuestions_STEP4();
        tryToHandleDNSCore();
    }

    /**
     * 处理header id字段（2字节）
     */
    void processHeaderID_SETP1() {
        // todo
        header.id = (unsigned short) reqPtr;
        reqPtr += 2;
    }

    /**
     * 处理header 第二行
     */
    void processQRRow_STEP2() {
        header.row2 = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;
    }

    /**
     * 处理4个account
     */
    void processCount_STEP3() {
        header.qdCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.anCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.nsCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;

        header.anCount = ntohs(*(unsigned short *) reqPtr);
        reqPtr += 2;
    }

    /**
     * 解析questions
     */
    void processQuestions_STEP4() {
        for (int i = 0; i < header.qdCount; i++) {
            DNSQuestion q;
            q.convertQnameToStdUrl(reqPtr);
            q.setQTypeFromPtr(reqPtr);
            q.setQClassFromPtr(reqPtr);
            questions[i] = q;
        }
    }

    void tryToHandleDNSCore() {
        for (int i = 0; i < header.qdCount; i++) {
            // 只处理qdcount = 1的情形
            if (i != 0) break;

            // 获取协议类型：ipv4/ipv6
            DNSQuestion q = questions[i];
            unsigned char qType = q.getQType();
            // ip结果
            string ip;
            int ipType = TYPE_IPV4;
            // 查询cache
            DNSNode &nodeInCache = lru.getNodeByUrl(q.qName);

            if (qType == 0x01 && header.getOpcode() == 0x0) {
                ipType = TYPE_IPV4;
                // cache中存在ipv4记录
                if (!nodeInCache.isIpv4Empty) {
                    ip = nodeInCache.getIpv4();
                    lru.putToFirst(nodeInCache);
                    addIpResponseToClient(ip, TYPE_IPV4, i == 0);
                }
                    // cache不存在ipv4记录
                else {
                    DNSNode &nodeInMap = localDnsFileHandler.getNodeByUrl(q.qName);
                    // 对照表存在ipv4记录
                    if (!nodeInMap.isIpv4Empty) {
                        ip = nodeInMap.getIpv4();
                        if (!nodeInCache.isIpv6Empty) {
                            nodeInCache.setIpv4(ip);
                            lru.putToFirst(nodeInCache);
                        } else {
                            lru.insertFirst(nodeInMap);
                        }
                        addIpResponseToClient(ip, TYPE_IPV4, i == 0);
                    }
                        // 对照表不存在ipv4记录，则上抛至外部服务器
                    else {
                        RelayDNSRequestHandler reh(reqMsg, reqLen, localDNSServerSocket, clientAddr);
                        return;
                    }
                }
            }
                // 查询ipv6
            else if (qType == 0x1c && header.getOpcode() == 0x0) {
                ipType = TYPE_IPV6;
                // cache中存在ipv6记录
                if (!nodeInCache.isIpv6Empty) {
                    ip = nodeInCache.getIpv6();
                    lru.putToFirst(nodeInCache);
                    addIpResponseToClient(ip, TYPE_IPV6, i == 0);
                }
                    // cache不存在ipv6记录
                else {
                    DNSNode &nodeInMap = localDnsFileHandler.getNodeByUrl(q.qName);
                    // 对照表存在ipv6记录
                    if (!nodeInMap.isIpv6Empty) {
                        ip = nodeInMap.getIpv6();
                        if (!nodeInCache.isIpv4Empty) {
                            nodeInCache.setIpv6(ip);
                            lru.putToFirst(nodeInCache);
                        } else {
                            lru.insertFirst(nodeInMap);
                        }
                        addIpResponseToClient(ip, TYPE_IPV6, i == 0);
                    }
                        // 对照表不存在ipv4记录，则上抛至外部服务器
                    else {
                        RelayDNSRequestHandler reh(reqMsg, reqLen, localDNSServerSocket, clientAddr);
                        return;
                    }
                }
            } else {
                RelayDNSRequestHandler reh(reqMsg, reqLen, localDNSServerSocket, clientAddr);
                return;
            }
        }
        sendto(localDNSServerSocket, resMsg, resPtr - resMsg, 0, (SOCKADDR *) &clientAddr, sizeof(SOCKADDR));
        ExecutionUtil::log("完成处理：" + to_string(*((unsigned short *) reqMsg)));
    }

    void addIpResponseToClient(string &ip, int ipType, bool first) {
        // 检查是否禁止
        if (!isForbidden(ip)) {
            // 确保指针移至answer
            if (first) {
                // 跳过id
                resPtr += 2;

                // 设置第二行
                (*(unsigned short *) resPtr) = htons(0x8180);
                resPtr += 2;

                // qDCount
                qdCount = (unsigned short *) resPtr;
                (*(unsigned short *) resPtr) = htons(header.qdCount);
                resPtr += 2;

                // anscount基地址
                (*(unsigned short *) resPtr) = htons(0x1);
                resPtr += 2;

                // nsCount
                (*(unsigned short *) resPtr) = htons(0x0);
                resPtr += 2;

                // arCount
                (*(unsigned short *) resPtr) = htons(0x0);
                resPtr += 2;

                for (int i = 0; i < header.qdCount; i++) {
                    resPtr += (questions->qName.length() + 1 + 5);
                }
            }
            // 指针移至answer
            // name
            *((unsigned short *) resPtr) = htons(0xC00C);
            resPtr += 2;
            // type
            *((unsigned short *) resPtr) = ipType == TYPE_IPV4 ? htons(0x1) : htons(0x1c);
            resPtr += 2;
            // class
            *((unsigned short *) resPtr) = htons(0x1);
            resPtr += 2;
            // ttl
            *((unsigned int *) resPtr) = htonl(DNS_TTL);  // 0xe10
            resPtr += 4;
            // len
            *((unsigned int *) resPtr) = ipType == TYPE_IPV4 ? htons(0x04) : htons(0x10);
            resPtr += 2;
            // data
            if (ipType == TYPE_IPV4) {
                *((unsigned int *) resPtr) = (unsigned int) inet_addr(ip.data());
                resPtr += 4;
            } else {
                struct in6_addr addr6;
                inet_pton(AF_INET6, ip.data(), (void *) &addr6);
                memcpy(resPtr, &addr6, 16);
                resPtr += 16;
            }
        }
    }

    bool isForbidden(string &ip) {
        if (ip == "0.0.0.0" || ip == "0:0:0:0:0:0:0:0") {
            // 跳过id
            resPtr += 2;
            // 设置第二行
            (*(unsigned short *) resPtr) = htons(0x8183);
            resPtr += 2;
            // 设置四个account
            (*(unsigned short *) resPtr) = htons(0x0);
            resPtr += 2;
            (*(unsigned short *) resPtr) = htons(0x0);
            resPtr += 2;
            (*(unsigned short *) resPtr) = htons(0x0);
            resPtr += 2;
            (*(unsigned short *) resPtr) = htons(0x0);
            resPtr += 2;
            sendto(localDNSServerSocket, resMsg, 12, 0, (SOCKADDR *) &clientAddr, sizeof(clientAddr));
            return true;
        }
        return false;
    }

};


#endif //DNS_DNSMESSAGEHANDLER_H
