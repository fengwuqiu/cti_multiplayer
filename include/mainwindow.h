#ifndef MAIMWINDOW_H
#define MAIMWINDOW_H
#pragma once

#include <QWidget>
#include <QMovie>
#include <QTimer>
#include <QLabel>
#include "kvconf.h"
#include <QTimer>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "common.h"
#include <QtWidgets>
#include <QVideoWidget>
#include <QImage>
#include <QColor>
#include <unistd.h> 
#include <stdlib.h>

#define main_window_H 1920
#define main_window_W 1080
#define label_carstate_H 10
#define label_carstate_W 180
#define label_rolltext_H 10
#define label_rolltext_W 1080
#define center_comp_H_min 940
#define center_comp_W_min 540
#define center_comp_H_max 1900
#define center_comp_W_max 1080
#define MONITOR_SIZE 10

extern void set_urgent_cmd_line(char *cmd, char *data);
struct URGENT_CMD{
    int cmd_id{1000};
    QString cmd_str{""};  
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow(){};
    //加载预置命令
    bool loadDefaultCmd(const QList<QString> &normal_cmd_list,const QList<QString> &face_cmd_list,const QList<QString> &urgent_cmd_list);
    //界面展示
    bool run();
private:

    //**************中央界面**************//
    QWidget* centerWindow_;
    //**************车辆状态**************//
    QWidget* stateWindow_;//状态界面
    QLabel* power_title_label_; //电量标题
    QLabel* power_show_label_;//电量显示
    QLabel* health_title_label_;//车辆状态标题
    QLabel* health_show_label_;//车辆状态显示
    QLabel* signal_title_label_;//网络信号标题
    QLabel* signal_show_label_;//网络信号显示
    //**************轮播**************//
    QWidget* playWindow_;//播放界面
    QLabel* normal_label_; //轮播,图片,文字,动图
    QMovie* normal_movie_;
    QList<QString> normal_cmd_list_; //预置轮播命令
    QMediaPlayer* normal_mediaplayer_; //轮播视频
    QVideoWidget* normal_videowidget_; //轮播视频
    QMediaPlaylist* normal_playlist_;
    QTimer* normal_timer_; //轮播定时器
    int currentMovieIndex_;//轮播序号
    int normal_file_type_; 
    bool normal_play_forever_; //无限播放
    //**************插播**************//
    QLabel* urgent_label_; //插播,图片,文字,动图
    QMovie* urgent_movie_;
    QList<QString> urgent_cmd_list_; //预置插播紧急命令
    QList<QString> face_cmd_list_; //预置插播表情命令
    QMediaPlayer* urgent_mediaplayer_; //插播视频
    QVideoWidget* urgent_videowidget_; //插播视频
    QMediaPlaylist* urgent_playlist_;
    QTimer* urgent_timer_; //插播定时器
    bool urgent_play_forever_; //无限播放
    //**************滚动字幕**************//
    QWidget* rolltextWindow_;//字幕界面
    QLabel* roll_text_label_; //滚动字幕显示
    QTimer* roll_text_timer_; //滚动字幕定时器
    QString roll_text_content_;//滚动字幕内容
    //**************布局控件**************//
    QHBoxLayout *Hlay_carstate_;//车辆状态信息水平布局
    QStackedLayout* Slay_play_;//播放界面堆叠布局
    QHBoxLayout *Hlay_rolltext_;//滚动字母水平布局
    int Hlay_rolltext_height_;//滚动字母水平布局高度
    QVBoxLayout *Vlay_main_;//主布局,整体垂直布局
    int normal_label_index_;//堆布局，轮播图面界面索引
    int normal_videowidget_index_;//堆布局，轮播视频界面索引
    int urgent_label_index_;//堆布局，插播图面界面索引
    int urgent_videowidget_index_;//堆布局，插播视频界面索引

    std::chrono::steady_clock::time_point cmd_end;  //结束时间

    //初始化成员变量
    void iniMember();
    //初始化布局
    void iniUI();
    //信号与槽
    void iniSignalSlots();
    //预置命令解析
    int parseCMD(QString cmd, QString& filePath, QString key);
    //设置屏幕亮度的功能
    char monitor[MONITOR_SIZE];
    void getMonitor(char *monitor);
    void setBrightness(int val);
    //亮度测试定时器
    QTimer* brightness_test_timer_; //滚动字幕定时器
private slots:
    void onNormalTimeout(); //轮播定时器
    void onUrgentTimeout(); //插播定时器
    void onRolltextTimeout(); //滚动字幕定时器
    void onBrightnessTimeout(); //插播定时器
};

#endif