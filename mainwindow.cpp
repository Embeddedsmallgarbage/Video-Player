#include "mainwindow.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainSplitter(nullptr)
    , m_videoContainer(nullptr)
    , m_playlistContainer(nullptr)
    , m_videoWidget(nullptr)
    , m_playlistWidget(nullptr)
    , m_controlsWidget(nullptr)
    , m_playButton(nullptr)
    , m_stopButton(nullptr)
    , m_openFolderButton(nullptr)
    , m_togglePlaylistButton(nullptr)
    , m_positionSlider(nullptr)
    , m_speedComboBox(nullptr)
    , m_timeLabel(nullptr)
    , m_mediaPlayer(nullptr)
    , m_audioOutput(nullptr)
    , m_playlistVisible(true)
    , m_duration(0)
    , m_settings(nullptr)
    , m_positionSliderPressed(false)
    , m_currentPlayingIndex(-1)
    , m_isMuted(false)
    , m_previousVolume(70)
    , m_longPressTimer(nullptr)
    , m_continuousSeekTimer(nullptr)
    , m_isLongPressing(false)
    , m_longPressForward(true)
    , m_leftKeySpeed(2.0)
    , m_rightKeySpeed(2.0)
    , m_settingsDialog(nullptr)
    , m_positionDialog(nullptr)
    , m_currentVideoHash("")
    , m_pendingJumpPosition(-1)
{
    // 初始化设置
    m_settings = new QSettings("VideoPlayer", "Settings", this);
    
    setupUI();
    setupConnections();
    loadSettings();
    
    // 设置窗口属性
    setWindowTitle("视频播放器");
    setMinimumSize(800, 600);
    resize(1200, 800);
    
    // 设置焦点策略，确保能接收键盘事件
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
}

MainWindow::~MainWindow()
{
    if (m_mediaPlayer) {
        // 保存当前播放位置
        saveVideoPosition();
        m_mediaPlayer->stop();
    }
    saveSettings();
}

