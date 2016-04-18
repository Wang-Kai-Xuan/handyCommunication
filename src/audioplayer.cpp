#include "audioplayer.h"

void AudioPlayer::newCompoent()
{
    glay = new QGridLayout(this);
    hlay = new QHBoxLayout(this);
    play_pause = new QPushButton("播放|暂停");
    next_audio= new QPushButton("下一首");
    pre_audio = new QPushButton("上一首");
    open_audio = new QPushButton("打开");
    player = new QMediaPlayer(this);
    play_list = new QMediaPlaylist();
    dial = new QDial;
    slider = new QSlider;
    timer= new QTimer(this);
    lab_signer= new QLabel("歌手");
    lab_artist= new QLabel("歌手");
    lab_pic= new QLabel("封面");
    lab_title= new QLabel("歌名");
}

void AudioPlayer::setComponent()
{
    hlay->addWidget(open_audio);
    hlay->addWidget(play_pause);
    hlay->addWidget(next_audio);
    hlay->addWidget(pre_audio);

    glay->addWidget(lab_pic,LAYOUT_BASE+0,1,1,1);
    glay->addWidget(slider,LAYOUT_BASE+1,0,1,1);
    glay->addWidget(dial,LAYOUT_BASE+2,0,1,1);
    glay->addWidget(lab_signer,LAYOUT_BASE+3,0,1,1);
    glay->addWidget(lab_artist,LAYOUT_BASE+4,0,1,1);
    glay->addLayout(hlay,LAYOUT_BASE+5,1);
    this->setLayout(glay);

    timer->setInterval(100);
    timer->start();
    this->resize(800,600);
}
void AudioPlayer::onPlay()
{
    if(player->state() == QMediaPlayer::PlayingState)
        this->player->pause();
    else{
        player->setVolume(50);
        player->play();
    }
}
void AudioPlayer::onUpdateSlider()
{
    int pos = player->position();
    long dura = player->duration();
    int value = 100*(1.0*pos)/dura;
    slider->setValue(value);
}

void AudioPlayer::onNextPlay()
{
    player->playlist()->next();
}

void AudioPlayer::onPrePlay()
{
    player->playlist()->previous();
}

void AudioPlayer::setConnect()
{
    connect(open_audio,SIGNAL(clicked(bool)),this,SLOT(onOpenAudio()));
    connect(play_pause,SIGNAL(clicked(bool)),this,SLOT(onPlay()));
    connect(next_audio,SIGNAL(clicked(bool)),this,SLOT(onNextPlay()));
    connect(pre_audio,SIGNAL(clicked(bool)),this,SLOT(onPrePlay()));
    connect(dial,SIGNAL(sliderMoved(int)),this,SLOT(onDial(int)));
    connect(slider,SIGNAL(sliderMoved(int)),this,SLOT(onSlider(int)));
    connect(timer,SIGNAL(timeout()),this,SLOT(onUpdateSlider()));
    connect(player,SIGNAL(metaDataChanged()),this,SLOT(onMetaDataUpdate()));
}

AudioPlayer::AudioPlayer(QWidget *parent)
{
    newCompoent();
    setComponent();
    setConnect();
}

void AudioPlayer::onOpenAudio(void){
    QStringList list;
    list = QFileDialog::getOpenFileNames();
    if(!list.isEmpty()){
        play_list->clear();
        foreach (QString file_name, list) {
            qDebug()<<file_name;
            QMediaContent media = QMediaContent(QUrl::fromLocalFile(file_name));
            play_list->addMedia(media);
        }
        play_list->setCurrentIndex(0);
        player->setPlaylist(play_list);
    }
}
void AudioPlayer::onDial(int position)
{
   player->setVolume(position);
}
void AudioPlayer::onMetaDataUpdate()
{
   QString title,album_title,artist;
   QImage cover;
   QPixmap pixmap;

   title = player->metaData("Title").toString();
   album_title = player->metaData("SubTitle").toString();
   artist= player->metaData("Author").toString();
   cover = player->metaData("CoverArtImage").value<QImage>();
   if(cover.isNull()){
       pixmap = QPixmap(":/res/backgroud0.jpg");
   }else{
       pixmap.convertFromImage(cover);
   }
   lab_artist->setText(artist);
   lab_signer->setText(title);
   lab_pic->setPixmap(pixmap.scaled(lab_pic->size()));

}
void AudioPlayer::onSlider(int position)
{
    float f = (position*1.0)/dial->maximum();
    long value = player->duration()*f;
    player->setPosition(value);
}
