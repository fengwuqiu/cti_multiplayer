#include "include/mainwindow.h"

URGENT_CMD urgent_cmd_;
std::chrono::steady_clock::time_point cmd_start; //开始时间

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    iniMember();
    iniUI();
    iniSignalSlots();
    currentMovieIndex_ = 0;
    normal_play_forever_ = false;
    urgent_play_forever_ = false;
    cmd_start = std::chrono::steady_clock::now();
}
void MainWindow::iniMember()
{
    //中心界面
    centerWindow_ = new QWidget(this);
    centerWindow_->setMaximumSize(main_window_W, main_window_H); //设置最大为 1080*1920
    this->setCentralWidget(centerWindow_);
    //轮播
    playWindow_ = new QWidget(this);
    normal_label_ = new QLabel;
    normal_label_->setStyleSheet("background-color: rgb(0, 0, 0);"); //背景色
    normal_label_->setScaledContents(1);                             //自动适应图片
    normal_movie_ = new QMovie;
    normal_mediaplayer_ = new QMediaPlayer;
    normal_videowidget_ = new QVideoWidget;
    normal_videowidget_->setAttribute(Qt::WA_OpaquePaintEvent);            //避免播放视频闪烁
    normal_videowidget_->setStyleSheet("background-color: rgb(0, 0, 0);"); //背景色
    normal_playlist_ = new QMediaPlaylist;
    normal_timer_ = new QTimer;
    //插播
    urgent_label_ = new QLabel;
    urgent_movie_ = new QMovie;
    urgent_label_->setStyleSheet("background-color: rgb(0, 0, 0);"); //背景色
    urgent_label_->setScaledContents(1);                             //自动适应图片
    urgent_mediaplayer_ = new QMediaPlayer;
    urgent_videowidget_ = new QVideoWidget;
    urgent_videowidget_->setAttribute(Qt::WA_OpaquePaintEvent);            //避免播放视频闪烁
    urgent_videowidget_->setStyleSheet("background-color: rgb(0, 0, 0);"); //背景色
    urgent_playlist_ = new QMediaPlaylist;
    urgent_timer_ = new QTimer;
    //状态显示
    stateWindow_ = new QWidget(this); //状态界面
    Hlay_rolltext_height_ = 20;
    power_title_label_ = new QLabel("power:");                                        //电量标题
    power_title_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色 字体颜色
    power_title_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    power_title_label_->setFixedHeight(Hlay_rolltext_height_);
    power_show_label_ = new QLabel("init");                                          //电量显示
    power_show_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    power_show_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    power_show_label_->setFixedHeight(Hlay_rolltext_height_);
    health_title_label_ = new QLabel("state:");                                        //车辆状态标题
    health_title_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    health_title_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    health_title_label_->setFixedHeight(Hlay_rolltext_height_);
    health_show_label_ = new QLabel("init");                                          //车辆状态显示
    health_show_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    health_show_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    health_show_label_->setFixedHeight(Hlay_rolltext_height_);
    signal_title_label_ = new QLabel("net:");                                          //网络信号标题
    signal_title_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    signal_title_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    signal_title_label_->setFixedHeight(Hlay_rolltext_height_);
    signal_show_label_ = new QLabel("init");                                          //网络信号显示
    signal_show_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    signal_show_label_->setAlignment(Qt::AlignCenter);                                //显示位置居中
    signal_show_label_->setFixedHeight(Hlay_rolltext_height_);
    //滚动字幕显示
    rolltextWindow_ = new QWidget(this);
    roll_text_label_ = new QLabel();               //滚动字幕显示
    roll_text_label_->setAlignment(Qt::AlignLeft); //显示位置居中
    roll_text_label_->setFixedHeight(Hlay_rolltext_height_);
    roll_text_content_ = "CANDELA";
    roll_text_label_->setStyleSheet("background-color: rgb(0, 0, 0);color:white;"); //背景色
    roll_text_timer_ = new QTimer;
    //获取屏幕
    getMonitor(monitor);
    brightness_test_timer_ = new QTimer;
}
void MainWindow::iniUI()
{
    //状态界面布局
    // Hlay_carstate_ = new QHBoxLayout(stateWindow_);
    // Hlay_carstate_->setContentsMargins(0,0,0,0);//边界 0
    // Hlay_carstate_->setSpacing(0);//布局内控件之间的距离 0
    // Hlay_carstate_->addWidget(power_title_label_);
    // Hlay_carstate_->addWidget(power_show_label_);
    // Hlay_carstate_->addWidget(health_title_label_);
    // Hlay_carstate_->addWidget(health_show_label_);
    // Hlay_carstate_->addWidget(signal_title_label_);
    // Hlay_carstate_->addWidget(signal_show_label_);
    //播放界面布局
    Slay_play_ = new QStackedLayout(playWindow_);
    Slay_play_->setStackingMode(QStackedLayout::StackAll);
    normal_label_index_ = Slay_play_->addWidget(normal_label_);
    normal_videowidget_index_ = Slay_play_->addWidget(normal_videowidget_);
    urgent_label_index_ = Slay_play_->addWidget(urgent_label_);
    urgent_videowidget_index_ = Slay_play_->addWidget(urgent_videowidget_);
    printf("normal_label_index_: %d\n", normal_label_index_);
    printf("normal_videowidget_index_: %d\n", normal_videowidget_index_);
    printf("urgent_label_index_: %d\n", urgent_label_index_);
    printf("urgent_videowidget_index_: %d\n", urgent_videowidget_index_);

    //字幕界面
    Hlay_rolltext_ = new QHBoxLayout(rolltextWindow_);
    Hlay_rolltext_->setContentsMargins(0, 0, 0, 0); //边界 0
    Hlay_rolltext_->setSpacing(0);                  //布局内控件之间的距离 0
    Hlay_rolltext_->addWidget(roll_text_label_);
    Hlay_rolltext_->addWidget(power_title_label_);
    Hlay_rolltext_->addWidget(power_show_label_);
    // Hlay_rolltext_->addWidget(health_title_label_);
    // Hlay_rolltext_->addWidget(health_show_label_);
    Hlay_rolltext_->addWidget(signal_title_label_);
    Hlay_rolltext_->addWidget(signal_show_label_);
    Hlay_rolltext_->setStretchFactor(roll_text_label_, 17);
    Hlay_rolltext_->setStretchFactor(power_title_label_, 1);
    Hlay_rolltext_->setStretchFactor(power_show_label_, 0.8);
    Hlay_rolltext_->setStretchFactor(signal_title_label_, 1);
    Hlay_rolltext_->setStretchFactor(signal_show_label_, 0.8);
    //主布局
    Vlay_main_ = new QVBoxLayout(centerWindow_);
    Vlay_main_->setContentsMargins(0, 0, 0, 0); //边界 0
    Vlay_main_->setSpacing(0);                  //布局内控件之间的距离 0
    // Vlay_main_->addWidget(stateWindow_);
    Vlay_main_->addWidget(playWindow_);
    Vlay_main_->addWidget(rolltextWindow_);
}
void MainWindow::iniSignalSlots()
{
    connect(normal_timer_, SIGNAL(timeout()), this, SLOT(onNormalTimeout()));
    connect(urgent_timer_, SIGNAL(timeout()), this, SLOT(onUrgentTimeout()));
    connect(roll_text_timer_, SIGNAL(timeout()), this, SLOT(onRolltextTimeout()));
    // connect(brightness_test_timer_,SIGNAL(timeout()),this,SLOT(onBrightnessTimeout()));
}

