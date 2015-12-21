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

#include "webpagesettings.h"
#include "ui_webpagesettings.h"

#include <QSettings>
#include <QUuid>

#include <quadrocore/quadrodebug.h>


WebPageSettings::WebPageSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::WebPageSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


WebPageSettings::~WebPageSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash WebPageSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    // general settings
    configuration[QString("Title")] = settings.value(QString("Title"), QString("None"));
    configuration[QString("Url")] = settings.value(QString("Url"), QString("about:blank"));
    configuration[QString("UseCache")] = settings.value(QString("UseCache"), true);

    configuration[QString("Cache")] = settings.value(
        QString("Cache"), generateCacheId(configuration[QString("Title")].toString()));

    init(configuration);
    return configuration;
}


QVariantHash WebPageSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration[QString("Title")] = ui->lineEdit_title->text();
    configuration[QString("Url")] = ui->lineEdit_url->text();
    configuration[QString("UseCache")] = ui->checkBox_cache->isChecked();

    for (auto key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool WebPageSettings::writeSettings(const QString configPath,
                                        const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // general settings
    settings.setValue(QString("Cache"), configuration[QString("Cache")]);
    settings.setValue(QString("Title"), configuration[QString("Title")]);
    settings.setValue(QString("Url"), configuration[QString("Url")]);
    settings.setValue(QString("UseCache"), configuration[QString("UseCache")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


QString WebPageSettings::generateCacheId(const QString title) const
{
    qCDebug(LOG_PL) << "Generate cache ID using title" << title;

    QString id = QString("quadro-%1-%2").arg(title).arg(QUuid::createUuid().toString());
    qCInfo(LOG_PL) << "Generated cache ID" << id;

    return id;
}


void WebPageSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_title->setText(configuration[QString("Title")].toString());
    ui->lineEdit_url->setText(configuration[QString("Url")].toString());
    ui->checkBox_cache->setChecked(configuration[QString("UseCache")].toBool());
}
