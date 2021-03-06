//
// Generated file, do not edit! Created by nedtool 4.6 from HttpMsg.msg.
//

#ifndef _HTTPMSG_M_H_
#define _HTTPMSG_M_H_

#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0406
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



// cplusplus {{
#include "NetPkt_m.h"
// }}

/**
 * Class generated from <tt>HttpMsg.msg:25</tt> by nedtool.
 * <pre>
 * //
 * // Message class for representing a HTTP request or reply.
 * // For purposes of this model, we employ a very simple notion of HTTP:
 * // every HTTP request and reply is modelled with a single HTTPMsg packet.
 * // (We don't care about TCP connection setup and teardown, about TCP acks,
 * // and also ignore the fact that long requests and replies may span
 * // several packets in real life).
 * //
 * packet HTTPMsg extends NetPkt
 * {
 *     string payload;
 * }
 * </pre>
 */
class HTTPMsg : public ::NetPkt
{
  protected:
    opp_string payload_var;

  private:
    void copy(const HTTPMsg& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const HTTPMsg&);

  public:
    HTTPMsg(const char *name=NULL, int kind=0);
    HTTPMsg(const HTTPMsg& other);
    virtual ~HTTPMsg();
    HTTPMsg& operator=(const HTTPMsg& other);
    virtual HTTPMsg *dup() const {return new HTTPMsg(*this);}
    virtual void parsimPack(cCommBuffer *b);
    virtual void parsimUnpack(cCommBuffer *b);

    // field getter/setter methods
    virtual const char * getPayload() const;
    virtual void setPayload(const char * payload);
};

inline void doPacking(cCommBuffer *b, HTTPMsg& obj) {obj.parsimPack(b);}
inline void doUnpacking(cCommBuffer *b, HTTPMsg& obj) {obj.parsimUnpack(b);}


#endif // ifndef _HTTPMSG_M_H_

