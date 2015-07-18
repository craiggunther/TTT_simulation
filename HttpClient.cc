//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

#include <omnetpp.h>
#include "HttpMsg_m.h"

class HTTPClient: public cSimpleModule {

private:
    int addr;
    int srvAddr;
    int clientRole;
    int clientsCount;

protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    void sendHTTPRequest();
    void sendHTTPRequestPost();
    void sendHTTPResponseToPeer(HTTPMsg *httpMsg);
    void processHTTPReply(HTTPMsg *httpMsg);

public:
    virtual HTTPMsg *generateMessage();
    virtual void forwardMessage(HTTPMsg *httpmsg);
};

Define_Module(HTTPClient);

void HTTPClient::initialize() {

    addr = par("addr");
    srvAddr = par("srvAddr");
    clientRole = par("clientRole");
    clientsCount = par("clientsCount");

    cMessage *timer = new cMessage("timer");
    scheduleAt(simTime() + par("sendIaTime").doubleValue(), timer);
}

void HTTPClient::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        EV << "Starting service of ";
        if (clientRole == 1) {
            sendHTTPRequestPost();
        } else {
            sendHTTPRequest();
        }
        scheduleAt(simTime() + par("sendIaTime").doubleValue(), msg);

    } else {
        int src = check_and_cast<HTTPMsg *>(msg)->getSrcAddress();
        EV << "check source addr "<<src <<endl;
        if (src == 1) {
        //forwardMessage(check_and_cast<HTTPMsg *>(msg));
        // sendHTTPResponseToPeer(check_and_cast<HTTPMsg *>(msg));
            processHTTPReply(check_and_cast<HTTPMsg *>(msg));
        }
        else {

        sendHTTPResponseToPeer(check_and_cast<HTTPMsg *>(msg));
        }
    }
}
void HTTPClient::sendHTTPRequest() {
    const char *header = "GET / HTTP/1.0\r\n\r\n";

    // assemble and send HTTP request
    //HTTPMsg *httpMsg = new HTTPMsg();
   // httpMsg->setPayload(header);
   // httpMsg->setDestAddress(srvAddr);
   // httpMsg->setSrcAddress(addr);
    //  int dest = httpMsg->getDestAddress();
//    int n = gateSize("voiceUpload");
//    int k = intuniform(0,n-1);
   // send(httpMsg, "voiceP2P$o", 0);

    for (int i = 0; i < clientsCount; i++) {
            HTTPMsg *httpMsg = new HTTPMsg();
            httpMsg->setPayload(header);
            httpMsg->setSrcAddress(addr);

            httpMsg->setDestAddress(2 + i);
            EV << "Starting test ";
            send(httpMsg, "voiceP2P$o", 5);
        }
}
void HTTPClient::sendHTTPRequestPost() {
    // const char *header = "GET / HTTP/1.0\r\n\r\n";
    const char *header = "POST / HTTP/1.1\r\n\r\n";
    // assemble and send HTTP request
    HTTPMsg *httpMsg = new HTTPMsg();
    httpMsg->setPayload(header);
    httpMsg->setDestAddress(srvAddr);
    httpMsg->setSrcAddress(addr);
    send(httpMsg, "voiceUpload$o", 0);

    // int dest = httpMsg->getDestAddress();
    // int n = gateSize("voiceFromPresenter");
    // int k = intuniform(0,n-1);
    for (int i = 0; i < clientsCount; i++) {
        HTTPMsg *httpMsg = new HTTPMsg();
        httpMsg->setPayload(header);
        httpMsg->setSrcAddress(addr);

        httpMsg->setDestAddress(2 + i);

        send(httpMsg, "voiceP2P$o", 0);
    }
}
void HTTPClient::forwardMessage(HTTPMsg *httpmsg) {

    //int n = gateSize("voiceP2P");
     //  n = n/2;
    //  int k = intuniform(0,n-1);

     // EV << "Forwarding message " << httpmsg << " on port out[" << k << "]\n";
   //   send(httpmsg, "voiceP2P$o", k);

    sendHTTPResponseToPeer(httpmsg);
}
void HTTPClient::sendHTTPResponseToPeer(HTTPMsg *httpmsg) {

    const char *header = "POST / HTTP/1.1\r\n\r\n";
    int dest = httpmsg->getSrcAddress();
    int src = httpmsg->getDestAddress();
    //HTTPMsg *msg = generateMessage();
    HTTPMsg *msg = new HTTPMsg();
    msg->setPayload(header);
    msg->setDestAddress(dest);
    msg->setSrcAddress(src);

    EV << "peer" <<dest << "send packet packet to peer addr=" << src << endl;
   //if (dest >= 2 and dest < clientsCount + 1) {
        //detect if the request received from peer node, peer node address greater than 2
        msg->setDestAddress(dest);
        send(msg, "voiceP2P$o", dest);
   // }
}

HTTPMsg *HTTPClient::generateMessage() {
    // Produce source and destination addresses.
    const char *header = "POST / HTTP/1.1\r\n\r\n";
    int src = getIndex();
    int n = size();
    //int n = clientsCount;
    int dest = intuniform(0, n-2);
    if (dest >= src)
        dest++;
    // Create message object and set source and destination field.
    HTTPMsg *httpmsg = new HTTPMsg();
    httpmsg->setPayload(header);
    httpmsg->setSrcAddress(src);
    httpmsg->setDestAddress(dest);
    return httpmsg;
}

void HTTPClient::processHTTPReply(HTTPMsg *httpMsg) {

    //HTTPMsg  httpMsg =  check_and_cast<HTTPMsg *>(httpMsg);
    int dest = httpMsg->getSrcAddress();
    EV << "delete packet packet from addr=" << dest << endl;
    delete httpMsg;
}

