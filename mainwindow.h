#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QVector>
#include <QDir>
#include <QTime>
#include <QDebug>
#include <QFileDialog>
#include <QShortcut>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_musicList_itemDoubleClicked(QListWidgetItem *item);

    void on_musicList_itemSelectionChanged();

    void on_musicList_itemActivated(QListWidgetItem *item);

    void on_volumeSlider_valueChanged(int value);

    void positionChanged(qint64 value);

    void durationChanged(qint64 value);

    void on_songPosition_sliderMoved(int position);

    void on_songPlay_clicked();

    void on_songPause_clicked();

    void on_songStop_clicked();

    void on_songAdd_clicked();

    void deleteItem();

private:
    Ui::MainWindow *ui;
    QMediaPlayer*    player;
    QMediaPlaylist*  playlist;
};
#endif // MAINWINDOW_H
