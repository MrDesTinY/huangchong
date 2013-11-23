/*
 * Copyright 2010 Ionut Dediu, deionut@yahoo.com
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __GO_TO_LINE_DLG_H__
#define __GO_TO_LINE_DLG_H__

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class GoToLineDlg : public QDialog
{
    Q_OBJECT
public:
    GoToLineDlg(QWidget *parent, int minLineNumber, int maxLineNumber, int defaultLineNumber);
    int getLineNo();

private:
    // Data:
    QLabel* label;
    QLineEdit* lineEdit;
    QPushButton* okButton;
    QPushButton* cancelButton;
    int minLineNo;
    int maxLineNo;
    int defaultLineNo;
    int selLineNo;
private slots:
    void okClicked();
    void cancelClicked();
};

#endif//__GO_TO_LINE_DLG_H__
