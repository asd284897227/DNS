//
// Created by 55044 on 2021/6/2.
//

#ifndef DNS_DNSNODE_H
#define DNS_DNSNODE_H
#include "../allLibs/allLibs.h"
/**
 * DNS¼ÇÂ¼ÔªËØ
 */
class DNSNode {
public:
    string url;
    string ipv4;
    string ipv6;

    DNSNode() {}

    DNSNode(const string &url, const string &ipv4, const string &ipv6) : url(url), ipv4(ipv4), ipv6(ipv6) {}

    const string &getUrl() const {
        return url;
    }

    void setUrl(const string &url) {
        DNSNode::url = url;
    }

    const string &getIpv4() const {
        return ipv4;
    }

    void setIpv4(const string &ipv4) {
        DNSNode::ipv4 = ipv4;
    }

    const string &getIpv6() const {
        return ipv6;
    }

    void setIpv6(const string &ipv6) {
        DNSNode::ipv6 = ipv6;
    }
};
#endif //DNS_DNSNODE_H
