//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//
// all the EV print is for debugging purpose, please delete them prior to run the real simulation.

#include <map>
#include <omnetpp.h>
#include "HttpMsg_m.h"


class HTTPClient: public cSimpleModule {

private:
    int addr;
    int srvAddr;
    int clientRole;
    int clientsCount;
    simtime_t propDelay;
    // signals
    simsignal_t endToEndDelaySignal;
    simsignal_t hopCountSignal;
    simsignal_t sourceAddressSignal;

    typedef std::map<int,int> RoutingTable; // destaddr -> gateindex
       RoutingTable rtable;
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

    // initialize the parameter and schedule self-message
    addr = par("addr");
    srvAddr = par("srvAddr");
    clientRole = par("clientRole");
    clientsCount = par("clientsCount");

     propDelay = (double)par("propDelay");
     cMessage *timer = new cMessage("timer");
     scheduleAt(simTime() + par("sendIaTime").doubleValue(), timer);
     //register signal for analysis
     endToEndDelaySignal = registerSignal("endToEndDelay");
     hopCountSignal =  registerSignal("hopCount");
     sourceAddressSignal = registerSignal("sourceAddress");

}

void HTTPClient::handleMessage(cMessage *msg) {
    //if self-message ,then check the role, the presenter role will send post message(in first phase, post message means include contents),
    //otherwise will send get message (get message means request)

    if (msg->isSelfMessage()) {
        if (clientRole == 1) {
            EV << "Client presenter Starting service of ";
            sendHTTPRequestPost();
        } else {
            EV << "Client students Starting service of ";

            sendHTTPRequest();
        }
        //scheduled message for trigger presenter to send content.
        scheduleAt(simTime() + par("sendIaTime").doubleValue(), msg);

    } else {
        // if not a sefl-message, which message received messge from other nodes, will check src address match 1(presenter)or 0(server),
        // delete them if so, otherwise  call sendtopeer function to send post message to response.
        // Handle incoming packet stat
        int src = check_and_cast<HTTPMsg *>(msg)->getSrcAddress();
        EV << "received packet " << endl;
        EV <<"the packet come from src addr is " << src<<endl;

        emit(endToEndDelaySignal, simTime() - msg->getCreationTime());
        //emit(hopCountSignal, msg->getHopCount());
        emit(sourceAddressSignal, check_and_cast<HTTPMsg *>(msg)->getSrcAddress());
        if (src == 1 or src == 0) {
            //if the message received from presenter and cloud server, we just simply delete them as this is a normal module.
            EV <<"test to delete message " << endl;
            processHTTPReply(check_and_cast<HTTPMsg *>(msg));

        }
        else {
            //get the input gate info from arrived message.
            cGate *gate = msg->getArrivalGate();
            EV <<"Gate name is : "<< gate->getFullName() <<endl;
            sendHTTPResponseToPeer(check_and_cast<HTTPMsg *>(msg));
        }
    }
}
void HTTPClient::sendHTTPRequest() {
// the method will send "get " type message to peer node to simulate as request. the current message will send to random gate
// the method will take self message as parameter to trigger the process
    const char *header = "GET / HTTP/1.0\r\n\r\n";

  //  k means dest address, so the address will start from 2 to number of peer
    int k = intuniform(2,clientsCount-1);
    EV <<"random number is : "<< k <<endl;

    EV << " addr is " << addr <<endl;
    // this is a self message, so this addr is the sender's own address, if the address is 3 which means the module is stu1. we always minus 2 to calculate the peer.
    int gateindex  = addr -2;

    // random number k will be assigned to dest address,this also is the stu [3].
    int destAddr = k;

    EV << "module name is  stu [ " << gateindex << "]"<<endl;
    // assemble and send HTTP request
    HTTPMsg *httpMsg = new HTTPMsg();
    httpMsg->setPayload(header);
    httpMsg->setDestAddress(destAddr);
    httpMsg->setSrcAddress(addr);

   //broadcasting message to other node
   // for (int i=0; i<n;i++){
    //    HTTPMsg *copy =httpMsg->dup();
    //    send(copy,"voiceP2P$o",i);
    //}
    if (k!=gateindex) {
        EV <<"gateindex1 is :"<< k <<endl;
        send(httpMsg, "voiceP2P$o", k);
    }
    else {

        int newaddr = abs(k-1);
        if (newaddr==addr){
            EV <<"this is the bug, it will cause the src and dest addr become to same value, so we just simply make dest addr plus 1."<<endl;
            newaddr = newaddr+1;
        }
        EV <<"gateindex2 is :"<< newaddr <<endl;
        httpMsg->setDestAddress(newaddr);
        send(httpMsg, "voiceP2P$o", abs(k-1));
    }

    //for (int i = 0; i < clientsCount; i++) {
    //        HTTPMsg *httpMsg = new HTTPMsg();
  //          httpMsg->setPayload(header);
    //        httpMsg->setSrcAddress(addr);

    //        httpMsg->setDestAddress(2 + i);
    //        EV << "Starting test ";
    //        send(httpMsg, "voiceP2P$o", 5);
    //    }
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

    for (int i = 0; i < clientsCount; i++) {
        HTTPMsg *httpMsg = new HTTPMsg();
        httpMsg->setPayload(header);
        httpMsg->setSrcAddress(addr);
        httpMsg->setDestAddress(2 + i);
        send(httpMsg, "voiceP2P$o", 0);
       // HTTPMsg *httpMsgnew = new HTTPMsg();
        //httpMsgnew->setPayload(header);
        //httpMsgnew->setSrcAddress(addr);

        //httpMsgnew->setDestAddress(2 + i);
       //          EV << "Test for Presenter2"<< endl;
      //  sendDelayed(httpMsgnew, propDelay,"voiceP2P$o", 1);
    }
}
void HTTPClient::sendHTTPResponseToPeer(HTTPMsg *httpmsg) {

    const char *header = "POST / HTTP/1.1\r\n\r\n";
    int destaddr = httpmsg->getDestAddress();
   // int srcaddr = httpmsg->getSrcAddress();
// the destination addr minus 2 is the peer name for example, is destaddr is 5, then this module is stud3
    HTTPMsg *msg = new HTTPMsg();
    msg->setPayload(header);
    //int n = gateSize("voiceP2P");
    int k = intuniform(2,clientsCount-1);

//the gate index is indicator of module name, for example, if index is 3 mean student[3] ,so the gate 3 is not connected to other node.
     int gateindex  = destaddr ;
     EV << "module name is  stu [ " << gateindex << "]"<<endl;

     int ownAddr  = destaddr;
     int peerAddr = k;
     EV << "No11 peer Addr " <<peerAddr<<endl;
     EV << "No22 ownAddr: " << ownAddr <<endl;
     msg->setDestAddress(peerAddr);
     msg->setSrcAddress(ownAddr);
// this will detect if the out gate index number same as moudule name, if so, this gate does not connect to other gate,we need to do some modify.
   if (k != gateindex ) {

        EV << "peer stu[" <<gateindex <<"]"<< "send packet to peer addr=" << k << endl;

        send(msg, "voiceP2P$o", k);
    }
   else {
     int  newdest = abs(k - 1);
       msg->setDestAddress(newdest);
      EV << " one peer" <<gateindex << "send packet to peer addr=" << newdest << endl;
       send(msg, "voiceP2P$o", abs(k-1));
   }
}

HTTPMsg *HTTPClient::generateMessage() {
    // Produce source and destination addresses.
    const char *header = "POST / HTTP/1.1\r\n\r\n";
    int n = gateSize("voiceP2P");
    int k = intuniform(2,n/2);
    // Create message object and set source and destination field.
    HTTPMsg *httpmsg = new HTTPMsg();
    httpmsg->setPayload(header);
    httpmsg->setSrcAddress(n);
    httpmsg->setDestAddress(k);
    return httpmsg;
}

void HTTPClient::processHTTPReply(HTTPMsg *httpMsg) {

    int dest = httpMsg->getSrcAddress();
    EV << "delete packet from addr=" << dest << endl;
    delete httpMsg;
}

void HTTPClient::forwardMessage(HTTPMsg *httpmsg) {

     // EV << "Forwarding message " << httpmsg << " on port out[" << k << "]\n";
   //   send(httpmsg, "voiceP2P$o", k);

    sendHTTPResponseToPeer(httpmsg);
}
