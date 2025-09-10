#include "positiondialog.h"
#include <QShowEvent>
#include <QApplication>
#include <QScreen>

PositionDialog::PositionDialog(const QString &videoName, qint64 lastPosition, QWidget *parent)
    : QDialog(parent)
    , m_messageLabel(nullptr)
    , m_yesButton(nullptr)
    , m_noButton(nullptr)
    , m_autoCloseTimer(nullptr)
    , m_jumpToPosition(false)
{
    setupUI(videoName, lastPosition);
    setupConnections();
    
    setWindowTitle("继续播放");
    setFixedSize(400, 150);
    setModal(false); // 非模态对话框，不阻塞视频播放
    
    // 设置窗口标志，使其始终在最前面但不阻塞
    setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    
    // 启动自动关闭定时器
    m_autoCloseTimer = new QTimer(this);
    m_autoCloseTimer->setSingleShot(true);
    m_autoCloseTimer->setInterval(AUTO_CLOSE_SECONDS * 1000);
}

void PositionDialog::setupUI(const QString &videoName, qint64 lastPosition)
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建消息标签
    QString timeStr = formatTime(lastPosition);
    QString message = QString("检测到您之前观看过《%1》\n\n是否跳转到上次观看位置 %2 ？\n\n(此对话框将在%3秒后自动关闭)")
                        .arg(videoName)
                        .arg(timeStr)
                        .arg(AUTO_CLOSE_SECONDS);
    
    m_messageLabel = new QLabel(message);
    m_messageLabel->setStyleSheet("color: black; font-size: 12px; padding: 10px;");
    m_messageLabel->setAlignment(Qt::AlignCenter);
    m_messageLabel->setWordWrap(true);
    
    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_yesButton = new QPushButton("是，跳转");
    m_yesButton->setStyleSheet("QPushButton { padding: 8px 20px; background-color: #0078d4; border: none; border-radius: 4px; color: white; font-weight: bold; } QPushButton:hover { background-color: #106ebe; } QPushButton:pressed { background-color: #005a9e; }");
    
    m_noButton = new QPushButton("否，从头播放");
    m_noButton->setStyleSheet("QPushButton { padding: 8px 20px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; } QPushButton:pressed { background-color: #d0d0d0; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_noButton);
    buttonLayout->addWidget(m_yesButton);
    
    // 添加到主布局
    mainLayout->addWidget(m_messageLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 设置整体样式
    setStyleSheet("QDialog { background-color: white; border: 2px solid #0078d4; border-radius: 8px; }");
}

void PositionDialog::setupConnections()
{
    connect(m_yesButton, &QPushButton::clicked, this, &PositionDialog::onYesClicked);
    connect(m_noButton, &QPushButton::clicked, this, &PositionDialog::onNoClicked);
    connect(m_autoCloseTimer, &QTimer::timeout, this, &PositionDialog::onAutoClose);
}

void PositionDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    
    // 启动自动关闭定时器
    m_autoCloseTimer->start();
    
    // 将对话框定位到视频播放界面的左下角
    if (parentWidget()) {
        QRect parentGeometry = parentWidget()->geometry();
        int x = parentGeometry.x() + 20; // 距离左边缘20像素
        int y = parentGeometry.y() + parentGeometry.height() - height() - 20; // 距离底边缘20像素
        move(x, y);
    } else {
        // 如果没有父窗口，则显示在屏幕左下角
        QScreen *screen = QApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();
        int x = screenGeometry.x() + 20;
        int y = screenGeometry.y() + screenGeometry.height() - height() - 50;
        move(x, y);
    }
}

void PositionDialog::onYesClicked()
{
    m_jumpToPosition = true;
    m_autoCloseTimer->stop();
    accept();
}

void PositionDialog::onNoClicked()
{
    m_jumpToPosition = false;
    m_autoCloseTimer->stop();
    accept();
}

void PositionDialog::onAutoClose()
{
    // 自动关闭时默认不跳转
    m_jumpToPosition = false;
    accept();
}

QString PositionDialog::formatTime(qint64 timeInMs)
{
    qint64 seconds = timeInMs / 1000;
    qint64 minutes = seconds / 60;
    qint64 hours = minutes / 60;
    
    seconds %= 60;
    minutes %= 60;
    
    if (hours > 0) {
        return QString("%1:%2:%3")
                .arg(hours, 2, 10, QChar('0'))
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    } else {
        return QString("%1:%2")
                .arg(minutes, 2, 10, QChar('0'))
                .arg(seconds, 2, 10, QChar('0'));
    }
}