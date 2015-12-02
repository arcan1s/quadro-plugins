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

#include "desktopenginesettings.h"
#include "ui_desktopenginesettings.h"

#include <QSettings>

#include <quadrocore/quadrodebug.h>


DesktopEngineSettings::DesktopEngineSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::DesktopEngineSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


DesktopEngineSettings::~DesktopEngineSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash DesktopEngineSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration[QString("Action")] = settings.value(QString("Action"), QString("true"));
    configuration[QString("Background")] = settings.value(QString("Background"), QString());
    configuration[QString("Executable")] = settings.value(QString("Executable"), QString("true"));
    configuration[QString("Interval")] = settings.value(QString("Interval"), 1000);
    configuration[QString("Height")] = settings.value(QString("Background"), 1);
    configuration[QString("Width")] = settings.value(QString("Background"), 1);

    init(configuration);
    return configuration;
}


bool DesktopEngineSettings::saveSettings(const QString configPath,
                                         const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("Action"), configuration[QString("Action")]);
    settings.setValue(QString("Background"), configuration[QString("Background")]);
    settings.setValue(QString("Executable"), configuration[QString("Executable")]);
    settings.setValue(QString("Interval"), configuration[QString("Interval")]);
    settings.setValue(QString("Height"), configuration[QString("Height")]);
    settings.setValue(QString("Width"), configuration[QString("Width")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void DesktopEngineSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_action->setText(configuration[QString("Action")].toString());
    ui->lineEdit_background->setText(configuration[QString("Background")].toString());
    ui->lineEdit_executable->setText(configuration[QString("Executable")].toString());
    ui->spinBox_interval->setValue(configuration[QString("Interval")].toInt());
    ui->spinBox_height->setValue(configuration[QString("Height")].toInt());
    ui->spinBox_width->setValue(configuration[QString("Width")].toInt());
}
