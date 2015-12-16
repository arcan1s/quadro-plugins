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

#include "datetimesettings.h"
#include "ui_datetimesettings.h"

#include <QSettings>

#include <quadrocore/quadrodebug.h>


DateTimeSettings::DateTimeSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::DateTimeSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


DateTimeSettings::~DateTimeSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash DateTimeSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration[QString("Background")] = settings.value(QString("Background"), QString());
    configuration[QString("Format")] = settings.value(QString("Format"), QString("dddd\nd MMM yyyy\nt HH:mm"));

    init(configuration);
    return configuration;
}


QVariantHash DateTimeSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration[QString("Background")] = ui->lineEdit_background->text();
    configuration[QString("Format")] = ui->plainTextEdit_format->toPlainText();

    for (auto key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool DateTimeSettings::writeSettings(const QString configPath,
                                     const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("Background"), configuration[QString("Background")]);
    settings.setValue(QString("Format"), configuration[QString("Format")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void DateTimeSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_background->setText(configuration[QString("Background")].toString());
    ui->plainTextEdit_format->setPlainText(configuration[QString("Format")].toString());
}
