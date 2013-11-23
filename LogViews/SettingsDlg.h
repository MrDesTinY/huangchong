#ifndef __SETTINGS_DLG_H__
#define __SETTINGS_DLG_H__

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;
class QGroupBox;
class QHBoxLayout;
class QRadioButton;
class UserSettings;

class SettingsDlg : public QDialog
{
    Q_OBJECT
public:
    SettingsDlg(QWidget *parent, UserSettings* settings);

private:
    // Helper methods:
    void LoadFromSettings();
    void PopulateSettings();

    // Data:
    UserSettings* userSettings;
    QRadioButton* udpRBtn;
    QRadioButton* btRBtn;
    QRadioButton* commRBtn;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QGroupBox* connTypeGroupBox;
    QGroupBox* portGroupBox;
    QHBoxLayout* portHBox;
    QLabel* portLabel;
    QLineEdit* portEdit;

private slots:
    void okClicked();
    void cancelClicked();
    void onRadioToggled(bool checked);
};

#endif//__SETTINGS_DLG_H__
