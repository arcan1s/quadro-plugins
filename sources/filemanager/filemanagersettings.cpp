/***************************************************************************
 *   This file is part of quadro                                           *
 *                                                                         *
 *   quadro is free software: you can redistribute it and/or               *
 *   modify it under the terms of the GNU General Public License as        *
 *   published by the Free Software Foundation, either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   quadro is distributed in the hope that it will be useful,             *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with quadro. If not, see http://www.gnu.org/licenses/           *
 ***************************************************************************/

#include "filemanagersettings.h"
#include "ui_filemanagersettings.h"

#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>

#include <quadrocore/quadrodebug.h>


FileManagerSettings::FileManagerSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::FileManagerSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);

    connect(ui->lineEdit_tabs, SIGNAL(returnPressed()), this, SLOT(addEntry()));
    connect(ui->pushButton_addTab, SIGNAL(clicked(const bool)), this, SLOT(addEntry()));
    connect(ui->pushButton_browseTab, SIGNAL(clicked(const bool)), this, SLOT(selectEntry()));
}


FileManagerSettings::~FileManagerSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash FileManagerSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    // general settings
    configuration[QString("SearchResult")] = settings.value(QString("SearchResult"), 20);
    configuration[QString("ShowHidden")] = settings.value(QString("ShowHidden"), false);
    configuration[QString("ShowToolbar")] = settings.value(QString("ShowToolbar"), false);

    // tab settings
    configuration[QString("Tabs")] = settings.value(QString("Tabs"),
                                                    QStringList() << QDir::homePath());

    init(configuration);
    return configuration;
}


bool FileManagerSettings::saveSettings(const QString configPath,
                                       const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // general settings
    settings.setValue(QString("SearchResult"), configuration[QString("SearchResult")]);
    settings.setValue(QString("ShowHidden"), configuration[QString("ShowHidden")]);
    settings.setValue(QString("ShowToolbar"), configuration[QString("ShowToolbar")]);

    // tab settings
    settings.setValue(QString("Tabs"), configuration[QString("Tabs")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void FileManagerSettings::keyPressEvent(QKeyEvent *pressedKey)
{
    if (pressedKey->key() == Qt::Key_Delete) {
        if (ui->listWidget_tabs->hasFocus()
            && (ui->listWidget_tabs->currentItem() != nullptr)) {
            delete ui->listWidget_tabs->currentItem();
        }
    }
}


void FileManagerSettings::addEntry()
{
    QFileInfo entry = QFileInfo(ui->lineEdit_tabs->text());
    qCInfo(LOG_PL) << "Looking for" << entry.absoluteFilePath();

    if ((entry.exists())
        && (QFileInfo(entry).isDir())) {
        ui->listWidget_tabs->addItem(entry.absoluteFilePath());
    } else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Could not find %1").arg(entry.absoluteFilePath()));
    }
}


void FileManagerSettings::selectEntry()
{
    QString entry = QFileDialog::getExistingDirectory(this,
                                                      tr("Select directory"),
                                                      QDir::homePath());

    if (!entry.isEmpty())
        ui->lineEdit_tabs->setText(entry);
}


void FileManagerSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->spinBox_searchCount->setValue(configuration[QString("SearchResult")].toInt());
    ui->checkBox_hidden->setChecked(configuration[QString("ShowHidden")].toBool());
    ui->checkBox_toolbar->setChecked(configuration[QString("ShowToolbar")].toBool());

    ui->listWidget_tabs->clear();
    for (auto tab : configuration[QString("Tabs")].toStringList())
        ui->listWidget_tabs->addItem(tab);
}
