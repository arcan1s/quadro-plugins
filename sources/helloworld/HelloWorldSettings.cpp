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

#include "HelloWorldSettings.h"
#include "ui_HelloWorldSettings.h"

#include <QSettings>

#include <quadrocore/Quadro.h>

using namespace Quadro;


HelloWorldSettings::HelloWorldSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HelloWorldSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


HelloWorldSettings::~HelloWorldSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash HelloWorldSettings::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration["Background"] = settings.value("Background", "");
    configuration["Text"] = settings.value("Text", "Hello world");

    init(configuration);
    return configuration;
}


QVariantHash HelloWorldSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration["Background"] = ui->lineEdit_background->text();
    configuration["Text"] = ui->lineEdit_text->text();

    for (auto &key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool HelloWorldSettings::writeSettings(const QString &configPath,
                                       const QVariantHash &configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath << "settings to save"
                    << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue("Background", configuration["Background"]);
    settings.setValue("Text", configuration["Text"]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void HelloWorldSettings::init(const QVariantHash &configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_background->setText(configuration["Background"].toString());
    ui->lineEdit_text->setText(configuration["Text"].toString());
}