int MainWindow::parseCMD(QString cmd, QString &filePath, QString key)
{
    printf("parseCMD(): cmd: %s\n", cmd.toStdString().data());
    qDebug() << "parseCMD:" << cmd;
    QStringList strList = cmd.split(" ");
    printf("parseCMD(): parse num: [%d]\n", (int)strList.size());
    qDebug() << "parse num:" << strList.size();
    for (int i = 0; i < strList.size(); ++i)
    {
        printf("[%d]: %s\n", i, strList.at(i).toStdString().data());
        qDebug() << "[" << i << "]" << strList.at(i);
    }
    //如果只解析到1个返回-1,解析大于2个返回-2,
    if (1 == strList.size())
    {
        filePath = strList.at(0);
        qDebug() << "get filePath:" << filePath;
        return -1;
    }
    else if (2 == strList.size())
    {
        int time_secs = strList.at(0).section(key, 1, 1).toInt();
        printf("parseCMD(): get time_secs: %d\n", time_secs);
        qDebug() << "get time_secs:" << time_secs;
        filePath = strList.at(1);
        return time_secs;
    }
    else
    {
        filePath = strList.at(0);
        return -2;
    }
}

bool MainWindow::loadDefaultCmd(const QList<QString> &normal_cmd_list, const QList<QString> &face_cmd_list, const QList<QString> &urgent_cmd_list)
{
    normal_cmd_list_ = normal_cmd_list;
    qDebug() << "normal_cmd_list_:";
    for (int i = 0; i < normal_cmd_list_.size(); i++)
    {
        qDebug() << i << normal_cmd_list_.at(i);
    }
    face_cmd_list_ = face_cmd_list;
    qDebug() << "face_cmd_list_:";
    for (int i = 0; i < face_cmd_list_.size(); i++)
    {
        qDebug() << i << face_cmd_list_.at(i);
    }
    urgent_cmd_list_ = urgent_cmd_list;
    qDebug() << "urgent_cmd_list_:";
    for (int i = 0; i < urgent_cmd_list_.size(); i++)
    {
        qDebug() << i << urgent_cmd_list_.at(i);
    }
    return true;
}

