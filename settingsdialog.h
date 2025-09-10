#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    
    // 获取和设置长按倍速
    double getLeftKeySpeed() const;
    double getRightKeySpeed() const;
    void setLeftKeySpeed(double speed);
    void setRightKeySpeed(double speed);

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    void setupUI();
    void setupConnections();
    
    QComboBox *m_leftSpeedComboBox;
    QComboBox *m_rightSpeedComboBox;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    
    double m_originalLeftSpeed;
    double m_originalRightSpeed;
};

#endif // SETTINGSDIALOG_H