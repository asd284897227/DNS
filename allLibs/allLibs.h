//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_ALLLIBS_H
#define DNS_ALLLIBS_H

#define TYPE_IPV4 4
#define TYPE_IPV6 6
#define EXTERN_DNS_SERVER_IP "114.114.114.114"
#define THREAD_COUNT 100
#define BUFFER_SIZE 1024
#define MAX_QUESTION_ACCOUNT 20
#define MAX_LRU_SIZE 100
#define DNS_TTL 0x0

#pragma comment(lib, "ws2_32.lib")


#include <thread>
#include <map>
#include <string>
#include <cstring>
#include <iostream>
#include <cstring>
#include <fstream>
#include <time.h>
#include <vector>
#include "list"
#include <process.h>

#include<stdio.h>
#include<stdlib.h>

#include <winsock2.h>
#include <WS2tcpip.h>

using namespace std;

#include "../headers/ExecutionUtil.h"

#endif //DNS_ALLLIBS_H