void MainWindow::getMonitor(char *monitor)
{
    FILE *fp = popen("xrandr | grep ' connected' | sed 's/ connected.*$//g'", "r");
    if (fp == NULL)
    {
        printf("Failed to run command to get monitor\n");
        exit(1);
    }
    fgets(monitor, MONITOR_SIZE - 1, fp);
    pclose(fp);
    monitor[strlen(monitor) - 1] = '\0';
}

void MainWindow::setBrightness(int val)
{
    if (val < 0)
    {
        val = 0;
    }
    if (val > 100)
    {
        val = 100;
    }
    char cmd[100];
    sprintf(cmd, "xrandr --output %s --brightness %.2f\n", monitor, val / 100.);
    int len = strlen(cmd);
    cmd[len - 4] = '.';
    system(cmd);
}

bool MainWindow::run()
{
    normal_timer_->start(100);
    urgent_timer_->start(100);
    roll_text_timer_->start(400);
    brightness_test_timer_->start(5000);
    this->showFullScreen();
    return true;
}

void MainWindow::onBrightnessTimeout()
{
    static int brightness = 50;
    brightness %= 100;
    setBrightness(brightness);
    power_show_label_->setText(QString::number(brightness));
    brightness += 10;
}

void MainWindow::onRolltextTimeout()
{
    static int nPos = 0;
    if (nPos > roll_text_content_.length())
    {
        nPos = 0;
    }
    roll_text_label_->setText(roll_text_content_.mid(nPos));
    nPos++;
}
void MainWindow::onNormalTimeout()
{

    cmd_end = std::chrono::steady_clock::now();
    std::chrono::duration<double> diff;
    diff = (cmd_end - cmd_start);
    // qDebug()<<"diff.count:::"<<diff.count();
    if ((diff.count() > 5) && (urgent_cmd_.cmd_id != 0))
    {
        urgent_cmd_.cmd_id = 0;
        cmd_start = cmd_end;
    }

    //如果轮播在播放视频,或着无停止播放,退出
    if (normal_mediaplayer_->state() == QMediaPlayer::PlayingState || normal_play_forever_)
    {
        return;
    }
    //判断文件类型
    normal_file_type_ = getFile_type(normal_cmd_list_.at(currentMovieIndex_));
    if (normal_file_type_ == FILE_TYPE_IMAGE)
    {
        //解析参数 -w 播放时间
        QString imageName;
        int image_play_secs_ = parseCMD(normal_cmd_list_.at(currentMovieIndex_), imageName, "w");
        printf("onNormalTimeout(): imageName: %s;  image_play_secs_: %d\n", imageName.toStdString().data(), image_play_secs_);
        qDebug() << "normal_img:" << imageName << "secs_:" << image_play_secs_;
        normal_movie_->stop();
        normal_movie_->setFileName(imageName);
        normal_label_->clear();
        normal_label_->setMovie(normal_movie_);
        normal_movie_->start();
        if (urgent_cmd_.cmd_id != 1000)
        {
            //如果有紧急播放,播放但不置顶
            normal_label_->show();
        }
        else
        {
            //如果没有紧急命令在播放,置顶播放
            Slay_play_->setCurrentIndex(normal_label_index_);
            normal_label_->show();
        }
        //如果参数<0 ,无限播放
        if (image_play_secs_ < 0)
        {
            normal_play_forever_ = true;
        }
        else
        {
            normal_timer_->start(image_play_secs_ * 1000);
        }
    }
    else if (normal_file_type_ == FILE_TYPE_VIDEO)
    {
        //解析参数 -l 播放循环次数
        QString videoName;
        int video_play_loop_ = parseCMD(normal_cmd_list_.at(currentMovieIndex_), videoName, "l");
        printf("onNormalTimeout(): videoName: %s;  video_play_loop_: %d\n", videoName.toStdString().data(), video_play_loop_);
        qDebug() << "normal_vid:" << videoName << "loop_:" << video_play_loop_;
        normal_playlist_->clear();
        for (int i = 0; i < video_play_loop_; i++)
        {
            normal_playlist_->addMedia(QUrl::fromLocalFile(videoName));
        }
        if (video_play_loop_ < 0)
        {
            normal_playlist_->addMedia(QUrl::fromLocalFile(videoName));
            normal_playlist_->setPlaybackMode(QMediaPlaylist::Loop); //列表循环播放
            normal_play_forever_ = true;
        }
        else
        {
            normal_playlist_->setPlaybackMode(QMediaPlaylist::Sequential); //列表播放一次
        }
        normal_mediaplayer_->setPlaylist(normal_playlist_);
        normal_mediaplayer_->setVideoOutput(normal_videowidget_);
        normal_mediaplayer_->play();

        if (urgent_cmd_.cmd_id != 1000)
        {
            //如果有紧急播放,播放但不置顶
            normal_videowidget_->show();
        }
        else
        {
            //如果没有紧急命令在播放,置顶播放
            Slay_play_->setCurrentIndex(normal_videowidget_index_);
            normal_videowidget_->show();
        }
        normal_timer_->start(100);
    }
    //检索+1
    currentMovieIndex_++;
    if (currentMovieIndex_ >= normal_cmd_list_.count())
    {
        currentMovieIndex_ = 0;
    }
}
void MainWindow::onUrgentTimeout()
{
    //播放视频时,只有命令0和200才能打断
    if (urgent_mediaplayer_->state() == QMediaPlayer::PlayingState)
    {
        if (urgent_cmd_.cmd_id == 0 || urgent_cmd_.cmd_id == 200)
        {
            ;
        }
        else
        {
            return;
        }
    }
    //循环播放时,只有命令0和200才能打断
    if (urgent_play_forever_)
    {
        if (urgent_cmd_.cmd_id == 0 || urgent_cmd_.cmd_id == 200)
        {
            urgent_play_forever_ = false;
            urgent_cmd_.cmd_id = 999; // 999代表命令执行过了
            urgent_cmd_.cmd_str = ""; //清空
        }
        else
        {
            return;
        }
    }
    //命令 == 1000代表空闲
    if (urgent_cmd_.cmd_id == 1000)
    {
        return;
    }
    QString urgent_content = ""; //紧急播放的内容
    int received_cmd = 0;        //接收到的命令号
    bool undefined_cmd = false;  //是否是自定义播放

    //收到打断命令或者命令已经执行过
    if (urgent_cmd_.cmd_id == 0 || urgent_cmd_.cmd_id == 200 || urgent_cmd_.cmd_id == 999)
    {
        if (normal_file_type_ == FILE_TYPE_IMAGE)
        {
            Slay_play_->setCurrentIndex(normal_label_index_);
            normal_label_->show();
        }
        if (normal_file_type_ == FILE_TYPE_VIDEO)
        {
            Slay_play_->setCurrentIndex(normal_videowidget_index_);
            normal_videowidget_->show();
        }
        urgent_cmd_.cmd_id = 1000; // 1000代表紧急播放空闲
        return;
    }
    else if (urgent_cmd_.cmd_id >= 1 && urgent_cmd_.cmd_id <= 99)
    {
        //进入默认紧急命令播放
        if (urgent_cmd_.cmd_id > urgent_cmd_list_.size())
        {
            //如果命令id超出默认数目
            std::cout << "error! run(): urgent_cmd_.cmd_id " << urgent_cmd_.cmd_id << " is greater than urgent_cmd_list_.cmd_cnt!" << urgent_cmd_list_.size() << std::endl;
            qWarning() << "urgent_cmd_.cmd_id:" << urgent_cmd_.cmd_id << " is greater than urgent_cmd_list_.cmd_cnt:" << urgent_cmd_list_.size();
            return;
        }
        urgent_content = urgent_cmd_list_[urgent_cmd_.cmd_id - 1];
        urgent_cmd_.cmd_id = 999; // 999代表命令执行过了
        urgent_cmd_.cmd_str = ""; //清空
    }
    else if (urgent_cmd_.cmd_id >= 201 && urgent_cmd_.cmd_id <= 250)
    {
        //紧急表情命令播放
        if (urgent_cmd_.cmd_id - 200 > face_cmd_list_.size())
        {
            //如果命令id超出默认数目
            std::cout << "error! run(): urgent_cmd_.cmd_id " << urgent_cmd_.cmd_id << " is greater than face_list_.cmd_cnt!" << face_cmd_list_.size() << std::endl;
            qWarning() << "urgent_cmd_.cmd_id:" << urgent_cmd_.cmd_id << "is greater than face_list_.cmd_cnt:" << face_cmd_list_.size();
            return;
        }
        urgent_content = face_cmd_list_[urgent_cmd_.cmd_id - 201];
        urgent_cmd_.cmd_id = 999; // 999代表命令执行过了
        urgent_cmd_.cmd_str = ""; //清空
    }
    else if ((urgent_cmd_.cmd_id >= 251 && urgent_cmd_.cmd_id <= 255) ||
             (urgent_cmd_.cmd_id >= 100 && urgent_cmd_.cmd_id <= 199))
    {
        urgent_content = urgent_cmd_.cmd_str;
        received_cmd = urgent_cmd_.cmd_id;
        undefined_cmd = true;     //是自定义命令
        urgent_cmd_.cmd_id = 999; // 999代表命令执行过了
        urgent_cmd_.cmd_str = ""; //清空
    }

    //命令解析完开始执行播放动作
    if (!undefined_cmd)
    {                                                 //如果是非自定义显示指令
        int file_type = getFile_type(urgent_content); //获取文件类型
        if (file_type == FILE_TYPE_IMAGE)
        {
            //解析参数 -w 播放时间
            QString imageName;
            int image_play_secs_ = parseCMD(urgent_content, imageName, "w");
            printf("onUrgentTimeout(): imageName: %s;  image_play_secs_: %d\n", imageName.toStdString().data(), image_play_secs_);
            qDebug() << "urgent_img:" << imageName << "secs_:" << image_play_secs_;
            urgent_movie_->stop();
            urgent_movie_->setFileName(imageName);
            urgent_label_->clear();
            urgent_label_->setMovie(urgent_movie_);
            urgent_movie_->start();
            Slay_play_->setCurrentIndex(urgent_label_index_);
            urgent_label_->show();

            //如果参数<0 ,无限播放
            if (image_play_secs_ < 0)
            {
                urgent_play_forever_ = true;
            }
            else
            {
                urgent_timer_->start(image_play_secs_ * 1000);
            }
        }
        else if (file_type == FILE_TYPE_VIDEO)
        {
            //解析参数 -l 播放循环次数
            QString videoName;
            int video_play_loop_ = parseCMD(urgent_content, videoName, "l");
            printf("onUrgentTimeout(): videoName: %s;  video_play_loop_: %d\n", videoName.toStdString().data(), video_play_loop_);
            qDebug() << "urgent_vid:" << videoName << "loop_:" << video_play_loop_;
            urgent_playlist_->clear();
            for (int i = 0; i < video_play_loop_; i++)
            {
                urgent_playlist_->addMedia(QUrl::fromLocalFile(videoName));
            }
            if (video_play_loop_ < 0)
            {
                urgent_playlist_->addMedia(QUrl::fromLocalFile(videoName));
                urgent_playlist_->setPlaybackMode(QMediaPlaylist::Loop); //列表循环播放
                urgent_play_forever_ = true;
            }
            else
            {
                urgent_playlist_->setPlaybackMode(QMediaPlaylist::Sequential); //列表播放一次
            }
            urgent_mediaplayer_->setPlaylist(urgent_playlist_);
            urgent_mediaplayer_->setVideoOutput(urgent_videowidget_);
            Slay_play_->setCurrentIndex(urgent_videowidget_index_);
            urgent_videowidget_->show();
            urgent_mediaplayer_->play();
            urgent_timer_->start(100);
        }
    }
    else
    {
        //自定义字符串播放
        if (received_cmd == 100)
        { //电量显示
            power_show_label_->setText(urgent_content);
            qDebug() << "received power" << urgent_content;
        }
        if (received_cmd == 101)
        { //网络信号显示
            signal_show_label_->setText(urgent_content);
            qDebug() << "received net" << urgent_content;
        }
        if (received_cmd == 102)
        { //屏幕亮度控制
            int val = urgent_content.toInt();
            setBrightness(val);
            qDebug() << "received brightness" << urgent_content;
        }
        if (received_cmd == 255)
        { //滚动字幕显示
            roll_text_label_->setText(urgent_content);
            roll_text_content_ = urgent_content;
            qDebug() << "received rollText" << urgent_content;
        }
    }
}

void set_urgent_cmd_line(char *cmd, char *data)
{
    // printf("set_urgent_cmd_line: cmd: %s\n",cmd);
    if ((NULL == cmd) || (NULL == data))
    {
        return;
    }

    uint8_t cmd_idx = atoi(cmd);
    qDebug() << "receive urgent_cmd:" << cmd_idx << data;
    cmd_start = std::chrono::steady_clock::now();

    //使用预置命令进行播放
    if (cmd_idx <= 99 || (cmd_idx >= 200 && cmd_idx <= 250))
    {
        urgent_cmd_.cmd_id = cmd_idx;
    }
    //使用工控机自定义字符串播放
    else if ((cmd_idx >= 100 && cmd_idx <= 199) ||
             (cmd_idx >= 251 && cmd_idx <= 255))
    {
        urgent_cmd_.cmd_id = cmd_idx;
        QString qstr(data);
        urgent_cmd_.cmd_str = qstr;
    }
    else
    {
        std::cout << "error! set_urgent_cmd_line(): The urgent cmd id is out of range!!!" << cmd_idx << std::endl;
        qWarning() << "The urgent cmd id is out of range:" << cmd_idx;
        return;
    }
}