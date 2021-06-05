//
// Created by 55044 on 2021/6/5.
//

#ifndef DNS_DNSCACHENODE_H
#define DNS_DNSCACHENODE_H

class DNSCacheNode {
    string url;
    char ipv4[BUFFER_SIZE];
    char ipv6[BUFFER_SIZE];
    int ipv4Len = 0;
    int ipv6Len = 0;
    std::time_t timestamp;

public:
    DNSCacheNode() {}

    DNSCacheNode(const string &url) : url(url) {
        timestamp = std::time(0);
    }


    string getTypeOfNode() {
        if (!this->isIpv4Empty() && this->isIpv6Empty())
            return "ipv4";
        else if (this->isIpv4Empty() && !this->isIpv6Empty())
            return "ipv6";
        else if (!this->isIpv4Empty() && !this->isIpv6Empty())
            return "ipv4 / ipv6";
        else {
            ExecutionUtil::fatalError("DNSCacheNode缓存出错！");
            return "";
        }
    }

    int getIpv4Len() const {
        if (std::time(0) - timestamp >= LRU_TTL * 60 * 1000)
            return 0;
        else
            return ipv4Len;
    }

    void setIpV4(char msg[BUFFER_SIZE], int len) {
        memcpy(ipv4, msg, len);
        ipv4Len = len;
    }

    void setIpV6(char msg[BUFFER_SIZE], int len) {
        memcpy(ipv6, msg, len);
        ipv6Len = len;
    }


    int getIpv6Len() const {
        if (std::time(0) - timestamp >= LRU_TTL * 60 * 1000)
            return 0;
        else
            return ipv6Len;
    }

    const string &getUrl() const {
        return url;
    }

    void setUrl(const string &url) {
        DNSCacheNode::url = url;
    }

    const char *getIpv4() const {
        return ipv4;
    }

    const char *getIpv6() const {
        return ipv6;
    }

    bool isIpv4Empty() const {
        return getIpv4Len() == 0;
    }


    bool isIpv6Empty() const {
        return getIpv6Len() == 0;
    }

    time_t getTimestamp() const {
        return timestamp;
    }

    void setTimestamp(time_t timestamp) {
        DNSCacheNode::timestamp = timestamp;
    }
};

#endif //DNS_DNSCACHENODE_H
