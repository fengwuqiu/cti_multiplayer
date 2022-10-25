#ifndef ZMQSERVER_H
#define ZMQSERVER_H

#include <stdio.h>

using namespace std;
#include <string>
#include "protocol.h"
#include "zmq_socket.h"
#include <signal.h>
#include <QThread>

extern void set_urgent_cmd_line(char *cmd, char *data);
//extern void InterruptHandler(int signo);

class ZmqServer : public QThread
{
public:
    ZmqServer();
    ~ZmqServer();
    void stop();
protected:
   virtual void run(void);
private:
   ZmqSocket socket;
   Protocol  procol;
   volatile bool interrupt_received;
};

#endif