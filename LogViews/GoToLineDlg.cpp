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

#include "GoToLineDlg.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>

GoToLineDlg::GoToLineDlg(QWidget *parent, int minLineNumber, int maxLineNumber, int defaultLineNumber):
        QDialog(parent),
        minLineNo(minLineNumber),
        maxLineNo(maxLineNumber),
        defaultLineNo(defaultLineNumber),
        selLineNo(-1)
{
    QString labelText = tr("Go to line number (%1 - %2):")
                                 .arg(minLineNo)
                                 .arg(maxLineNo);
    label = new QLabel(labelText);
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);
    QValidator* intValidator = new QIntValidator(minLineNo, maxLineNo, this);
    lineEdit->setValidator(intValidator);
    if(minLineNo <= defaultLineNo && defaultLineNo <= maxLineNo)
        lineEdit->setText(QString::number(defaultLineNo));
    else
        lineEdit->setText(QString::null);
    lineEdit->selectAll();

    okButton = new QPushButton(tr("&Ok"));
    okButton->setDefault(true);
    okButton->setEnabled(true);
    connect(okButton, SIGNAL(clicked()), this, SLOT(okClicked()));

    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->setDefault(false);
    cancelButton->setEnabled(true);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));

    QVBoxLayout* mainVLayout = new QVBoxLayout;
    mainVLayout->addWidget(label);
    mainVLayout->addWidget(lineEdit);
    QHBoxLayout* bottomHLayout = new QHBoxLayout;
    bottomHLayout->addWidget(okButton);
    bottomHLayout->addWidget(cancelButton);
    mainVLayout->addLayout(bottomHLayout);
    setLayout(mainVLayout);

    setModal(true);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

int GoToLineDlg::getLineNo()
{
    return selLineNo;
}

void GoToLineDlg::okClicked()
{
    selLineNo = lineEdit->text().toInt();
    lineEdit->setText(QString::null);
    close();
}

void GoToLineDlg::cancelClicked()
{
    lineEdit->setText(QString::null);
    selLineNo = -1;
    close();
}
