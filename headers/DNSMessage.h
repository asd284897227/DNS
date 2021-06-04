//
// Created by wangwei on 2021/6/2.
//

#ifndef DNS_DNSMESSAGE_H
#define DNS_DNSMESSAGE_H

class DNSHeader {
public:
    unsigned short id;
    unsigned short row2;
    unsigned short qdCount = 0x0;
    unsigned short anCount = 0x0;
    unsigned short nsCount = 0x0;
    unsigned short arCount = 0x0;

    unsigned char getQR() {
        return 0x8000 & row2 >> 15;
    }

    unsigned char getOpcode() {
        return 0x7800 & row2 >> 11;
    }

    unsigned char getAA() {
        return 0x0400 & row2 >> 10;
    }

    unsigned char getTC() {
        return 0x0200 & row2 >> 9;
    }

    unsigned char getRD() {
        return 0x0100 & row2 >> 8;
    }

    unsigned char getRA() {
        return 0x0080 & row2 >> 7;
    }

    unsigned char getZ() {
        return 0x0070 & row2 >> 4;
    }

    unsigned char getRCode() {
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
        unsigned char a = ptr[0];
        unsigned char b = ptr[1];
        ptr += 2;
    }

    void setQClassFromPtr(char *&ptr) {
        this->qClass = ntohs(*(unsigned short *) ptr);
        ptr += 2;
    }

    unsigned char getQType() {
        return qType;
    }

    unsigned short getQClass() const {
        return qClass;
    }
};

class DNSResourceRecord {
    unsigned short name;
    unsigned short type;
    unsigned short clazz;
    unsigned int ttl;
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
