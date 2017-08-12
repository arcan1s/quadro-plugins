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

#include "WebPageSettings.h"
#include "ui_WebPageSettings.h"

#include <QSettings>
#include <QUuid>

#include <quadrocore/Quadro.h>


WebPageSettings::WebPageSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WebPageSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


WebPageSettings::~WebPageSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash WebPageSettings::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    // general settings
    configuration["Title"] = settings.value("Title", "None");
    configuration["Url"] = settings.value("Url", "about:blank");
    configuration["UseCache"] = settings.value("UseCache", true);

    configuration["Cache"] = settings.value(
        "Cache", generateCacheId(configuration["Title"].toString()));

    init(configuration);
    return configuration;
}


QVariantHash WebPageSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration["Title"] = ui->lineEdit_title->text();
    configuration["Url"] = ui->lineEdit_url->text();
    configuration["UseCache"] = ui->checkBox_cache->isChecked();

    for (auto &key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool WebPageSettings::writeSettings(const QString &configPath,
                                    const QVariantHash &configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath << "settings to save"
                    << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    // general settings
    settings.setValue("Cache", configuration["Cache"]);
    settings.setValue("Title", configuration["Title"]);
    settings.setValue("Url", configuration["Url"]);
    settings.setValue("UseCache", configuration["UseCache"]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


QString WebPageSettings::generateCacheId(const QString &title) const
{
    qCDebug(LOG_PL) << "Generate cache ID using title" << title;

    QString id = QString("quadro-%1-%2")
                     .arg(title)
                     .arg(QUuid::createUuid().toString());
    qCInfo(LOG_PL) << "Generated cache ID" << id;

    return id;
}


void WebPageSettings::init(const QVariantHash &configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_title->setText(configuration["Title"].toString());
    ui->lineEdit_url->setText(configuration["Url"].toString());
    ui->checkBox_cache->setChecked(configuration["UseCache"].toBool());
}
