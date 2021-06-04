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
#define SIMULATING_DELAY 0
#define LOCAL_DNS_RULES_TTL 60 * 10


#pragma comment(lib, "ws2_32.lib")

#include<stdio.h>
#include<stdlib.h>
#include <time.h>


#include <cstring>
#include <string>
#include <iostream>
#include <fstream>


#include <thread>
#include <process.h>
#include <shared_mutex>



#include <winsock2.h>
#include <WS2tcpip.h>

#include <map>
#include <list>
#include <vector>

using namespace std;

#include "../headers/ExecutionUtil.h"

#endif //DNS_ALLLIBS_H
