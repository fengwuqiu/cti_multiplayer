#ifndef __ZMQ_SOCKET__
#define __ZMQ_SOCKET__
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <zmq.h>

typedef struct
{
    void *socket;
    void *context;
}zmqsocket_;

class ZmqSocket
{
public:
    ZmqSocket();
    ~ZmqSocket();
    void zmqversion_display();
    int create_zmqclient(const char *addr, const int timeout);
    int create_zmqserver(const char *addr, const int timeout);
    void zmq_destroy();
    int zmqsend(void *buf,int size);
    int zmqrecv(void *buf,int size);
private:
    zmqsocket_ zmqsocket;
};

#endif
