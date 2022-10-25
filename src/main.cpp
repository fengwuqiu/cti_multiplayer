#include "include/common.h"
#include "include/kvconf.h"
#include <QApplication>
#include <QHBoxLayout>
#include "include/mainwindow.h"
#include "include/zmqserver.h"


volatile bool interrupt_received = false;
bool m_stop_ = false;
void set_urgent_cmd_line(char *cmd, char *data);

int main(int argc, char *argv[])
{
    //app
    QApplication app(argc,argv);
    //注册log
    qInstallMessageHandler(outputMessage);
    qDebug()<<"-------------------------";
    qDebug()<<"cti_multiplayer start now!";
    qDebug()<<"-------------------------";
    //实例化
    MainWindow mainwindow;
    //从profile.conf 中解析预置播放命令
    DISPLAY_LIST normal_list;
    DISPLAY_LIST face_list;
    DISPLAY_LIST urgent_list;
    normal_list = get_normal_list();
    face_list = get_face_list();
    urgent_list = get_urgent_list();
    //导入预置播放命令
    mainwindow.loadDefaultCmd(normal_list.cmd_list, face_list.cmd_list, urgent_list.cmd_list);
    mainwindow.run();
    //开启zmq服务
    ZmqServer* zmqServer_ = new ZmqServer;
    zmqServer_->start();
    return app.exec();
}