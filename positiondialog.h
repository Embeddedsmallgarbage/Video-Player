#ifndef POSITIONDIALOG_H
#define POSITIONDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>

class PositionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PositionDialog(const QString &videoName, qint64 lastPosition, QWidget *parent = nullptr);
    
    bool shouldJumpToPosition() const { return m_jumpToPosition; }
    
protected:
    void showEvent(QShowEvent *event) override;
    
private slots:
    void onYesClicked();
    void onNoClicked();
    void onAutoClose();
    
private:
    void setupUI(const QString &videoName, qint64 lastPosition);
    void setupConnections();
    QString formatTime(qint64 timeInMs);
    
    QLabel *m_messageLabel;
    QPushButton *m_yesButton;
    QPushButton *m_noButton;
    QTimer *m_autoCloseTimer;
    
    bool m_jumpToPosition;
    static const int AUTO_CLOSE_SECONDS = 10;
};

#endif // POSITIONDIALOG_H