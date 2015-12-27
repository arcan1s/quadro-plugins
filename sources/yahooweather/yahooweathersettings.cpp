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

#include "yahooweathersettings.h"
#include "ui_yahooweathersettings.h"

#include <QSettings>

#include <quadrocore/quadrodebug.h>


YahooWeatherSettings::YahooWeatherSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::YahooWeatherSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


YahooWeatherSettings::~YahooWeatherSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash YahooWeatherSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration[QString("City")] = settings.value(QString("City"), QString("London"));
    configuration[QString("Country")] = settings.value(QString("Country"), QString("uk"));

    init(configuration);
    return configuration;
}


QVariantHash YahooWeatherSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration[QString("City")] = ui->lineEdit_city->text();
    configuration[QString("Country")] = ui->lineEdit_country->text();

    for (auto key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool YahooWeatherSettings::writeSettings(const QString configPath,
                                         const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("City"), configuration[QString("City")]);
    settings.setValue(QString("Country"), configuration[QString("Country")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void YahooWeatherSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_city->setText(configuration[QString("City")].toString());
    ui->lineEdit_country->setText(configuration[QString("Country")].toString());
}
