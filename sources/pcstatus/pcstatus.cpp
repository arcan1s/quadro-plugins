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


#include "pcstatus.h"

#include <QSettings>

#include "pcstatushelper.h"


PCStatus::~PCStatus()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_helper;
}


QWidget *PCStatus::configWidget()
{
    return nullptr;
}


QString PCStatus::data() const
{
    return m_stats;
}


QString PCStatus::name() const
{
    return tr("PC status");
}


void PCStatus::init()
{
    m_helper = new PCStatusHelper(this);
}


void PCStatus::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    m_configuration[QString("ShowCPU")] = settings.value(QString("ShowCPU"), true);
    m_configuration[QString("ShowMem")] = settings.value(QString("ShowMem"), true);
    m_configuration[QString("ShowNet")] = settings.value(QString("ShowNet"), true);
    m_configuration[QString("ShowSwap")] = settings.value(QString("ShowSwap"), true);
    m_configuration[QString("Update")] = settings.value(QString("Update"), 1000);
}


bool PCStatus::saveSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("ShowCPU"), m_configuration[QString("ShowCPU")]);
    settings.setValue(QString("ShowMem"), m_configuration[QString("ShowMem")]);
    settings.setValue(QString("ShowNet"), m_configuration[QString("ShowNet")]);
    settings.setValue(QString("ShowSwap"), m_configuration[QString("ShowSwap")]);
    settings.setValue(QString("Update"), m_configuration[QString("Update")]);

    settings.sync();

    return settings.status() == QSettings::NoError;
}


void PCStatus::update()
{
    // cpu utilization
    float cpu = m_helper->updateCPUStats();
    // memory
    float mem = m_helper->updateMemoryStats();
    // swap
    float swap = m_helper->updateSwapStats();
    // network
    QString networkDevice = m_helper->updateNetworkDevice();
    float down = m_helper->updateDownStats(networkDevice, m_configuration[QString("Update")].toInt());
    float up = m_helper->updateUpStats(networkDevice, m_configuration[QString("Update")].toInt());

    QStringList dataStats;
    if (m_configuration[QString("ShowCPU")].toBool())
        dataStats.append(tr("CPU: %1").arg(cpu, 5, 'f', 1));
    if (m_configuration[QString("ShowMem")].toBool())
        dataStats.append(tr("Memory: %1").arg(mem, 5, 'f', 1));
    if (m_configuration[QString("ShowSwap")].toBool())
        dataStats.append(tr("Swap: %1").arg(swap, 5, 'f', 1));
    if (m_configuration[QString("ShowNet")].toBool())
        dataStats.append(tr("Network speed: %1/%2").arg(down, 4, 'f', 0).arg(up, 4, 'f', 0));
    m_stats = dataStats.join(QChar('\n'));
}


int PCStatus::updateInterval() const
{
    return m_configuration[QString("Update")].toInt();
}
