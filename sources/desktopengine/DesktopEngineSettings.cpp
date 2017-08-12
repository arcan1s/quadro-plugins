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

#include "DesktopEngineSettings.h"
#include "ui_DesktopEngineSettings.h"

#include <QSettings>

#include <quadrocore/Quadro.h>


DesktopEngineSettings::DesktopEngineSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DesktopEngineSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


DesktopEngineSettings::~DesktopEngineSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash DesktopEngineSettings::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    QVariantHash configuration;
    configuration["Action"] = settings.value("Action", "true");
    configuration["Background"] = settings.value("Background", "");
    configuration["Executable"] = settings.value("Executable", "true");
    configuration["Interval"] = settings.value("Interval", 1000);
    configuration["Height"] = settings.value("Background", 1);
    configuration["Width"] = settings.value("Background", 1);

    init(configuration);
    return configuration;
}


QVariantHash DesktopEngineSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration["Action"] = ui->lineEdit_action->text();
    configuration["Background"] = ui->lineEdit_background->text();
    configuration["Executable"] = ui->lineEdit_executable->text();
    configuration["Interval"] = ui->spinBox_interval->value();
    configuration["Height"] = ui->spinBox_height->value();
    configuration["Width"] = ui->spinBox_width->value();

    for (auto key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool DesktopEngineSettings::writeSettings(
    const QString &configPath, const QVariantHash &configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath << "settings to save"
                    << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue("Action", configuration["Action"]);
    settings.setValue("Background", configuration["Background"]);
    settings.setValue("Executable", configuration["Executable"]);
    settings.setValue("Interval", configuration["Interval"]);
    settings.setValue("Height", configuration["Height"]);
    settings.setValue("Width", configuration["Width"]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void DesktopEngineSettings::init(const QVariantHash &configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->lineEdit_action->setText(configuration["Action"].toString());
    ui->lineEdit_background->setText(configuration["Background"].toString());
    ui->lineEdit_executable->setText(configuration["Executable"].toString());
    ui->spinBox_interval->setValue(configuration["Interval"].toInt());
    ui->spinBox_height->setValue(configuration["Height"].toInt());
    ui->spinBox_width->setValue(configuration["Width"].toInt());
}
