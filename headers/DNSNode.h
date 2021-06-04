//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSNODE_H
#define DNS_DNSNODE_H

#include "../allLibs/allLibs.h"

/**
 * DNS节点类
 */
class DNSNode {
public:
    string url;
    string ipv4;
    string ipv6;
    std::time_t timestamp;

    DNSNode() {
        timestamp = std::time(0);
    }

    DNSNode(const string &url, const string &ipv4, const string &ipv6) : url(url), ipv4(ipv4), ipv6(ipv6) {
        timestamp = std::time(0);
    }

    const string &getUrl() const {
        return url;
    }

    void setUrl(const string &url) {
        this->url = url;
    }

    void setIpv4(const string &ipv4) {
        this->ipv4 = ipv4;
        timestamp = std::time(0);
    }

    string getIpv6OfCache() const {
        if (std::time(0) - timestamp >= LRU_TTL * 60)
            return "";
        else
            return ipv6;
    }


    string getIpv4OfCache() const {
        if (std::time(0) - timestamp >= LRU_TTL * 60)
            return "";
        else
            return ipv4;
    }

    string getIpv6() const {
        return ipv6;
    }

    string getIpv4() const {
        return ipv4;
    }

    void setIpv6(const string &ipv6) {
        this->ipv6 = ipv6;
        timestamp = std::time(0);
    }
};

#endif //DNS_DNSNODE_H
