//#include "ledshow/zmq_socket.h"
#include "include/zmq_socket.h"

ZmqSocket::ZmqSocket()
{
    zmqsocket.socket=NULL;
    zmqsocket.context=NULL;
}

ZmqSocket::~ZmqSocket()
{
    zmq_destroy();
}
void ZmqSocket::zmqversion_display()
{
    int major, minor, patch;
    zmq_version (&major, &minor, &patch);
    printf ("Current ØMQ version is %d.%d.%d\n", major, minor, patch);
}

int ZmqSocket::create_zmqclient(const char *addr, const int timeout)
{
    //创建context
    if((zmqsocket.context = zmq_ctx_new()) == NULL)
    {
        fprintf(stderr, "zmq_ctx_new faild\n");
        return -1;
    }
    //创建socket
    if((zmqsocket.socket = zmq_socket(zmqsocket.context, ZMQ_DEALER)) == NULL)
    {
        fprintf(stderr, "zmq_socket faild\n");
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    int iSndTimeout = timeout;// millsecond
    //设置接收超时
    if(zmq_setsockopt(zmqsocket.socket, ZMQ_RCVTIMEO, &iSndTimeout, sizeof(iSndTimeout)) < 0)
    {
        fprintf(stderr, "zmq_setsockopt faild\n");
        zmq_close(zmqsocket.socket);
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    //连接目标ip，端口
    if(zmq_connect(zmqsocket.socket, addr) < 0)
    {
        fprintf(stderr, "zmq_connect faild\n");
        zmq_close(zmqsocket.socket);
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    return 0;
}
int ZmqSocket::create_zmqserver(const char *addr, const int timeout)
{
    //创建context，zmq的socket 需要在context上进行创建
    if((zmqsocket.context=zmq_ctx_new())==NULL){
        fprintf(stderr,"error:zmq_ctx_new fail.\n");
        return -1;
    }
    //创建zmq socket ，socket目前有6中属性 ，这里使用dealer方式
    if((zmqsocket.socket = zmq_socket(zmqsocket.context, ZMQ_DEALER)) == NULL){
        fprintf(stderr,"error:zmq_socket fail.\n");
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    int iRcvTimeout = timeout;// millsecond
    //设置zmq的接收超时时间为5秒
    if(zmq_setsockopt(zmqsocket.socket, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0){
        fprintf(stderr,"error:zmq_setsockopt fail.\n");
        zmq_close(zmqsocket.socket);
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    //绑定地址
    //也就是使用tcp协议进行通信，网络端口
    if(zmq_bind(zmqsocket.socket, addr) < 0){
        fprintf(stderr,"error:zmq_bind fail.\n");
        zmq_close(zmqsocket.socket);
        zmq_ctx_destroy(zmqsocket.context);
        return -1;
    }
    printf("bind at : %s\n", addr);
    return 0;
}
void ZmqSocket::zmq_destroy()
{
    if(zmqsocket.socket){
        zmq_close(zmqsocket.socket);
    }
    if(zmqsocket.context){
        zmq_ctx_destroy(zmqsocket.context);
    }
}
int ZmqSocket::zmqsend(void *buf,int size)
{
    if(zmqsocket.socket == NULL){
        return -1;
    }
    return zmq_send(zmqsocket.socket, buf, size, 0);
}
int ZmqSocket::zmqrecv(void *buf,int size)
{
    if(zmqsocket.socket == NULL){
        return -1;
    }
    return zmq_recv(zmqsocket.socket, buf, size, 0);
}

