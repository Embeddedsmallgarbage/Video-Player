#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_leftSpeedComboBox(nullptr)
    , m_rightSpeedComboBox(nullptr)
    , m_okButton(nullptr)
    , m_cancelButton(nullptr)
    , m_originalLeftSpeed(2.0)
    , m_originalRightSpeed(2.0)
{
    setupUI();
    setupConnections();
    
    setWindowTitle("设置");
    setFixedSize(400, 220);
    setModal(true);
}

void SettingsDialog::setupUI()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建设置组
    QGroupBox *speedGroup = new QGroupBox("长按方向键倍速设置");
    speedGroup->setStyleSheet("QGroupBox { font-weight: bold; color: black; border: 1px solid #ccc; border-radius: 4px; margin: 5px 0; padding-top: 10px; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }");
    
    QVBoxLayout *speedLayout = new QVBoxLayout(speedGroup);
    
    // 创建说明标签
    QLabel *descLabel = new QLabel("分别设置长按左右方向键时的快退/快进倍速:");
    descLabel->setStyleSheet("color: black; margin: 5px 0;");
    descLabel->setWordWrap(true);
    
    // 创建左方向键设置
    QHBoxLayout *leftLayout = new QHBoxLayout();
    QLabel *leftLabel = new QLabel("左方向键(快退):");
    leftLabel->setStyleSheet("color: black;");
    leftLabel->setMinimumWidth(120);
    
    m_leftSpeedComboBox = new QComboBox();
    m_leftSpeedComboBox->addItems({"0.5x", "0.75x", "1.0x", "1.25x", "1.5x", "1.75x", "2.0x", "2.25x", "2.5x", "2.75x", "3.0x"});
    m_leftSpeedComboBox->setCurrentText("2.0x");
    m_leftSpeedComboBox->setStyleSheet("QComboBox { padding: 5px 10px; background-color: white; border: 1px solid #ccc; border-radius: 4px; color: black; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border: none; } QComboBox QAbstractItemView { background-color: white; color: black; border: 1px solid #ccc; }");
    
    leftLayout->addWidget(leftLabel);
    leftLayout->addWidget(m_leftSpeedComboBox);
    
    // 创建右方向键设置
    QHBoxLayout *rightLayout = new QHBoxLayout();
    QLabel *rightLabel = new QLabel("右方向键(快进):");
    rightLabel->setStyleSheet("color: black;");
    rightLabel->setMinimumWidth(120);
    
    m_rightSpeedComboBox = new QComboBox();
    m_rightSpeedComboBox->addItems({"0.5x", "0.75x", "1.0x", "1.25x", "1.5x", "1.75x", "2.0x", "2.25x", "2.5x", "2.75x", "3.0x"});
    m_rightSpeedComboBox->setCurrentText("2.0x");
    m_rightSpeedComboBox->setStyleSheet("QComboBox { padding: 5px 10px; background-color: white; border: 1px solid #ccc; border-radius: 4px; color: black; } QComboBox::drop-down { border: none; } QComboBox::down-arrow { image: none; border: none; } QComboBox QAbstractItemView { background-color: white; color: black; border: 1px solid #ccc; }");
    
    rightLayout->addWidget(rightLabel);
    rightLayout->addWidget(m_rightSpeedComboBox);
    
    speedLayout->addWidget(descLabel);
    speedLayout->addLayout(leftLayout);
    speedLayout->addLayout(rightLayout);
    
    // 创建按钮布局
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_okButton = new QPushButton("确定");
    m_okButton->setStyleSheet("QPushButton { padding: 8px 20px; background-color: #0078d4; border: none; border-radius: 4px; color: white; font-weight: bold; } QPushButton:hover { background-color: #106ebe; } QPushButton:pressed { background-color: #005a9e; }");
    
    m_cancelButton = new QPushButton("取消");
    m_cancelButton->setStyleSheet("QPushButton { padding: 8px 20px; background-color: #f0f0f0; border: 1px solid #ccc; border-radius: 4px; color: black; } QPushButton:hover { background-color: #e0e0e0; } QPushButton:pressed { background-color: #d0d0d0; }");
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_okButton);
    
    // 添加到主布局
    mainLayout->addWidget(speedGroup);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(15, 15, 15, 15);
}

void SettingsDialog::setupConnections()
{
    connect(m_okButton, &QPushButton::clicked, this, &SettingsDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

double SettingsDialog::getLeftKeySpeed() const
{
    QString speedText = m_leftSpeedComboBox->currentText();
    speedText.remove("x");
    bool ok;
    double speed = speedText.toDouble(&ok);
    return ok ? speed : 2.0;
}

double SettingsDialog::getRightKeySpeed() const
{
    QString speedText = m_rightSpeedComboBox->currentText();
    speedText.remove("x");
    bool ok;
    double speed = speedText.toDouble(&ok);
    return ok ? speed : 2.0;
}

void SettingsDialog::setLeftKeySpeed(double speed)
{
    QString speedText = QString::number(speed) + "x";
    int index = m_leftSpeedComboBox->findText(speedText);
    if (index != -1) {
        m_leftSpeedComboBox->setCurrentIndex(index);
    }
    m_originalLeftSpeed = speed;
}

void SettingsDialog::setRightKeySpeed(double speed)
{
    QString speedText = QString::number(speed) + "x";
    int index = m_rightSpeedComboBox->findText(speedText);
    if (index != -1) {
        m_rightSpeedComboBox->setCurrentIndex(index);
    }
    m_originalRightSpeed = speed;
}

void SettingsDialog::onOkClicked()
{
    accept();
}

void SettingsDialog::onCancelClicked()
{
    // 恢复原始设置
    setLeftKeySpeed(m_originalLeftSpeed);
    setRightKeySpeed(m_originalRightSpeed);
    reject();
}