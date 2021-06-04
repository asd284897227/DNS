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
    bool isIpv4Empty = true;

    string ipv6;
    bool isIpv6Empty = true;


    DNSNode() {
    }

    DNSNode(bool isEmpty) {
        this->isIpv4Empty = isEmpty;
    }

    DNSNode(const string &url, const string &ipv4, const string &ipv6) : url(url), ipv4(ipv4), ipv6(ipv6) {
        if (!ipv4.empty()) this->isIpv4Empty = false;
        else if (!ipv6.empty()) this->isIpv6Empty = false;
    }

    const string &getUrl() const {
        return url;
    }

    void setUrl(const string &url) {
        this->url = url;
    }

    const string &getIpv4() const {
        return ipv4;
    }

    void setIpv4(const string &ipv4) {
        this->ipv4 = ipv4;
        if (!ipv4.empty())this->isIpv4Empty = false;
    }

    const string &getIpv6() const {
        return ipv6;
    }

    void setIpv6(const string &ipv6) {
        this->ipv6 = ipv6;
        if (!ipv6.empty())this->isIpv6Empty = false;
    }
};

#endif //DNS_DNSNODE_H
