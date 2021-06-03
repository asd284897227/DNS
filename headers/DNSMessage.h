//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_DNSMESSAGE_H
#define DNS_DNSMESSAGE_H

class DNSHeader {
public:
    unsigned short id;
    unsigned short row2;
    unsigned short qdCount;
    unsigned short anCount;
    unsigned short nsCount;
    unsigned short arCount;

    unsigned char getQR(unsigned short row2) {
        return 0x8000 & row2;
    }

    unsigned char getOpcode(unsigned short row2) {
        return 0x7800 & row2;
    }

    unsigned char getAA(unsigned short row2) {
        return 0x0400 & row2;
    }

    unsigned char getTC(unsigned short row2) {
        return 0x0200 & row2;
    }

    unsigned char getRD(unsigned short row2) {
        return 0x0100 & row2;
    }

    unsigned char getRA(unsigned short row2) {
        return 0x0080 & row2;
    }

    unsigned char getZ(unsigned short row2) {
        return 0x0070 & row2;
    }

    unsigned char getRcode(unsigned short row2) {
        return 0x000F & row2;
    }
};


class DNSQuestion {
public:
    string qName;
    unsigned short qType;
    unsigned short qClass;

    void convertQnameToStdUrl(char *&begin) {
        string url = "";
        unsigned char len;
        while ((len = *begin++) != 0x00) {
            while (len--)
                url = url + (*begin++);
            url = url + '.';
        }
        url.pop_back();
        this->qName = url;
    }

    void setQTypeFromPtr(char *&ptr) {
        this->qType = ntohs(*(unsigned short *) ptr);
        ptr += 2;
    }

    void setQClassFromPtr(char *&ptr) {
        this->qClass = ntohs(*(unsigned short *) ptr);
        ptr += 2;
    }
};

class DNSResourceRecord {
    unsigned short name;//һ��ΪC00C
    unsigned short type;
    unsigned short clazz;
    int ttl;
    unsigned short rdLength;
    char *rData;
};

class DNSMessage {
    DNSHeader header;
    DNSQuestion question;
    DNSResourceRecord answer;
    DNSResourceRecord authority;
    DNSResourceRecord additional;
} Message;

#endif //DNS_DNSMESSAGE_H
