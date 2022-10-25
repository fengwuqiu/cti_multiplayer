#ifndef MEDIAWIDGET_H
#define MEDIAWIDGET_H

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

QT_BEGIN_NAMESPACE
class QMovie;
class QTimer;
class QLabel;
class QMediaPlayer;
class QVideoWidget;
class QMediaPlaylist;
class QImage;
class QColor;
QT_END_NAMESPACE


#define DEFAULT_VIDEO_PLAY_LOOP 1
#define DEFAULT_IMAGE_PLAY_SECS 5
extern volatile bool interrupt_received;

class MediaWidget : public QWidget
{
    Q_OBJECT
    public:
        MediaWidget(QWidget* parent = NULL);
        ~MediaWidget(){};
        bool load(const QList<QString> &fileList);
        bool normalPlay();
        bool urgentPlay(bool is_custom);
        bool stop();
        bool close();
    private slots:
        void onMediaChangeTimeout();
        void onUrgentTimeout();
        int parseImageCMD(QString cmd, QString& filePath);
        int parseVideoCMD(QString cmd, QString& filePath);
    private:
        QTimer *normalTimer_; 
        QTimer *urgentTimer_;
        QList<QString> fileList_;
        QMovie currentMovie_;
        int currentMovieIndex_;
        QLabel label_;
        QMediaPlayer* mediaplayer_;
        QVideoWidget* videowidget_;
        QMediaPlaylist* playlist_;
        private:
        int video_play_loop_; //视频播放循环次数
        int image_play_secs_; //图片播放持续时间
        bool urgent_image_has_played; //紧急图片已经播放过
        bool urgent_video_has_played; //紧急视频已经播放过
};
#endif