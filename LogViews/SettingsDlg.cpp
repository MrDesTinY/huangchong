#include "SettingsDlg.h"

#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "UserSettings.h"

SettingsDlg::SettingsDlg(QWidget *parent, UserSettings* settings):
        QDialog(parent),
        userSettings(settings),
        udpRBtn(NULL),
        btRBtn(NULL),
        commRBtn(NULL),
        okButton(NULL),
        cancelButton(NULL),
        connTypeGroupBox(NULL),
        portGroupBox(NULL),
        portHBox(NULL),
        portLabel(NULL),
        portEdit(NULL)
{
    connTypeGroupBox = new QGroupBox(tr("Connection type"));
    udpRBtn = new QRadioButton(tr("&UDP"));
    connect(udpRBtn, SIGNAL(toggled(bool)), SLOT(onRadioToggled(bool)));

    btRBtn = new QRadioButton(tr("&Bluetooth"));
    connect(btRBtn, SIGNAL(toggled(bool)), SLOT(onRadioToggled(bool)));

    commRBtn = new QRadioButton(tr("&Serial"));
    connect(commRBtn, SIGNAL(toggled(bool)), SLOT(onRadioToggled(bool)));
    commRBtn->setChecked(true);

    QVBoxLayout* connTypeVBox = new QVBoxLayout;
    connTypeVBox->addWidget(udpRBtn);
    connTypeVBox->addWidget(btRBtn);
    connTypeVBox->addWidget(commRBtn);
    connTypeVBox->addStretch(1);
    connTypeGroupBox->setLayout(connTypeVBox);

    portGroupBox = new QGroupBox(tr("Port"));
    portLabel = new QLabel(tr("COM port:"));
    portLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    portLabel->setFixedWidth(50);
    portEdit = new QLineEdit();
    portEdit->setFixedWidth(80);
    portHBox = new QHBoxLayout;
    portHBox->addWidget(portLabel);
    portHBox->addWidget(portEdit);
    portGroupBox->setLayout(portHBox);
    portGroupBox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    okButton = new QPushButton(tr("&Ok"));
    okButton->setDefault(true);
    okButton->setEnabled(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));

    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->setDefault(false);
    cancelButton->setEnabled(true);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QVBoxLayout* mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(connTypeGroupBox);
    mainVLayout->addWidget(portGroupBox);
    QHBoxLayout* bottomHLayout = new QHBoxLayout;
    bottomHLayout->addWidget(okButton);
    bottomHLayout->addWidget(cancelButton);
    mainVLayout->addLayout(bottomHLayout);
    setLayout(mainVLayout);

    this->setWindowTitle(tr("Settings"));

    setModal(true);
    layout()->setSizeConstraint(QLayout::SetFixedSize);

    LoadFromSettings();
}

void SettingsDlg::LoadFromSettings()
{
    switch(userSettings->connectionType)
    {
    case CONN_UDP:
        udpRBtn->setChecked(true);
        portEdit->setText(QString::number(userSettings->udpPort));
        break;
    case CONN_BLUETOOTH:
        btRBtn->setChecked(true);
        break;
    case CONN_COMM:
        commRBtn->setChecked(true);
        portEdit->setText(userSettings->commPort);
        break;
    }
}

void SettingsDlg::PopulateSettings()
{
    if(udpRBtn->isChecked())
    {
        userSettings->connectionType = ConnectionType::CONN_UDP;
        bool ok = false;
        int udpPort = portEdit->text().toInt(&ok);
        if(ok)
        {
            userSettings->udpPort = udpPort;
        }
    }
    else if(btRBtn->isChecked())
    {
        userSettings->connectionType = ConnectionType::CONN_BLUETOOTH;
    }
    else if(commRBtn->isChecked())
    {
        userSettings->connectionType = ConnectionType::CONN_COMM;
        userSettings->commPort = portEdit->text();
    }
}

void SettingsDlg::okClicked()
{
    PopulateSettings();
    accept();
}

void SettingsDlg::cancelClicked()
{
    close();
}

void SettingsDlg::onRadioToggled(bool checked)
{
    if(portLabel == NULL || portEdit == NULL)
        return;
    QRadioButton* radioBtn = qobject_cast< QRadioButton* >(QObject::sender());
    if(radioBtn == udpRBtn)
    {
        if(checked)
        {
            portGroupBox->setEnabled(true);
            portLabel->setEnabled(true);
            portLabel->setText(tr("UDP port:"));
            portEdit->setEnabled(true);
            portEdit->setText(QString::number(userSettings->udpPort));
        }
    }
    else if(radioBtn == btRBtn)
    {
        if(checked)
        {
            portGroupBox->setEnabled(false);
            portLabel->setText(tr("Port:"));
            portLabel->setEnabled(false);
            portEdit->setText("");
            portEdit->setEnabled(false);
        }
    }
    else if(radioBtn == commRBtn)
    {
        if(checked)
        {
            portGroupBox->setEnabled(true);
            portLabel->setEnabled(true);
            portLabel->setText(tr("COM port:"));
            portEdit->setEnabled(true);
            portEdit->setText(userSettings->commPort);
        }
    }
}
