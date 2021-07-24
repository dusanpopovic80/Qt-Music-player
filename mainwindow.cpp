#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef Q_OS_LINUX
    const QString    musicPath = "/home/hel/Music/";
    const QString    os        = "Linux";
#elif defined(Q_OS_WIN32)
    const QString    musicPath = "C:\\Users\\Admin\\Music";
    const QString    os        = "Windows";
#endif

quint8           volume = 100;
QVector<QString> songList;

QDir        directory(musicPath);
QStringList files = directory.entryList(QStringList()
                                         << "*.wma"
                                         << "*.aac"
                                         << "*.ogg"
                                         << "*.wav"
                                         << "*.mp3"
                                         << "*.flac",
                                         QDir::Files);

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player     = new QMediaPlayer;
    playlist  = new QMediaPlaylist(player);

    qDebug() << os;

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::durationChanged);

    new QShortcut(QKeySequence(Qt::Key_Delete), this, SLOT(deleteItem()));

    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::Key_Delete), ui->musicList);
    connect(shortcut, SIGNAL(activated()), this, SLOT(deleteItem()));

    ui->setupUi(this);
    ui->volumeInfo->setText("Volume: " + QString::number(volume));

    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);

    // JSON Playlist settings
    /*
    QJsonDocument playlistDocument;
    QJsonObject playlistJson;

    QJsonArray array;
    QJsonObject someObj;

    array.append( QJsonValue(32.4) );
    array.append( someObj );

    playlistJson["intValue"] = 1;
    playlistJson["SomeEmptyObject"] = someObj;
    playlistJson["somearray"] = array;

    playlistDocument.setObject(playlistJson);
    QString playlistJsonStr =  playlistDocument.toJson();

    QFile playlistSettings(QDir::currentPath() + "/playlist.json");
    playlistSettings.open(QIODevice::ReadWrite);

    playlistSettings.write(playlistJsonStr.toUtf8());
*/

/*
{
    "SomeEmptyObject": {
    },
    "intValue": 1,
    "somearray": [
        32.4,
        {
        }
    ]
}
*/

    QJsonDocument playlistDocument;
    QJsonObject playlistJson;

    QJsonArray songArr;

    songArr.append(QJsonObject{ {"path", musicPath + "song1"} });
    songArr.append(QJsonObject{ {"path", musicPath + "song2"} });
    songArr.append(QJsonObject{ {"path", musicPath + "song2"} });
    songArr.append(QJsonObject{ {"url", "https://www.youtube.com/watch?v=d_AP3SGMxxM"} });
    songArr.append(QJsonObject{ {"url", "https://www.youtube.com/watch?v=IHoj7qvNmwk"} });

    songArr.append(QJsonObject{
                       {"param1", 1},
                       {"param2", "string 2"}
                   });

    playlistJson["songList"] = songArr;

    playlistDocument.setObject(playlistJson);
    QString playlistJsonStr =  playlistDocument.toJson();

    QFile playlistSettings(QDir::currentPath() + "/playlist.json");
    playlistSettings.open(QIODevice::ReadWrite);

    playlistSettings.write(playlistJsonStr.toUtf8());

    for (QString fileName : files)
    {
        songList.push_back(fileName);
        ui->musicList->addItem("[  LocalStorage  ]"
                               + QString("          ")
                               + fileName);
        /*[  LocalStorage       ] SongName
          [  Youtube            ] OnlineSong
          [  Downloading - 57%  ] DownloadingSong*/

        playlist->addMedia(QUrl::fromLocalFile(musicPath + fileName));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_musicList_itemDoubleClicked(QListWidgetItem *item)
{
    quint32 index = ui->musicList->currentIndex().row();
    playlist->setCurrentIndex(index);
    player->setVolume(volume);
    player->play();
}

void MainWindow::on_musicList_itemSelectionChanged()
{
    quint32 index = ui->musicList->currentIndex().row();
    ui->songInfo->setText( "Index: "
                           + QString::number(index)
                           + " "
                           + "Length: "
                           + "00:00" );
}

void MainWindow::deleteItem()
{
    QList<QListWidgetItem*> items = ui->musicList->selectedItems();
    for(QListWidgetItem* item : items)
    {
        playlist->removeMedia(ui->musicList->row(item));
        delete ui->musicList->takeItem(ui->musicList->row(item));
    }
}

void MainWindow::on_musicList_itemActivated(QListWidgetItem *item)
{
    quint32 index = ui->musicList->currentIndex().row();
    playlist->setCurrentIndex(index);
    player->setVolume(volume);
    player->play();
}

void MainWindow::on_volumeSlider_valueChanged(int value)
{
    volume = value;
    ui->volumeInfo->setText("Volume: " + QString::number(volume));
    player->setVolume(volume);
}

void MainWindow::positionChanged(qint64 value)
{
    static QTime songProgression  ( 0, 0, 0, 0 );
    ui->songPosition->setValue    ( value );
    ui->songProgressInfo->setText ( songProgression
                                    .addMSecs(value)
                                    .toString("hh:mm:ss") );
}

void MainWindow::durationChanged(qint64 value)
{
    ui->songPosition->setMaximum(value);
}

void MainWindow::on_songPosition_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::on_songPlay_clicked()
{
    player->play();
}

void MainWindow::on_songPause_clicked()
{
    player->pause();
}

void MainWindow::on_songStop_clicked()
{
    player->stop();
}

void MainWindow::on_songAdd_clicked()
{
    QStringList filePaths = QFileDialog::getOpenFileNames(0,
                                                          tr("Open File"),
                                                          musicPath);
    for (QString filePath : filePaths)
    {

        QString getFile(filePath);
        QStringList slpitPath = getFile.split("/");
        QString fileName = slpitPath.at(slpitPath.size() - 1);

        ui->musicList->addItem("[  LocalStorage  ]"
                               + QString("          ")
                               + fileName);

        playlist->addMedia(QUrl::fromLocalFile(filePath));
    }
}
