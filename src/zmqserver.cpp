#include "include/zmqserver.h"
#include <stdlib.h>

ZmqServer::ZmqServer()
{
    interrupt_received = false;
}

ZmqServer::~ZmqServer()
{
}

void ZmqServer::stop()
{
    interrupt_received = true;
}

void ZmqServer::run(void)
{
    socket.zmqversion_display();
    std::string pAddr = "tcp://*:50001";
    if(socket.create_zmqserver(pAddr.c_str(),100) < 0){
        fprintf(stderr,"error:zmq create fail.quit.\n");
        return;
    }
    
    //signal(SIGTERM, InterruptHandler);
    //signal(SIGINT, InterruptHandler);
    
    while(!interrupt_received)
    {

        //printf("zmqThread start\n");
        char szMsg[1024] = {0};
        // static int cmd_int = 0;
        // cmd_int++;
        // cmd_int %= 100;
        // char cmd[10];
        // char data[10]={0};
        // sprintf(cmd,"%d",cmd_int);
        // set_urgent_cmd_line(cmd, data);
        // printf("zmqServer set cmd: %s\n",cmd);

        //循环等待接收到来的消息，当超�?秒没有接到消息时�?        //zmq_recv函数返回错误信息 ，并使用zmq_strerror函数进行错误定位
        if(socket.zmqrecv(szMsg, sizeof(szMsg)) > 0)
        {
            fprintf(stderr,"received message : %s\n", szMsg);
            char NO[20];
            char pack[1024]={0};
            int size = procol.parseDataBuf(szMsg,NO,pack);
            fprintf(stderr,"NO:%s\n",NO);
            fprintf(stderr,"pack:%s\n",pack);
            bool getit=false;
            if(size >= 0)
            {
                getit = true;
                char cmd[10], data[50];
                int ret = 0;
                ret = procol.getData(pack,cmd,data);
                if(0 == ret)
                {
                    fprintf(stderr,"cmd(%s)\n",cmd);
                    fprintf(stderr,"data(%s)\n",data);
                    set_urgent_cmd_line(cmd, data);
                }
            }
            char resdata[100]={0};
            size = procol.resPackData(resdata,NO,getit?"OK":"ERROR");
            socket.zmqsend(resdata,size);
        }
	    usleep(10000);
        //printf("zmqThread end\n");
    }
    socket.zmq_destroy();
}
