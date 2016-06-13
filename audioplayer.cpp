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
    slider = new QSlider(Qt::Horizontal);
    timer= new QTimer(this);
    lab_signer= new QLabel("歌手");
    lab_artist= new QLabel("歌手");
    lab_pic= new QLabel("封面");
    lab_title= new QLabel("歌名");
}

void AudioPlayer::setComponent()
{
    lab_pic->setFixedHeight(200);
    lab_pic->setFixedWidth(200);
    hlay->addWidget(open_audio);
    hlay->addWidget(play_pause);
    hlay->addWidget(next_audio);
    hlay->addWidget(pre_audio);

    glay->addWidget(lab_title,0,0,1,1);
    glay->addWidget(lab_pic,1,0,1,1);
    glay->addWidget(lab_signer,2,0,1,1);
    glay->addWidget(lab_artist,3,0,1,1);
    glay->addWidget(slider,4,0,1,1);
    glay->addWidget(dial,4,1,1,1);
    glay->addLayout(hlay,5,1,1,2);

    timer->setInterval(100);
    timer->start();

    this->resize(800,600);
    this->setLayout(glay);
}

void AudioPlayer::onPlay()
{
    if(player->state() == QMediaPlayer::PlayingState) this->player->pause();
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

AudioPlayer::AudioPlayer()
{
    newCompoent();
    setComponent();
    setConnect();
//    lab_pic->setPixmap(QPixmap("F:/Project/handyCommunication/src/backgroud0.jpg"));
}

AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::onOpenAudio(void){
    QStringList list = QFileDialog::getOpenFileNames(
                this\
                ,"选择要添加的音乐文件"\
                ,QStandardPaths::writableLocation(QStandardPaths::MusicLocation)\
                ,"Audio(*.mp3)");
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
   title = player->metaData("Title").toString();
   album_title = player->metaData("SubTitle").toString();
   artist= player->metaData("Author").toString();
   cover = player->metaData("CoverArtImage").value<QImage>();
   if(cover.isNull()){
//       pixmap.load(":/res/backgroud0.jpg");
   }else{
   }

   lab_artist->setText(artist);
   lab_signer->setText(title);



}
void AudioPlayer::onSlider(int position)
{
    float f = (position*1.0)/dial->maximum();
    long value = player->duration()*f;
    player->setPosition(value);
}
