#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include "header.h"

class AudioPlayer : public QWidget
{
    Q_OBJECT
public:
    AudioPlayer();
    ~AudioPlayer();
    QMediaPlayer * player;
    QMediaPlaylist * play_list;
    QGridLayout *glay;
    QHBoxLayout * hlay;
    QPushButton *play_pause;
    QPushButton *next_audio;
    QPushButton *pre_audio;
    QPushButton *open_audio;
    QDial *dial;
    QSlider * slider;
    QTimer* timer;
    QLabel* lab_signer;
    QLabel* lab_artist;
    QLabel* lab_pic;
    QLabel* lab_title;
    void newCompoent();
    void setComponent();
    void setConnect();

signals:

public slots:
    void onOpenAudio();
    void onPlay();
    void onNextPlay();
    void onPrePlay();
    void onDial(int position);
    void onSlider(int position);
    void onUpdateSlider();
    void onMetaDataUpdate();
};

#endif // AUDIOPLAYER_H