void MainWindow::setupUI()
{
    // 创建中央部件
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // 创建主分割器
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // 创建视频容器
    m_videoContainer = new QWidget();
    m_videoContainer->setStyleSheet("background-color: black;"); // 视频区域保持黑色
    
    // 创建视频播放组件
    m_videoWidget = new ClickableVideoWidget();
    m_videoWidget->setStyleSheet("background-color: black;");
    
    // 创建媒体播放器和音频输出
    m_mediaPlayer = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_mediaPlayer->setAudioOutput(m_audioOutput);
    m_mediaPlayer->setVideoOutput(m_videoWidget);
    
    // 创建控制面板
    m_controlsWidget = new QWidget();
    m_controlsWidget->setMaximumHeight(80);
    m_controlsWidget->setStyleSheet("background-color: white; border-top: 1px solid #ccc;");
    
    // 创建控制按钮
    m_playButton = new QPushButton();
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    m_playButton->setFixedSize(40, 40);
    m_playButton->setStyleSheet("QPushButton { border: none; background-color: #f0f0f0; border-radius: 20px; } QPushButton:hover { background-color: #e0e0e0; }");
    
    m_stopButton = new QPushButton();
    m_stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    m_stopButton->setFixedSize(40, 40);
    m_stopButton->setStyleSheet("QPushButton { border: none; background-color: #f0f0f0; border-radius: 20px; } QPushButton:hover { background-color: #e0e0e0; }");
    
    m_openFolderButton = new QPushButton("选择文件/文件夹");
    m_openFolderButton->setStyleSheet("QPushButton { padding: 8px 16px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; }");
    
    m_togglePlaylistButton = new QPushButton("隐藏播放列表");
    m_togglePlaylistButton->setStyleSheet("QPushButton { padding: 8px 16px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; }");
    
    m_nextButton = new QPushButton();
    m_nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));
    m_nextButton->setFixedSize(40, 40);
    m_nextButton->setStyleSheet("QPushButton { border: none; background-color: #f0f0f0; border-radius: 20px; } QPushButton:hover { background-color: #e0e0e0; }");
    m_nextButton->setToolTip("播放下一个视频");
    
    m_settingsButton = new QPushButton();
    m_settingsButton->setIcon(QIcon(":/Images/setting.png"));
    m_settingsButton->setFixedSize(40, 40);
    m_settingsButton->setStyleSheet("QPushButton { border: none; background-color: #f0f0f0; border-radius: 20px; } QPushButton:hover { background-color: #e0e0e0; }");
    m_settingsButton->setToolTip("设置");
    
    // 创建进度条（支持点击定位）
    m_positionSlider = new ClickableSlider(Qt::Horizontal);
    m_positionSlider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #ccc; height: 8px; background: #f5f5f5; margin: 2px 0; border-radius: 4px; } QSlider::handle:horizontal { background: #0078d4; border: 1px solid #0078d4; width: 18px; margin: -2px 0; border-radius: 9px; }");
    
    // 创建倍速选择
    m_speedComboBox = new QComboBox();
    m_speedComboBox->addItems({"0.5x", "0.75x", "1.0x", "1.25x", "1.5x", "1.75x", "2.0x", "2.25x", "2.5x", "2.75x", "3.0x"});
    m_speedComboBox->setCurrentText("1.0x");
    m_speedComboBox->setStyleSheet("QComboBox { padding: 4px 8px; background-color: white; border: 1px solid #ccc; border-radius: 4px; color: black; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border: none; } QComboBox QAbstractItemView { background-color: white; color: black; border: 1px solid #ccc; }");
    
    // 创建时间标签
    m_timeLabel = new QLabel("00:00 / 00:00");
    m_timeLabel->setStyleSheet("color: black; font-family: monospace;");
    m_timeLabel->setMinimumWidth(100);
    
    // 创建音量控制
    m_volumeButton = new QPushButton();
    m_volumeButton->setFixedSize(32, 32);
    m_volumeButton->setStyleSheet("QPushButton { border: none; background-color: transparent; } QPushButton:hover { background-color: #f0f0f0; border-radius: 16px; }");
    m_volumeButton->setToolTip("点击静音/取消静音");
    
    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(70);
    m_volumeSlider->setMaximumWidth(100);
    m_volumeSlider->setStyleSheet("QSlider::groove:horizontal { border: 1px solid #ccc; height: 6px; background: #f5f5f5; margin: 2px 0; border-radius: 3px; } QSlider::handle:horizontal { background: #0078d4; border: 1px solid #0078d4; width: 16px; margin: -2px 0; border-radius: 8px; }");
    
    // 布局控制面板
    QHBoxLayout *controlsLayout = new QHBoxLayout(m_controlsWidget);
    controlsLayout->addWidget(m_playButton);
    controlsLayout->addWidget(m_stopButton);
    controlsLayout->addWidget(m_nextButton);
    controlsLayout->addWidget(m_positionSlider, 1);
    controlsLayout->addWidget(m_timeLabel);
    controlsLayout->addWidget(m_volumeButton);
    controlsLayout->addWidget(m_volumeSlider);
    
    QLabel *speedLabel = new QLabel("倍速:");
    speedLabel->setStyleSheet("color: black;");
    controlsLayout->addWidget(speedLabel);
    controlsLayout->addWidget(m_speedComboBox);
    controlsLayout->addWidget(m_openFolderButton);
    controlsLayout->addWidget(m_settingsButton);
    controlsLayout->addWidget(m_togglePlaylistButton);
    controlsLayout->setContentsMargins(10, 10, 10, 10);
    
    // 倍速标签样式已在上面设置
    
    // 布局视频容器
    QVBoxLayout *videoLayout = new QVBoxLayout(m_videoContainer);
    videoLayout->addWidget(m_videoWidget, 1);
    videoLayout->addWidget(m_controlsWidget);
    videoLayout->setContentsMargins(0, 0, 0, 0);
    videoLayout->setSpacing(0);
    
    // 创建播放列表容器
    m_playlistContainer = new QWidget();
    m_playlistContainer->setMaximumWidth(300);
    m_playlistContainer->setMinimumWidth(250);
    m_playlistContainer->setStyleSheet("background-color: white; border-left: 1px solid #ccc;");
    
    // 创建播放列表
    m_playlistWidget = new QListWidget();
    m_playlistWidget->setStyleSheet("QListWidget { background-color: white; border: none; color: black; } QListWidget::item { padding: 8px; border-bottom: 1px solid #eee; } QListWidget::item:selected { background-color: #0078d4; color: white; } QListWidget::item:hover { background-color: #f5f5f5; color: black; } QListWidget::item:selected:hover { background-color: #0078d4; color: white; }");
    
    // 启用拖拽排序
    m_playlistWidget->setDragDropMode(QAbstractItemView::InternalMove);
    m_playlistWidget->setDefaultDropAction(Qt::MoveAction);
    
    // 创建上移下移按钮
    m_moveUpButton = new QPushButton("↑ 上移");
    m_moveUpButton->setStyleSheet("QPushButton { padding: 6px 12px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; } QPushButton:disabled { background-color: #f8f8f8; color: #ccc; }");
    m_moveUpButton->setToolTip("将选中的视频向上移动");
    
    m_moveDownButton = new QPushButton("↓ 下移");
    m_moveDownButton->setStyleSheet("QPushButton { padding: 6px 12px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; } QPushButton:disabled { background-color: #f8f8f8; color: #ccc; }");
    m_moveDownButton->setToolTip("将选中的视频向下移动");
    
    m_removeButton = new QPushButton("✕ 删除");
    m_removeButton->setStyleSheet("QPushButton { padding: 6px 12px; background-color: #ff4444; border: 1px solid #cc0000; border-radius: 4px; color: white; } QPushButton:hover { background-color: #cc0000; } QPushButton:disabled { background-color: #f8f8f8; color: #ccc; }");
    m_removeButton->setToolTip("删除选中的视频");
    
    // 初始状态下禁用按钮
    m_moveUpButton->setEnabled(false);
    m_moveDownButton->setEnabled(false);
    m_removeButton->setEnabled(false);
    
    // 播放列表标题
    QLabel *playlistTitle = new QLabel("播放列表");
    playlistTitle->setStyleSheet("color: black; font-weight: bold; padding: 10px; background-color: #f8f8f8; border-bottom: 1px solid #ccc;");
    playlistTitle->setAlignment(Qt::AlignCenter);
    
    // 创建按钮容器
    QWidget *buttonContainer = new QWidget();
    buttonContainer->setStyleSheet("background-color: #f8f8f8; border-top: 1px solid #ccc;");
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonContainer);
    buttonLayout->addWidget(m_moveUpButton);
    buttonLayout->addWidget(m_moveDownButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addStretch();
    buttonLayout->setContentsMargins(8, 8, 8, 8);
    buttonLayout->setSpacing(8);
    
    // 布局播放列表容器
    QVBoxLayout *playlistLayout = new QVBoxLayout(m_playlistContainer);
    playlistLayout->addWidget(playlistTitle);
    playlistLayout->addWidget(m_playlistWidget, 1);
    playlistLayout->addWidget(buttonContainer);
    playlistLayout->setContentsMargins(0, 0, 0, 0);
    playlistLayout->setSpacing(0);
    
    // 添加到主分割器
    m_mainSplitter->addWidget(m_videoContainer);
    m_mainSplitter->addWidget(m_playlistContainer);
    m_mainSplitter->setStretchFactor(0, 1);
    m_mainSplitter->setStretchFactor(1, 0);
    
    // 主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_mainSplitter);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    
    // 初始化音量图标
    updateVolumeIcon();
    
    // 初始化长按定时器
    m_longPressTimer = new QTimer(this);
    m_longPressTimer->setInterval(500); // 500ms后开始长按
    m_longPressTimer->setSingleShot(true);
    
    // 初始化连续跳转定时器
    m_continuousSeekTimer = new QTimer(this);
    m_continuousSeekTimer->setInterval(100); // 每100ms跳转一次
}

