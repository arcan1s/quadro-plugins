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

#include "pcstatussettings.h"
#include "ui_pcstatussettings.h"

#include <QSettings>

#include <quadrocore/quadrodebug.h>


PCStatusSettings::PCStatusSettings(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PCStatusSettings)
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    ui->setupUi(this);
}


PCStatusSettings::~PCStatusSettings()
{
    qCDebug(LOG_UI) << __PRETTY_FUNCTION__;

    delete ui;
}


QVariantHash PCStatusSettings::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QVariantHash configuration;

    configuration[QString("ShowCPU")] = settings.value(QString("ShowCPU"), true);
    configuration[QString("ShowMem")] = settings.value(QString("ShowMem"), true);
    configuration[QString("ShowNet")] = settings.value(QString("ShowNet"), true);
    configuration[QString("ShowSwap")] = settings.value(QString("ShowSwap"), true);
    configuration[QString("Update")] = settings.value(QString("Update"), 1000);

    init(configuration);
    return configuration;
}


bool PCStatusSettings::saveSettings(const QString configPath,
                                    const QVariantHash configuration) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;
    qCDebug(LOG_PL) << "Settings to save" << configuration;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("ShowCPU"), configuration[QString("ShowCPU")]);
    settings.setValue(QString("ShowMem"), configuration[QString("ShowMem")]);
    settings.setValue(QString("ShowNet"), configuration[QString("ShowNet")]);
    settings.setValue(QString("ShowSwap"), configuration[QString("ShowSwap")]);
    settings.setValue(QString("Update"), configuration[QString("Update")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void PCStatusSettings::init(const QVariantHash configuration)
{
    qCDebug(LOG_PL) << "Init UI with settings" << configuration;

    ui->checkBox_cpu->setChecked(configuration[QString("ShowCPU")].toBool());
    ui->checkBox_memory->setChecked(configuration[QString("ShowMem")].toBool());
    ui->checkBox_swap->setChecked(configuration[QString("ShowNet")].toBool());
    ui->checkBox_network->setChecked(configuration[QString("ShowSwap")].toBool());
    ui->spinBox_update->setValue(configuration[QString("Update")].toInt());
}
