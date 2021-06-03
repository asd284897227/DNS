//
// Created by 55044 on 2021/6/3.
//
#include "../allLibs/allLibs.h"
#ifndef DNS_THREADPARAMS_H
#define DNS_THREADPARAMS_H
class TheadTaskParams {
    SOCKET &localDnsServerSocket;
    SOCKADDR_IN &clientAddr;
    int len;
};
#endif //DNS_THREADPARAMS_H