void MainWindow::setupConnections()
{
    // 播放控制连接
    connect(m_playButton, &QPushButton::clicked, this, &MainWindow::playVideo);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::stopVideo);
    connect(m_nextButton, &QPushButton::clicked, this, &MainWindow::playNextVideo);
    connect(m_openFolderButton, &QPushButton::clicked, this, &MainWindow::openFileOrFolder);
    connect(m_settingsButton, &QPushButton::clicked, this, &MainWindow::openSettings);
    connect(m_togglePlaylistButton, &QPushButton::clicked, this, &MainWindow::togglePlaylist);
    
    // 进度条连接
    connect(m_positionSlider, &QSlider::sliderMoved, this, &MainWindow::setPosition);
    connect(m_positionSlider, &QSlider::sliderPressed, [this]() { m_positionSliderPressed = true; });
    connect(m_positionSlider, &QSlider::sliderReleased, [this]() { m_positionSliderPressed = false; });
    
    // 音量控制连接
    connect(m_volumeSlider, &QSlider::valueChanged, this, &MainWindow::setVolume);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &MainWindow::updateVolumeIcon);
    connect(m_volumeButton, &QPushButton::clicked, this, &MainWindow::toggleMute);
    
    // 倍速连接
    connect(m_speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::changePlaybackRate);
    
    // 播放列表连接
    connect(m_playlistWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::onPlaylistItemDoubleClicked);
    connect(m_playlistWidget, &QListWidget::itemSelectionChanged, this, [this]() {
        bool hasSelection = m_playlistWidget->currentRow() >= 0;
        bool canMoveUp = hasSelection && m_playlistWidget->currentRow() > 0;
        bool canMoveDown = hasSelection && m_playlistWidget->currentRow() < m_playlistWidget->count() - 1;
        m_moveUpButton->setEnabled(canMoveUp);
        m_moveDownButton->setEnabled(canMoveDown);
        m_removeButton->setEnabled(hasSelection);
    });
    
    // 上移下移删除按钮连接
    connect(m_moveUpButton, &QPushButton::clicked, this, &MainWindow::moveItemUp);
    connect(m_moveDownButton, &QPushButton::clicked, this, &MainWindow::moveItemDown);
    connect(m_removeButton, &QPushButton::clicked, this, &MainWindow::removeSelectedVideo);
    
    // 媒体播放器连接
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::updatePosition);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::updateDuration);
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::mediaStatusChanged);
    connect(m_mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &MainWindow::playbackStateChanged);
    
    // 视频组件连接
    connect(m_videoWidget, &ClickableVideoWidget::doubleClicked, this, &MainWindow::onVideoWidgetDoubleClicked);
    
    // 长按定时器连接
    connect(m_longPressTimer, &QTimer::timeout, this, &MainWindow::onLongPressTimer);
    connect(m_continuousSeekTimer, &QTimer::timeout, this, &MainWindow::onContinuousSeekTimer);
    
    // 音频设备变化监听
    // 注意：QMediaDevices在Qt 6中是静态类，需要创建一个实例来连接信号
    QMediaDevices *mediaDevices = new QMediaDevices(this);
    connect(mediaDevices, &QMediaDevices::audioOutputsChanged, this, &MainWindow::onAudioOutputsChanged);
}

