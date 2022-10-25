#ifndef __PROTOCOL__
#define __PROTOCOL__

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

//协议帧头
#define HEAD_PROTOCOL  "#"
#define TAIL_PROTOCOL  "&"
#define MAX_SIZE       (1024)

class Protocol
{
public:
    Protocol();
    ~Protocol();
    int resPackData(char *msg, const char *NO, const char *data);
    int packData(char *msg, const unsigned int cmd, const char *data=NULL);
    int parseDataBuf(char *msg, char *NO, char *data);
    int getData(char *msg, char *cmd, char *data);
    void getstimeval(char *buf);
private:
    void split(char *src,const char *separator,char **dest,int *num);
    int  makePackData(char *datapack, const char *NO, const char *data);

};


#endif
