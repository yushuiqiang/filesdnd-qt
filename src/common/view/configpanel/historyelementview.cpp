/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#include "historyelementview.h"
#include "ui_historyelementview.h"
#include "helpers/settingsmanager.h"
#include "helpers/filehelper.h"
#include "appconfig.h"

#include <QFileInfo>
#include <QDebug>

HistoryElementView::HistoryElementView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryElementView)
{
    ui->setupUi(this);

    setProgressBarEnabled(false);
}

HistoryElementView::HistoryElementView(const QString &date, const QString &text, HistoryElementType type, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryElementView)
{
    ui->setupUi(this);

    _type = type;
    _text = text;
    _date = date;

    refresh();
}

HistoryElementView::~HistoryElementView()
{
    delete ui;
}

QString HistoryElementView::getText() const
{
    return _text;
}

bool HistoryElementView::isDownloading() const
{
    return ui->progressBar->isVisible();
}

void HistoryElementView::refresh()
{
    QString trucatedText = textForType(_type, _text);
    QIcon icon;

    switch (_type)
    {
    case HISTORY_FILE_FOLDER_TYPE:
        // File information
        ui->fileName->setText(trucatedText);
        ui->fileDate->setText(_date);
        ui->fileSize->setText(FileHelper::getFileSize(_text));
        ui->fileName->setWordWrap(false);
        // TODO
        ui->fileUserLabel->setText(trucateName(ui->fileUserLabel->text()));

        // File icon
        icon = QIcon(FILE_ICON);
        ui->fileIcon->setPixmap(icon.pixmap(16));
        break;

    case HISTORY_TEXT_TYPE:
        ui->text->setText(trucatedText);
        ui->textDate->setText(_date);
        ui->text->setWordWrap(true);
        // TODO
        ui->textUserLabel->setText(trucateName(ui->textUserLabel->text()));
        break;

    case HISTORY_URL_TYPE:
        ui->url->setText(trucatedText);
        ui->urlDate->setText(_date);
        ui->url->setWordWrap(true);
        // TODO
        ui->urlUserLabel->setText(trucateName(ui->urlUserLabel->text()));
        break;
    }

    ui->stackedWidget->setCurrentIndex(_type);

    setToolTip((_type == HISTORY_FILE_FOLDER_TYPE) ? _text : trucatedText);
    setProgressBarEnabled(false);
}

QString HistoryElementView::trucateName(QString fullDataName)
{
    int cutAt = 10;
    QString endingString = ".";
    QString trucatedName = fullDataName.left(cutAt - endingString.size());

    if (fullDataName.size() > trucatedName.size())
        trucatedName.append(endingString);

    return trucatedName;
}

QString HistoryElementView::textForType(HistoryElementType type, QString text)
{
    int cutAt = 25;
    QString endingString = " ...";
    int maxTextSize = (type == HISTORY_FILE_FOLDER_TYPE) ? (cutAt - endingString.size()) : 100;

    QString trucatedText = text.left(maxTextSize);

    if (type != HISTORY_FILE_FOLDER_TYPE)
    {
        int size = trucatedText.size();
        int i = 0;

        while (size > cutAt)
        {
            trucatedText.insert(++i * cutAt, " ");
            size -= cutAt;
        }
    }

    if (text.size() > maxTextSize)
        trucatedText.append(endingString);

    return trucatedText;

}

HistoryElementType HistoryElementView::getType() const
{
    return _type;
}

void HistoryElementView::setProgress(unsigned progress)
{
    if (_type == HISTORY_FILE_FOLDER_TYPE)
    {
        ui->fileSize->setText(FileHelper::getFileSize(_text));
        ui->progressBar->setValue(progress);
        ui->progressBar->setVisible(true);
        ui->cancelButton->setVisible(true);
    }
}

void HistoryElementView::setProgressBarEnabled(bool enabled)
{
    ui->progressBar->setValue(0);
    ui->progressBar->setVisible(enabled);
    ui->cancelButton->setVisible(enabled);
}

QString HistoryElementView::getFileSize() const
{
    return ui->fileSize->text();
}

void HistoryElementView::on_cancelButton_clicked()
{
    emit cancelIncomingTransfert();
}