void MainWindow::openFileOrFolder()
{
    // 创建自定义对话框，支持选择文件或文件夹
    QFileDialog dialog(this);
    dialog.setWindowTitle("选择视频文件或文件夹");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setOption(QFileDialog::DontUseNativeDialog, true);
    
    // 设置视频文件过滤器
    QStringList filters;
    filters << "视频文件 (*.mp4 *.avi *.mkv *.mov *.wmv *.flv *.webm *.m4v *.3gp *.ts *.mts)"
            << "所有文件 (*.*)";
    dialog.setNameFilters(filters);
    
    // 设置起始目录
    if (!m_currentFolder.isEmpty()) {
        dialog.setDirectory(m_currentFolder);
    }
    
    if (dialog.exec() == QDialog::Accepted) {
        QStringList selectedFiles = dialog.selectedFiles();
        if (!selectedFiles.isEmpty()) {
            QString firstPath = selectedFiles.first();
            QFileInfo fileInfo(firstPath);
            
            if (fileInfo.isDir()) {
                // 选择的是文件夹，加载文件夹中的所有视频
                m_currentFolder = firstPath;
                loadVideosFromFolder(firstPath);
            } else {
                // 选择的是文件，添加所有选中的视频文件
                m_currentFolder = fileInfo.absolutePath();
                for (const QString &filePath : selectedFiles) {
                    addVideoFile(filePath);
                }
                
                // 如果添加了文件，自动播放第一个新添加的文件
                if (!selectedFiles.isEmpty()) {
                    QString firstFile = selectedFiles.first();
                    // 找到该文件在列表中的位置并播放
                    for (int i = 0; i < m_playlistWidget->count(); ++i) {
                        QListWidgetItem *item = m_playlistWidget->item(i);
                        if (item && item->data(Qt::UserRole).toString() == firstFile) {
                            m_currentPlayingIndex = i;
                            m_playlistWidget->setCurrentRow(i);
                            playVideoFile(firstFile);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::loadVideosFromFolder(const QString &folderPath)
{
    m_playlistWidget->clear();
    
    QDir dir(folderPath);
    QStringList videoExtensions = {"*.mp4", "*.avi", "*.mkv", "*.mov", "*.wmv", "*.flv", "*.webm", "*.m4v", "*.3gp", "*.ts", "*.mts"};
    
    QFileInfoList files = dir.entryInfoList(videoExtensions, QDir::Files | QDir::Readable, QDir::Name);
    
    for (const QFileInfo &fileInfo : files) {
        QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
        item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
        item->setToolTip(fileInfo.absoluteFilePath());
        m_playlistWidget->addItem(item);
    }
    
    // 移除弹窗提示，静默加载视频列表
}

void MainWindow::playVideo()
{
    if (m_mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        pauseVideo();
    } else {
        m_mediaPlayer->play();
    }
}

void MainWindow::pauseVideo()
{
    m_mediaPlayer->pause();
}

void MainWindow::stopVideo()
{
    m_mediaPlayer->stop();
    m_positionSlider->setValue(0);
}

void MainWindow::setPosition(int position)
{
    if (m_duration > 0) {
        qint64 newPosition = (position * m_duration) / 100;
        m_mediaPlayer->setPosition(newPosition);
    }
}

void MainWindow::updatePosition(qint64 position)
{
    if (m_duration > 0 && !m_positionSliderPressed) {
        int sliderPosition = (position * 100) / m_duration;
        m_positionSlider->blockSignals(true);
        m_positionSlider->setValue(sliderPosition);
        m_positionSlider->blockSignals(false);
    }
    
    // 更新时间显示
    QString currentTime, totalTime;
    formatTime(position, currentTime);
    formatTime(m_duration, totalTime);
    m_timeLabel->setText(currentTime + " / " + totalTime);
}

void MainWindow::updateDuration(qint64 duration)
{
    m_duration = duration;
    m_positionSlider->setRange(0, 100);
}

void MainWindow::onPlaylistItemDoubleClicked(QListWidgetItem *item)
{
    if (item) {
        QString filePath = item->data(Qt::UserRole).toString();
        m_currentPlayingIndex = m_playlistWidget->row(item);
        playVideoFile(filePath);
        
        // 高亮当前播放的项目
        m_playlistWidget->setCurrentItem(item);
    }
}

void MainWindow::playVideoFile(const QString &filePath)
{
    if (QFileInfo::exists(filePath)) {
        // 保存之前视频的播放位置
        saveVideoPosition();
        
        // 设置新视频
        m_mediaPlayer->setSource(QUrl::fromLocalFile(filePath));
        m_currentVideoHash = getVideoHash(filePath);
        setWindowTitle(QString("视频播放器 - %1").arg(QFileInfo(filePath).baseName()));
        
        // 开始播放
        m_mediaPlayer->play();
        
        // 检查是否有历史播放位置
        checkAndShowPositionDialog(filePath);
    } else {
        QMessageBox::warning(this, "错误", "文件不存在或无法访问");
    }
}

void MainWindow::changePlaybackRate()
{
    QString speedText = m_speedComboBox->currentText();
    speedText.remove("x");
    bool ok;
    qreal speed = speedText.toDouble(&ok);
    if (ok) {
        m_mediaPlayer->setPlaybackRate(speed);
    }
}

void MainWindow::togglePlaylist()
{
    m_playlistVisible = !m_playlistVisible;
    m_playlistContainer->setVisible(m_playlistVisible);
    m_togglePlaylistButton->setText(m_playlistVisible ? "隐藏播放列表" : "显示播放列表");
}

void MainWindow::mediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    switch (status) {
    case QMediaPlayer::LoadedMedia:
        // 媒体加载完成
        break;
    case QMediaPlayer::InvalidMedia:
        QMessageBox::warning(this, "错误", "无法播放该媒体文件");
        break;
    case QMediaPlayer::EndOfMedia:
        // 视频播放完成，自动播放下一个
        playNextVideoAuto();
        break;
    default:
        break;
    }
}

void MainWindow::playbackStateChanged(QMediaPlayer::PlaybackState state)
{
    updatePlayButton();
}

void MainWindow::updatePlayButton()
{
    switch (m_mediaPlayer->playbackState()) {
    case QMediaPlayer::PlayingState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    case QMediaPlayer::PausedState:
    case QMediaPlayer::StoppedState:
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void MainWindow::formatTime(qint64 timeInMs, QString &str)
{
    qint64 seconds = timeInMs / 1000;
    qint64 minutes = seconds / 60;
    qint64 hours = minutes / 60;
    
    seconds %= 60;
    minutes %= 60;
    
    if (hours > 0) {
        str = QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    } else {
        str = QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    }
}

void MainWindow::setVolume(int volume)
{
    if (m_audioOutput) {
        qreal linearVolume = QAudio::convertVolume(volume / 100.0, QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
        m_audioOutput->setVolume(linearVolume);
    }
}

// onProgressBarClicked函数已移除，现在使用ClickableSlider类处理点击事件

void MainWindow::saveSettings()
{
    if (m_settings) {
        m_settings->setValue("lastFolder", m_currentFolder);
        m_settings->setValue("volume", m_volumeSlider->value());
        m_settings->setValue("playlistVisible", m_playlistVisible);
        m_settings->setValue("windowGeometry", saveGeometry());
        m_settings->setValue("windowState", saveState());
    }
}

void MainWindow::loadSettings()
{
    if (m_settings) {
        // 加载上次的文件夹
        QString lastFolder = m_settings->value("lastFolder", "").toString();
        if (!lastFolder.isEmpty() && QDir(lastFolder).exists()) {
            m_currentFolder = lastFolder;
            loadVideosFromFolder(lastFolder);
        }
        
        // 加载音量设置
        int volume = m_settings->value("volume", 70).toInt();
        m_volumeSlider->setValue(volume);
        setVolume(volume);
        
        // 加载播放列表可见性
        bool playlistVisible = m_settings->value("playlistVisible", true).toBool();
        if (playlistVisible != m_playlistVisible) {
            togglePlaylist();
        }
        
        // 加载窗口几何和状态
        restoreGeometry(m_settings->value("windowGeometry").toByteArray());
        restoreState(m_settings->value("windowState").toByteArray());
        
        // 加载长按倍速设置
        m_leftKeySpeed = m_settings->value("leftKeySpeed", 2.0).toDouble();
        m_rightKeySpeed = m_settings->value("rightKeySpeed", 2.0).toDouble();
    }
}

void MainWindow::setupProgressBarClickable()
{
    // 这个函数可以用于未来扩展进度条的点击功能
    // 目前通过自定义ClickableSlider类来实现点击功能
}

void MainWindow::playNextVideo()
{
    if (m_playlistWidget->count() == 0) {
        return; // 没有视频列表
    }
    
    int nextIndex;
    if (m_currentPlayingIndex == -1 || m_currentPlayingIndex >= m_playlistWidget->count() - 1) {
        // 如果没有当前播放项目或者是最后一个，播放第一个
        nextIndex = 0;
    } else {
        // 播放下一个
        nextIndex = m_currentPlayingIndex + 1;
    }
    
    QListWidgetItem *nextItem = m_playlistWidget->item(nextIndex);
    if (nextItem) {
        QString filePath = nextItem->data(Qt::UserRole).toString();
        m_currentPlayingIndex = nextIndex;
        playVideoFile(filePath);
        
        // 高亮当前播放的项目
        m_playlistWidget->setCurrentItem(nextItem);
    }
}

void MainWindow::toggleMute()
{
    if (m_isMuted) {
        // 取消静音，恢复之前的音量
        m_volumeSlider->setValue(m_previousVolume);
        m_isMuted = false;
    } else {
        // 静音，保存当前音量并设置为0
        m_previousVolume = m_volumeSlider->value();
        m_volumeSlider->setValue(0);
        m_isMuted = true;
    }
    updateVolumeIcon();
}

void MainWindow::updateVolumeIcon()
{
    int volume = m_volumeSlider->value();
    QString iconPath;
    
    if (volume == 0 || m_isMuted) {
        iconPath = ":/Images/Volume-0.png";
    } else if (volume <= 25) {
        iconPath = ":/Images/Volume-25.png";
    } else if (volume <= 50) {
        iconPath = ":/Images/Volume-50.png";
    } else if (volume <= 75) {
        iconPath = ":/Images/Volume-75.png";
    } else {
        iconPath = ":/Images/Volume-100.png";
    }
    
    QIcon volumeIcon(iconPath);
    m_volumeButton->setIcon(volumeIcon);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        if (!event->isAutoRepeat()) {
            // 首次按下，启动定时器
            m_longPressForward = false;
            m_longPressTimer->start();
        }
        break;
    case Qt::Key_Right:
        if (!event->isAutoRepeat()) {
            // 首次按下，启动定时器
            m_longPressForward = true;
            m_longPressTimer->start();
        }
        break;
    default:
        QMainWindow::keyPressEvent(event);
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
    case Qt::Key_Right:
        if (!event->isAutoRepeat()) {
            if (m_longPressTimer->isActive()) {
                // 短按，快进/快退15秒
                m_longPressTimer->stop();
                int seconds = (event->key() == Qt::Key_Left) ? -15 : 15;
                seekVideo(seconds);
            } else if (m_isLongPressing) {
                // 停止长按快进/快退
                m_isLongPressing = false;
                m_continuousSeekTimer->stop();
            }
        }
        break;
    default:
        QMainWindow::keyReleaseEvent(event);
        break;
    }
}

void MainWindow::onLongPressTimer()
{
    // 开始长按快进/快退
    m_isLongPressing = true;
    m_continuousSeekTimer->start();
}

void MainWindow::seekVideo(int seconds)
{
    if (m_mediaPlayer && m_duration > 0) {
        qint64 currentPos = m_mediaPlayer->position();
        qint64 newPos = currentPos + (seconds * 1000); // 转换为毫秒
        newPos = qBound(0LL, newPos, m_duration);
        m_mediaPlayer->setPosition(newPos);
    }
}

void MainWindow::startFastSeek(bool forward)
{
    if (m_mediaPlayer) {
        double rate = forward ? m_rightKeySpeed : m_leftKeySpeed;
        m_mediaPlayer->setPlaybackRate(rate);
    }
}

void MainWindow::onContinuousSeekTimer()
{
    if (m_isLongPressing && m_mediaPlayer && m_duration > 0) {
        // 根据方向和倍速计算跳转距离
        double speed = m_longPressForward ? m_rightKeySpeed : m_leftKeySpeed;
        int seekDistance = m_longPressForward ? (int)(speed * 1000) : -(int)(speed * 1000); // 每次跳转的毫秒数
        
        qint64 currentPos = m_mediaPlayer->position();
        qint64 newPos = currentPos + seekDistance;
        newPos = qBound(0LL, newPos, m_duration);
        
        m_mediaPlayer->setPosition(newPos);
    }
}

void MainWindow::openSettings()
{
    if (!m_settingsDialog) {
        m_settingsDialog = new SettingsDialog(this);
    }
    
    m_settingsDialog->setLeftKeySpeed(m_leftKeySpeed);
    m_settingsDialog->setRightKeySpeed(m_rightKeySpeed);
    
    if (m_settingsDialog->exec() == QDialog::Accepted) {
        m_leftKeySpeed = m_settingsDialog->getLeftKeySpeed();
        m_rightKeySpeed = m_settingsDialog->getRightKeySpeed();
        // 保存设置
        if (m_settings) {
            m_settings->setValue("leftKeySpeed", m_leftKeySpeed);
            m_settings->setValue("rightKeySpeed", m_rightKeySpeed);
        }
     }
}

void MainWindow::saveVideoPosition()
{
    if (m_mediaPlayer && !m_currentVideoHash.isEmpty() && m_duration > 0) {
        qint64 currentPosition = m_mediaPlayer->position();
        // 只有播放超过30秒且不在最后30秒时才保存位置
        if (currentPosition > 30000 && currentPosition < (m_duration - 30000)) {
            QString key = QString("videoPosition_%1").arg(m_currentVideoHash);
            if (m_settings) {
                m_settings->setValue(key, currentPosition);
            }
        }
    }
}

void MainWindow::checkAndShowPositionDialog(const QString &filePath)
{
    if (!m_settings || m_currentVideoHash.isEmpty()) {
        return;
    }
    
    QString key = QString("videoPosition_%1").arg(m_currentVideoHash);
    qint64 lastPosition = m_settings->value(key, -1).toLongLong();
    
    if (lastPosition > 0) {
        // 有历史播放位置，显示提示对话框
        QString videoName = QFileInfo(filePath).baseName();
        
        if (m_positionDialog) {
            m_positionDialog->deleteLater();
        }
        
        m_positionDialog = new PositionDialog(videoName, lastPosition, this);
        connect(m_positionDialog, &QDialog::finished, this, &MainWindow::onPositionDialogFinished);
        
        m_pendingJumpPosition = lastPosition;
        m_positionDialog->show();
    }
}

void MainWindow::onPositionDialogFinished()
{
    if (m_positionDialog && m_positionDialog->shouldJumpToPosition() && m_pendingJumpPosition > 0) {
        // 用户选择跳转到历史位置
        m_mediaPlayer->setPosition(m_pendingJumpPosition);
    }
    
    m_pendingJumpPosition = -1;
    
    if (m_positionDialog) {
        m_positionDialog->deleteLater();
        m_positionDialog = nullptr;
    }
}

QString MainWindow::getVideoHash(const QString &filePath)
{
    QFileInfo fileInfo(filePath);
    QString hashSource = QString("%1_%2_%3")
                           .arg(fileInfo.absoluteFilePath())
                           .arg(fileInfo.size())
                           .arg(fileInfo.lastModified().toString(Qt::ISODate));
    
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(hashSource.toUtf8());
    return hash.result().toHex();
}

void MainWindow::onVideoWidgetDoubleClicked()
{
    // 双击视频播放区域时切换播放/暂停状态
    playVideo();
}

void MainWindow::moveItemUp()
{
    int currentRow = m_playlistWidget->currentRow();
    if (currentRow > 0) {
        // 保存当前播放的索引
        bool wasCurrentPlaying = (currentRow == m_currentPlayingIndex);
        bool wasAbovePlaying = (currentRow - 1 == m_currentPlayingIndex);
        
        // 移动项目
        QListWidgetItem *item = m_playlistWidget->takeItem(currentRow);
        m_playlistWidget->insertItem(currentRow - 1, item);
        m_playlistWidget->setCurrentRow(currentRow - 1);
        
        // 更新当前播放索引
        if (wasCurrentPlaying) {
            m_currentPlayingIndex = currentRow - 1;
        } else if (wasAbovePlaying) {
            m_currentPlayingIndex = currentRow;
        }
    }
}

void MainWindow::moveItemDown()
{
    int currentRow = m_playlistWidget->currentRow();
    if (currentRow >= 0 && currentRow < m_playlistWidget->count() - 1) {
        // 保存当前播放的索引
        bool wasCurrentPlaying = (currentRow == m_currentPlayingIndex);
        bool wasBelowPlaying = (currentRow + 1 == m_currentPlayingIndex);
        
        // 移动项目
        QListWidgetItem *item = m_playlistWidget->takeItem(currentRow);
        m_playlistWidget->insertItem(currentRow + 1, item);
        m_playlistWidget->setCurrentRow(currentRow + 1);
        
        // 更新当前播放索引
        if (wasCurrentPlaying) {
            m_currentPlayingIndex = currentRow + 1;
        } else if (wasBelowPlaying) {
            m_currentPlayingIndex = currentRow;
        }
    }
}

void MainWindow::playNextVideoAuto()
{
    if (m_playlistWidget->count() == 0) {
        return;
    }
    
    // 检查当前播放的是否是最后一个视频
    if (m_currentPlayingIndex >= m_playlistWidget->count() - 1) {
        // 已经是最后一个视频，显示完成提示
        m_mediaPlayer->stop();
        
        // 在视频区域显示完成提示
        QLabel *completionLabel = new QLabel("视频列表播放完了", m_videoWidget);
        completionLabel->setStyleSheet("QLabel { color: white; font-size: 24px; font-weight: bold; background-color: rgba(0, 0, 0, 0.7); padding: 20px; border-radius: 10px; }");
        completionLabel->setAlignment(Qt::AlignCenter);
        
        // 设置标签大小和位置居中
        completionLabel->resize(300, 80);
        int x = (m_videoWidget->width() - completionLabel->width()) / 2;
        int y = (m_videoWidget->height() - completionLabel->height()) / 2;
        completionLabel->move(x, y);
        completionLabel->show();
        
        // 5秒后自动隐藏提示
        QTimer::singleShot(5000, completionLabel, &QLabel::deleteLater);
        
        // 不重置播放索引，保持当前状态
        return;
    }
    
    // 还有下一个视频，自动播放
    int nextIndex = m_currentPlayingIndex + 1;
    m_currentPlayingIndex = nextIndex;
    m_playlistWidget->setCurrentRow(nextIndex);
    
    QListWidgetItem *nextItem = m_playlistWidget->item(nextIndex);
    if (nextItem) {
        QString filePath = nextItem->data(Qt::UserRole).toString();
        playVideoFile(filePath);
    }
}

void MainWindow::onAudioOutputsChanged()
{
    if (!m_audioOutput || !m_mediaPlayer) {
        return;
    }
    
    // 保存当前音量和静音状态
    qreal currentVolume = m_audioOutput->volume();
    bool wasMuted = m_audioOutput->isMuted();
    
    // 创建新的音频输出对象（会自动使用系统默认音频设备）
    QAudioOutput *newAudioOutput = new QAudioOutput(this);
    
    // 恢复音量和静音状态
    newAudioOutput->setVolume(currentVolume);
    newAudioOutput->setMuted(wasMuted);
    
    // 更新媒体播放器的音频输出
    m_mediaPlayer->setAudioOutput(newAudioOutput);
    
    // 删除旧的音频输出对象
    m_audioOutput->deleteLater();
    m_audioOutput = newAudioOutput;
    
    // 更新音量图标（以防静音状态改变）
    updateVolumeIcon();
}

void MainWindow::addVideoFile(const QString &filePath)
{
    if (!QFileInfo::exists(filePath)) {
        return;
    }
    
    QFileInfo fileInfo(filePath);
    
    // 检查是否是支持的视频格式
    QStringList supportedExtensions = {"mp4", "avi", "mkv", "mov", "wmv", "flv", "webm", "m4v", "3gp", "ts", "mts"};
    if (!supportedExtensions.contains(fileInfo.suffix().toLower())) {
        return;
    }
    
    // 检查文件是否已经在播放列表中
    for (int i = 0; i < m_playlistWidget->count(); ++i) {
        QListWidgetItem *existingItem = m_playlistWidget->item(i);
        if (existingItem && existingItem->data(Qt::UserRole).toString() == filePath) {
            // 文件已存在，不重复添加
            return;
        }
    }
    
    // 添加到播放列表
    QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
    item->setData(Qt::UserRole, fileInfo.absoluteFilePath());
    item->setToolTip(fileInfo.absoluteFilePath());
    m_playlistWidget->addItem(item);
}

void MainWindow::removeSelectedVideo()
{
    int currentRow = m_playlistWidget->currentRow();
    if (currentRow < 0) {
        return;
    }
    
    // 如果要删除的是当前播放的视频
    bool wasCurrentPlaying = (currentRow == m_currentPlayingIndex);
    
    // 删除项目
    QListWidgetItem *item = m_playlistWidget->takeItem(currentRow);
    if (item) {
        delete item;
    }
    
    // 更新当前播放索引
    if (wasCurrentPlaying) {
        // 如果删除的是当前播放的视频，停止播放
        m_mediaPlayer->stop();
        m_currentPlayingIndex = -1;
        setWindowTitle("视频播放器");
    } else if (m_currentPlayingIndex > currentRow) {
        // 如果删除的视频在当前播放视频之前，索引需要减1
        m_currentPlayingIndex--;
    }
    
    // 如果播放列表为空，重置状态
    if (m_playlistWidget->count() == 0) {
        m_currentPlayingIndex = -1;
        m_mediaPlayer->stop();
        setWindowTitle("视频播放器");
    }
}
