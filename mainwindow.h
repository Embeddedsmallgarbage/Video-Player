#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QListWidget>
#include <QComboBox>
#include <QFileDialog>
#include <QDir>
#include <QTimer>
#include <QStyle>
#include <QApplication>
#include <QAudioOutput>
#include <QSettings>
#include <QMouseEvent>
#include <QAudio>
#include <QTimer>
#include <QKeyEvent>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QAbstractItemView>
#include <QMediaDevices>
#include "settingsdialog.h"
#include "positiondialog.h"

// 自定义进度条类，支持点击定位
class ClickableSlider : public QSlider
{
    Q_OBJECT
public:
    explicit ClickableSlider(Qt::Orientation orientation, QWidget *parent = nullptr)
        : QSlider(orientation, parent) {}

protected:
    void mousePressEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            if (orientation() == Qt::Horizontal) {
                int value = minimum() + ((maximum() - minimum()) * event->x()) / width();
                setValue(value);
                emit sliderMoved(value);
            }
        }
        QSlider::mousePressEvent(event);
    }
};

// 自定义视频播放组件，支持双击暂停/播放
class ClickableVideoWidget : public QVideoWidget
{
    Q_OBJECT
public:
    explicit ClickableVideoWidget(QWidget *parent = nullptr)
        : QVideoWidget(parent) {}

signals:
    void doubleClicked();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override
    {
        if (event->button() == Qt::LeftButton) {
            emit doubleClicked();
        }
        QVideoWidget::mouseDoubleClickEvent(event);
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void playVideo();
    void pauseVideo();
    void stopVideo();
    void setPosition(int position);
    void updatePosition(qint64 position);
    void updateDuration(qint64 duration);
    void onPlaylistItemDoubleClicked(QListWidgetItem *item);
    void changePlaybackRate();
    void togglePlaylist();
    void mediaStatusChanged(QMediaPlayer::MediaStatus status);
    void playbackStateChanged(QMediaPlayer::PlaybackState state);
    void setVolume(int volume);
    void playNextVideo();
    void toggleMute();
    void updateVolumeIcon();
    void openSettings();
    void onLongPressTimer();
    void seekVideo(int seconds);
    void startFastSeek(bool forward);
    void onContinuousSeekTimer();
    void saveVideoPosition();
    void checkAndShowPositionDialog(const QString &filePath);
    void onPositionDialogFinished();
    QString getVideoHash(const QString &filePath);
    void onVideoWidgetDoubleClicked();
    void moveItemUp();
    void moveItemDown();
    void playNextVideoAuto();
    void onAudioOutputsChanged();
    void openFileOrFolder();
    void addVideoFile(const QString &filePath);
    void removeSelectedVideo();

private:
    void setupUI();
    void setupConnections();
    void loadVideosFromFolder(const QString &folderPath);
    void playVideoFile(const QString &filePath);
    void updatePlayButton();
    void formatTime(qint64 timeInMs, QString &str);
    void saveSettings();
    void loadSettings();
    void setupProgressBarClickable();
    
protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    
    // UI组件
    QWidget *m_centralWidget;
    QSplitter *m_mainSplitter;
    QWidget *m_videoContainer;
    QWidget *m_playlistContainer;
    ClickableVideoWidget *m_videoWidget;
    QListWidget *m_playlistWidget;
    
    // 控制组件
    QWidget *m_controlsWidget;
    QPushButton *m_playButton;
    QPushButton *m_stopButton;
    QPushButton *m_openFolderButton;
    QPushButton *m_togglePlaylistButton;
    ClickableSlider *m_positionSlider;
    QSlider *m_volumeSlider;
    QPushButton *m_nextButton;
    QPushButton *m_settingsButton;
    QPushButton *m_moveUpButton;
    QPushButton *m_moveDownButton;
    QPushButton *m_removeButton;
    QComboBox *m_speedComboBox;
    QLabel *m_timeLabel;
    QPushButton *m_volumeButton;
    
    // 媒体播放器
    QMediaPlayer *m_mediaPlayer;
    QAudioOutput *m_audioOutput;
    
    // 状态变量
    bool m_playlistVisible;
    qint64 m_duration;
    QString m_currentFolder;
    QSettings *m_settings;
    bool m_positionSliderPressed;
    int m_currentPlayingIndex;
    bool m_isMuted;
    int m_previousVolume;
    
    // 键盘控制相关
    QTimer *m_longPressTimer;
    QTimer *m_continuousSeekTimer;
    bool m_isLongPressing;
    bool m_longPressForward;
    double m_leftKeySpeed;
    double m_rightKeySpeed;
    SettingsDialog *m_settingsDialog;
    PositionDialog *m_positionDialog;
    QString m_currentVideoHash;
    qint64 m_pendingJumpPosition;
};

#endif // MAINWINDOW_H
