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

#include "PCStatusSettings.h"
#include "ui_PCStatusSettings.h"

#include <QSettings>

#include <quadrocore/Quadro.h>


PCStatusSettings::PCStatusSettings(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PCStatusSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


PCStatusSettings::~PCStatusSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash PCStatusSettings::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QVariantHash configuration;

    configuration["ShowCPU"] = settings.value("ShowCPU", true);
    configuration["ShowMem"] = settings.value("ShowMem", true);
    configuration["ShowNet"] = settings.value("ShowNet", true);
    configuration["ShowSwap"] = settings.value("ShowSwap", true);
    configuration["Update"] = settings.value("Update", 1000);

    init(configuration);
    return configuration;
}


QVariantHash PCStatusSettings::saveSettings() const
{
    QVariantHash configuration;
    configuration["ShowCPU"] = ui->checkBox_cpu->isChecked();
    configuration["ShowMem"] = ui->checkBox_memory->isChecked();
    configuration["ShowNet"] = ui->checkBox_swap->isChecked();
    configuration["ShowSwap"] = ui->checkBox_network->isChecked();
    configuration["Update"] = ui->spinBox_update->value();

    for (auto &key : configuration.keys())
        qCInfo(LOG_PL) << key << "=" << configuration[key];
    return configuration;
}


bool PCStatusSettings::writeSettings(const QString &configPath,
                                     const QVariantHash &configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath << "settings to save"
                    << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue("ShowCPU", configuration["ShowCPU"]);
    settings.setValue("ShowMem", configuration["ShowMem"]);
    settings.setValue("ShowNet", configuration["ShowNet"]);
    settings.setValue("ShowSwap", configuration["ShowSwap"]);
    settings.setValue("Update", configuration["Update"]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void PCStatusSettings::init(const QVariantHash &configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->checkBox_cpu->setChecked(configuration["ShowCPU"].toBool());
    ui->checkBox_memory->setChecked(configuration["ShowMem"].toBool());
    ui->checkBox_swap->setChecked(configuration["ShowNet"].toBool());
    ui->checkBox_network->setChecked(configuration["ShowSwap"].toBool());
    ui->spinBox_update->setValue(configuration["Update"].toInt());
}
