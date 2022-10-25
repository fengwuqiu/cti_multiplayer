//#include "ledshow/protocol.h"
#include "include/protocol.h"

Protocol::Protocol()
{
}
Protocol::~Protocol()
{
}

void Protocol::getstimeval(char *buf)
{
    struct timeval tv;
    struct tm      tm;
    unsigned int len=0;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &tm);
    len=sprintf(buf, "%04d%02d%02d%02d%02d%02d", tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    sprintf(buf+len, "%03d", (int)(tv.tv_usec/1000));
}

int Protocol::makePackData(char *datapack, const char *NO, const char *data)
{
    unsigned char crc=0;
    if(datapack==NULL)
    {
        return -1;
    }
    for (unsigned int i=0; i<strlen(data); i++)
    {
        crc += data[i];
    }
    int size = 0;
    size += sprintf(datapack+size,"%s",HEAD_PROTOCOL);
    size += sprintf(datapack+size,"NO=%s",NO);
    size += sprintf(datapack+size,"{%s}",data);
    size += sprintf(datapack+size,"%d",crc);
    size += sprintf(datapack+size,"%s",TAIL_PROTOCOL);
    return size;
}
int Protocol::packData(char *msg, const unsigned int cmd, const char *data)
{
    int  len = (data==NULL)?5:(strlen(data)+5);
    char databuf[10+len];
    unsigned int n=0;
    n = sprintf(databuf,"CMD=%d",cmd);
    if(data)
    {
        n += sprintf(databuf+n,",DATA=%s",data);
    }
    char time_no[100];
    getstimeval(time_no);
    int size = makePackData(msg, time_no, databuf);
    return size;
}
int Protocol::resPackData(char *msg, const char *NO, const char *data)
{
    int  len = strlen(data);
    char databuf[len];
    sprintf(databuf,"%s",data);
    int size = makePackData(msg, NO, databuf);
    return size;
}

/*
src 源字符串的首地址(buf的地址)
separator 指定的分割字符
dest 接收子字符串的数组
num 分割后子字符串的个数
*/
void Protocol::split(char *src,const char *separator,char **dest,int *num)
{
    int count = 0;
    if (src == NULL || strlen(src) == 0) //如果传入的地址为空或长度为0，直接终止
        return;
    if (separator == NULL || strlen(separator) == 0) //如未指定分割的字符串，直接终止
        return;
    char *pNext = (char *)strtok(src, separator); //
    while(pNext != NULL)
    {
        *dest++ = pNext;
        ++count;
        pNext = (char *)strtok(NULL,separator); //必须使用(char *)进行强制类型转换
    }
    *num = count;
}

int Protocol::parseDataBuf(char *msg, char *NO, char *data)
{
    int ret = -1;
    unsigned int no_len=0,data_len=0,crc_len=0;
    unsigned int checkcrc=0;

    if(!msg || !NO || !data){
        return ret;
    }
    char *start_temp = strchr(msg,'#');
    char *end_temp = strchr(msg,'&');
    char *start_data = strchr(msg,'{');
    char *end_data = strchr(msg,'}');
    if(start_temp && end_temp && start_data && end_data)
    {
        no_len = (unsigned int)(start_data-start_temp);
        crc_len = (unsigned int)(end_temp-end_data);
        data_len = (unsigned int)(end_data-start_data);
        //printf("no_len=%d;crc_len=%d;data_len=%d\n",no_len,crc_len,data_len);
        if(no_len > 0 && crc_len > 0 && data_len > 0)
        {
            //获取序列号
            char data_no[no_len];
            strncpy(data_no, start_temp+1, no_len);
            data_no[no_len-1]='\0';
            sscanf(data_no,"NO=%s",NO);
            //printf("NO(%s)\n",NO);
            //获取crc
            char data_crc[crc_len];
            strncpy(data_crc, end_data+1, crc_len);
            data_crc[crc_len-1]='\0';
            sscanf(data_crc,"%d",&checkcrc);
            //printf("checkcrc(%d)\n",checkcrc);
            //获取数据
            char data_data[data_len];
            strncpy(data_data, start_data+1, data_len);
            data_data[data_len-1]='\0';
            sscanf(data_data,"%s",data);
            //printf("data_data(%s)\n",data_data);
            //校验CRC
            unsigned char crc=0;
            for(unsigned int cnt=0;cnt<strlen(data_data);cnt++){
                crc += data_data[cnt];
            }
            //printf("checkcrc(%d) ?= crc(%d)\n",checkcrc,crc);
            if(checkcrc == crc)
            {
                ret = (data_len-1);
            }
        }
    }
    return ret;
}

int Protocol::getData(char *msg, char *cmd, char *data)
{
    int ret = -1;
    char *pNext = (char *)strtok(msg, ","); //
    while(pNext != NULL)
    {
        char *value = NULL;
        if((value = strstr(pNext,"CMD=")) != NULL)
        {
            strncpy(cmd, value+4, strlen(value)-4);
            cmd[strlen(value)-4]='\0';
            ret = 0;
        }
        if((value  = strstr(pNext,"DATA=")) != NULL)
        {
            strncpy(data, value+5, strlen(value)-5);
            data[strlen(value)-5]='\0';
            ret = 0;
        }
        pNext = (char *)strtok(NULL,","); //必须使用(char *)进行强制类型转换
    }
    return ret;
}
