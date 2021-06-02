#include "allLibs/allLibs.h"
#include "headers/DNSFileHandler.h"
#include "headers/InitAllStaticField.h"
#include "headers/DNSLRU.h"

int main() {



    //·½Ê½Ò»
    DNSFileHandler handler;
    handler.readLocalIpv4DNSItem("C:\\Users\\55044\\CLionProjects\\DNS\\dnsrelay.txt");

    DNSLRU cache = DNSLRU();
    DNSNode node1("A", "a", "aa");
    DNSNode node2("B", "b", "bb");
    DNSNode node3("C", "c", "cc");

    cache.insertFirst(node1);
    cache.insertFirst(node2);
    cache.insertFirst(node3);

    string url = "A";
    DNSNode &node = cache.removeLastNode();
    cout <<node.getIpv6();


    return 0;
}
