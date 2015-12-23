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

#include "newspluginsettings.h"
#include "ui_newspluginsettings.h"

#include <QSettings>

#include <quadrocore/quadrodebug.h>


NewsPluginSettings::NewsPluginSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::NewsPluginSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


NewsPluginSettings::~NewsPluginSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash NewsPluginSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration[QString("Provider")] = settings.value(QString("Provider"), 0);
    configuration[QString("Retrieve")] = settings.value(QString("Retrieve"), 3600);
    configuration[QString("Update")] = settings.value(QString("Update"), 60);

    init(configuration);
    return configuration;
}


QVariantHash NewsPluginSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration[QString("Provider")] = ui->comboBox_provider->currentIndex();
    configuration[QString("Retrieve")] = ui->spinBox_retrieve->value();
    configuration[QString("Update")] = ui->spinBox_update->value();

    for (auto key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool NewsPluginSettings::writeSettings(const QString configPath,
                                       const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("Provider"), configuration[QString("Provider")]);
    settings.setValue(QString("Retrieve"), configuration[QString("Retrieve")]);
    settings.setValue(QString("Update"), configuration[QString("Update")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void NewsPluginSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->comboBox_provider->setCurrentIndex(configuration[QString("Provider")].toInt());
    ui->spinBox_retrieve->setValue(configuration[QString("Retrieve")].toInt());
    ui->spinBox_update->setValue(configuration[QString("Update")].toInt());
}
