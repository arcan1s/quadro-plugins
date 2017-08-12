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

#include "FileManagerSettings.h"
#include "ui_FileManagerSettings.h"

#include <QDir>
#include <QFileDialog>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSettings>

#include <quadrocore/Quadro.h>


FileManagerSettings::FileManagerSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FileManagerSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);

    connect(ui->lineEdit_tabs, SIGNAL(returnPressed()), this, SLOT(addEntry()));
    connect(ui->pushButton_addTab, SIGNAL(clicked(const bool)), this,
            SLOT(addEntry()));
    connect(ui->pushButton_browseTab, SIGNAL(clicked(const bool)), this,
            SLOT(selectEntry()));
}


FileManagerSettings::~FileManagerSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash FileManagerSettings::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    // general settings
    configuration["SearchResult"] = settings.value("SearchResult", 20);
    configuration["ShowHidden"] = settings.value("ShowHidden", false);
    configuration["ShowToolbar"] = settings.value("ShowToolbar", false);

    // tab settings
    configuration["Tabs"]
        = settings.value("Tabs", QStringList() << QDir::homePath());

    settings.beginGroup("UI");
    configuration["BarPosition"]
        = settings.value("BarPosition", Qt::TopToolBarArea);
    settings.endGroup();

    init(configuration);
    return configuration;
}


QVariantHash FileManagerSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration["SearchResult"] = ui->spinBox_searchCount->value();
    configuration["ShowHidden"] = ui->checkBox_hidden->isChecked();
    configuration["ShowToolbar"] = ui->checkBox_toolbar->isChecked();
    QList<QListWidgetItem *> items = ui->listWidget_tabs->findItems(
        QString("*"), Qt::MatchWrap | Qt::MatchWildcard);
    QStringList tabs;
    for (auto item : items)
        tabs.append(item->text());
    configuration["Tabs"] = tabs;

    for (auto &key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool FileManagerSettings::writeSettings(const QString &configPath,
                                        const QVariantHash &configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath << "settings to save"
                    << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // general settings
    settings.setValue("SearchResult", configuration["SearchResult"]);
    settings.setValue("ShowHidden", configuration["ShowHidden"]);
    settings.setValue("ShowToolbar", configuration["ShowToolbar"]);

    // tab settings
    settings.setValue("Tabs", configuration["Tabs"]);

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

    if ((entry.exists()) && (QFileInfo(entry).isDir())) {
        ui->listWidget_tabs->addItem(entry.absoluteFilePath());
    } else {
        QMessageBox::critical(
            this, tr("Error"),
            tr("Could not find %1").arg(entry.absoluteFilePath()));
    }
}


void FileManagerSettings::selectEntry()
{
    QString entry = QFileDialog::getExistingDirectory(
        this, tr("Select directory"), QDir::homePath());

    if (!entry.isEmpty())
        ui->lineEdit_tabs->setText(entry);
}


void FileManagerSettings::init(const QVariantHash &configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->spinBox_searchCount->setValue(configuration["SearchResult"].toInt());
    ui->checkBox_hidden->setChecked(configuration["ShowHidden"].toBool());
    ui->checkBox_toolbar->setChecked(configuration["ShowToolbar"].toBool());

    ui->listWidget_tabs->clear();
    for (auto &tab : configuration["Tabs"].toStringList())
        ui->listWidget_tabs->addItem(tab);
}
