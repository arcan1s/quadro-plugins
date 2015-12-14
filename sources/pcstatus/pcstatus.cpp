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

#include "pcstatushelper.h"
#include "pcstatussettings.h"


PCStatus::~PCStatus()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
    delete m_helper;
}


QWidget *PCStatus::configWidget()
{
    return m_config;
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
    m_config = new PCStatusSettings(nullptr);
    m_helper = new PCStatusHelper(this);
}


QSize PCStatus::minimalSize() const
{
    return QSize(1, 1);
}


void PCStatus::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    m_configuration = m_config->readSettings(configPath);
}


void PCStatus::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool PCStatus::writeSettings(const QString configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
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
