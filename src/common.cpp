#include "include/common.h"

int getFile_type(const QString file){
    if(
        file.contains(".jpg") ||
        file.contains(".bmp") ||
        file.contains(".png") ||
        file.contains(".gif")
    ){
        return FILE_TYPE_IMAGE;
    }else if(
        file.contains(".mp4")
    ){
        return FILE_TYPE_VIDEO;
    }else{
        return FILE_TYPE_UNDEFIEND;
    }
}
DISPLAY_LIST get_normal_list(){
    DISPLAY_LIST ret_list;
    char str[100+1]={0};
     if( 0 == GetProfileString("./config/profile.conf", "normal_cnt", "cnt", str) ){
         ret_list.cmd_cnt=atoi(str);
     }
    printf("Get normal cmd cnt: [%d]\n",ret_list.cmd_cnt);
    for(int i =0; i < ret_list.cmd_cnt;i++){
        char cmd_x[100+1] = {0};
        sprintf(cmd_x,"cmd%d",i+1);
        char cmd_list_part[100+1] = {0};
        GetProfileString("./config/profile.conf", "normal_cmd", cmd_x,cmd_list_part);
        ret_list.cmd_list.append(cmd_list_part);
        printf("Get normal cmd%d:%s\n",i, cmd_list_part); 
    }
    return ret_list;
}
DISPLAY_LIST get_face_list(){
    DISPLAY_LIST ret_list;
    char str[100+1]={0};
     if( 0 == GetProfileString("./config/profile.conf", "face_cnt", "cnt", str) ){
         ret_list.cmd_cnt=atoi(str);
     }
    printf("Get face cmd cnt: [%d]\n",ret_list.cmd_cnt);
    for(int i =0; i < ret_list.cmd_cnt;i++){
        char cmd_x[100+1] = {0};
        sprintf(cmd_x,"cmd%d",i+1);
        char cmd_list_part[100+1] = {0};
        GetProfileString("./config/profile.conf", "face_cmd", cmd_x,cmd_list_part);
        ret_list.cmd_list.append(cmd_list_part);
         printf("Get face cmd%d:%s\n",i, cmd_list_part); 
    }
    return ret_list;
}
DISPLAY_LIST get_urgent_list(){
    DISPLAY_LIST ret_list;
    char str[100+1]={0};
     if( 0 == GetProfileString("./config/profile.conf", "urgent_cnt", "cnt", str) ){
         ret_list.cmd_cnt=atoi(str);
     }
    printf("Get urgent cmd cnt: [%d]\n",ret_list.cmd_cnt);
    for(int i =0; i < ret_list.cmd_cnt;i++){
        char cmd_x[100+1] = {0};
        sprintf(cmd_x,"cmd%d",i+1);
        char cmd_list_part[100+1] = {0};
        GetProfileString("./config/profile.conf", "urgent_cmd", cmd_x,cmd_list_part);
        ret_list.cmd_list.append(cmd_list_part);
         printf("Get urgent cmd%d:%s\n",i, cmd_list_part); 
    }
    return ret_list;
}

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
   
    static QMutex mutex;
    mutex.lock();
    QString text;
    switch(type)
    {
    case QtDebugMsg:
        text = QString("Debug:");
        break;
    case QtWarningMsg:
        text = QString("Warning:");
        break;
    case QtCriticalMsg:
        text = QString("Critical:");
        break;
    case QtFatalMsg:
        text = QString("Fatal:");
    }
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString current_date = QString("(%1)").arg(current_date_time);
    QString message = QString("%1 %2 %3 %4").arg(current_date).arg(text).arg(context_info).arg(msg);
    QFile file("log/cti_multiplayer.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(&file);
    text_stream << message << "\r\n";
    file.flush();
    file.close();
    mutex.unlock();
}
