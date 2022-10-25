#include "mediawidget.h"
#include <unistd.h>

MediaWidget::MediaWidget(QWidget* parent) 
    : QWidget(parent)
    , currentMovieIndex_(0)
{
    normalTimer_ = new QTimer(this);
    connect(normalTimer_,SIGNAL(timeout()),this,SLOT(onMediaChangeTimeout()));
    urgentTimer_ = new QTimer(this);
    connect(urgentTimer_,SIGNAL(timeout()),this,SLOT(onUrgentTimeout()));
    mediaplayer_ = new QMediaPlayer(this,QMediaPlayer::VideoSurface);
    videowidget_ = new QVideoWidget;
    playlist_ = new QMediaPlaylist;
    video_play_loop_ = 1;
    image_play_secs_ = 5;
    urgent_image_has_played = false;
    urgent_video_has_played = false;
    return;
}

bool MediaWidget::load(const QList<QString> &fileList){
    fileList_=fileList;
    return true;
}

bool MediaWidget::normalPlay(){
    label_.setStyleSheet(QString("background-color:") + "black");
    videowidget_->setStyleSheet(QString("background-color:") + "black");
    normalTimer_->start(500);
    return true;
}

bool MediaWidget::urgentPlay(bool is_custom){
    label_.setStyleSheet(QString("background-color:") + "black");
    videowidget_->setStyleSheet(QString("background-color:") + "black");
    currentMovie_.setBackgroundColor(QColor(0,0,0));
    //开始播放
    if(is_custom){  //自定义播放
        label_.setText(fileList_.at(currentMovieIndex_));
        label_.showFullScreen();
        label_.show();
    }
    else{  //预置命令播放
        urgentTimer_->start(100);
        urgent_image_has_played = false;
        urgent_video_has_played = false;
    }
    return true;
}

void MediaWidget::onMediaChangeTimeout(){
    if(interrupt_received){
        return;
    }
    //判断是否在播放视频
    if(mediaplayer_->state() == QMediaPlayer::PlayingState){
        return;
    label_.setVisible(true);
    }else{
        normalTimer_->stop();
    }
    //清空状态
    currentMovie_.stop();
    label_.clear();
    playlist_->clear();
    mediaplayer_->pause();
    mediaplayer_->stop();
    //判断文件类型
    int file_type = getFile_type(fileList_.at(currentMovieIndex_));
    //分类播放
    if (file_type == FILE_TYPE_IMAGE){
        //解析参数 -w 播放时间
        QString imageName;
        image_play_secs_ = parseImageCMD(fileList_.at(currentMovieIndex_),imageName);
        printf("onMediaChangeTimeout(): imageName: %s;  image_play_secs_: %d\n",imageName.toStdString().data(),image_play_secs_);
        currentMovie_.setFileName(imageName);
        label_.setMovie(&currentMovie_);
        label_.setScaledContents(1);
        label_.showFullScreen();
        currentMovie_.start();
        //如果参数<0 ,无限播放
        if(image_play_secs_ < 0){
            normalTimer_->stop();
        }else{
            normalTimer_->start(image_play_secs_ * 1000);
        }
    }else if (file_type == FILE_TYPE_VIDEO){
        //解析参数 -l 播放循环次数
        QString videoName;
        video_play_loop_ = parseVideoCMD(fileList_.at(currentMovieIndex_),videoName);
        printf("onMediaChangeTimeout(): videoName: %s;  video_play_loop_: %d\n",videoName.toStdString().data(),video_play_loop_);
        for(int i = 0; i < video_play_loop_; i++){
            playlist_->addMedia(QUrl::fromLocalFile(videoName));
        }
        if(video_play_loop_ < 0){
            playlist_->addMedia(QUrl::fromLocalFile(videoName));
            playlist_->setPlaybackMode(QMediaPlaylist::Loop);
            normalTimer_->stop();
        }else{
            playlist_->setPlaybackMode(QMediaPlaylist::Sequential);
            normalTimer_->stop();
            normalTimer_->start(200);
        }
        mediaplayer_->setPlaylist(playlist_);
        mediaplayer_->setVideoOutput(videowidget_);
        videowidget_->setFullScreen(1);
        videowidget_->show();
        mediaplayer_->play();
    }

    currentMovieIndex_++;
    if(currentMovieIndex_ >= fileList_.count()){
        currentMovieIndex_ = 0;
    }
 }


void MediaWidget::onUrgentTimeout(){
    interrupt_received = true;
    currentMovie_.stop();
    //判断是否在播放视频
    if(mediaplayer_->state() == QMediaPlayer::PlayingState){
        return;
    }else{
        urgentTimer_->stop();
    }
    //判断文件类型
    int file_type = getFile_type(fileList_.at(0));
    //分类播放
    if (file_type == FILE_TYPE_IMAGE){
        //解析参数 -w 播放时间
        QString imageName;
        image_play_secs_ = parseImageCMD(fileList_.at(0),imageName);
        printf("onUrgentTimeout(): imageName: %s;  image_play_secs_: %d\n",imageName.toStdString().data(),image_play_secs_);
        currentMovie_.setFileName(imageName);
        label_.setMovie(&currentMovie_);
        label_.setScaledContents(1);
        currentMovie_.start();
        label_.showFullScreen();
        //usleep(100000);
        label_.setVisible(true);
        //如果参数<0 ,无限播放
        if(image_play_secs_ < 0){
            urgentTimer_->stop();
        }else {
            //如果还没有播放过则播放,如果播放过,则关掉窗口
            if(!urgent_image_has_played){
                urgentTimer_->start(image_play_secs_ * 1000);
                urgent_image_has_played = true;
            }else{
                this->close();
                interrupt_received = false;
                return;
            }      
        }
    }else if (file_type == FILE_TYPE_VIDEO){
        //解析参数 -l 播放循环次数
        QString videoName;
        video_play_loop_ = parseVideoCMD(fileList_.at(0),videoName);
        printf("onUrgentTimeout(): videoName: %s;  video_play_loop_: %d\n",videoName.toStdString().data(),video_play_loop_);
        for(int i = 0; i < video_play_loop_; i++){
            playlist_->addMedia(QUrl::fromLocalFile(videoName));
        }
        //如果参数<0,无限播放
        if(video_play_loop_ < 0){
            playlist_->addMedia(QUrl::fromLocalFile(videoName));
            playlist_->setPlaybackMode(QMediaPlaylist::Loop);
            urgentTimer_->stop();
        }else{
            //如果还没有播放过则播放,如果播放过,则关掉窗口
            if(!urgent_video_has_played){
                playlist_->setPlaybackMode(QMediaPlaylist::Sequential);
                urgentTimer_->stop();
                urgentTimer_->start(200);
                urgent_video_has_played = true;
            }else{
                this->close();
		interrupt_received = false;
                return;
            }
        }
        mediaplayer_->setPlaylist(playlist_);
        mediaplayer_->setVideoOutput(videowidget_);
        videowidget_->setFullScreen(1);
        videowidget_->show();
        mediaplayer_->play();
    }
}

bool MediaWidget::stop(){
    urgentTimer_->stop();
    normalTimer_->stop();
    currentMovie_.stop();
    label_.clear();
    playlist_->clear();
    mediaplayer_->pause();
    mediaplayer_->stop();
    return true;
}
bool MediaWidget::close(){ 
    label_.clear();
    //******************
    //此段代码是为了解决这一次的紧急播放命令的图在下一次紧急命令播放时会短暂的出现（貌似是隐藏的
    //时候有缓存），暂时没有别的办法，每次播放完之后都用黑色背景覆盖掉，临时办法。
    QMovie tempMovie;
    tempMovie.setFileName("/home/pi/cti_multiplayer/media/black.jpg");
    label_.setMovie(&tempMovie);
    tempMovie.setSpeed(1000);
    tempMovie.start();
    label_.setScaledContents(1);
    label_.showFullScreen(); 
    //******************
    label_.setVisible(false);

    playlist_->clear();
    mediaplayer_->pause();
    mediaplayer_->stop();
    videowidget_->hide();
    interrupt_received = false;

    urgentTimer_->stop();
    normalTimer_->stop();
    return true;
}

int MediaWidget::parseImageCMD(QString cmd, QString& filePath){
    printf("parseImageCMD(): cmd: %s\n",cmd.toStdString().data());
    QStringList strList = cmd.split(" ");
    printf("parseImageCMD(): parse num: [%d]\n",(int)strList.size());
    for(int i = 0; i < strList.size(); ++i){
        printf("[%d]: %s\n",i,strList.at(i).toStdString().data());
    }
    //如果只解析到1个返回-1,解析大于2个返回-2,
    if(1 == strList.size()){ 
        filePath = strList.at(0);
        return -1;
    }else if(2 == strList.size()){ 
        int time_secs = strList.at(0).section("w",1,1).toInt();
        printf("parseImageCMD(): get time_secs: %d\n",time_secs);
        filePath = strList.at(1);
        return time_secs;
    }else{
        filePath = strList.at(0);
        return -2;
    }
}
int MediaWidget::parseVideoCMD(QString cmd, QString& filePath){
    printf("parseVideoCMD(): cmd: %s\n",cmd.toStdString().data());
    QStringList strList = cmd.split(" ");
    printf("parseVideoCMD(): parse num: [%d]\n",(int)strList.size());
    for(int i = 0; i < strList.size(); ++i){
        printf("[%d]: %s\n",i,strList.at(i).toStdString().data());
    }
    //如果只解析到1个返回-1,解析大于2个返回-2,
    if(1 == strList.size()){ 
        filePath = strList.at(0);
        return -1;
    }else if(2 == strList.size()){ 
        int loop_cnts = strList.at(0).section("l",1,1).toInt();
        printf("parseVideoCMD(): get loop_cnts: %d\n",loop_cnts);
        filePath = strList.at(1);
        return loop_cnts;
    }else{
        filePath = strList.at(0);
        return -2;
    }
}
