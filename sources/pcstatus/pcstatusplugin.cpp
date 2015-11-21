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


#include "pcstatusplugin.h"

#include <QNetworkInterface>
#include <QSettings>
#include <QTextCodec>


PCStatusPlugin::~PCStatusPlugin()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QWidget *PCStatusPlugin::configWidget()
{
    return nullptr;
}


QString PCStatusPlugin::data() const
{
    return m_stats;
}


QString PCStatusPlugin::name() const
{
    return tr("PC status");
}


void PCStatusPlugin::readSettings(const QString configPath)
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


bool PCStatusPlugin::saveSettings(const QString configPath)
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


void PCStatusPlugin::update()
{
    // cpu utilization
    float cpu = updateCPUStats();
    // memory
    float mem = updateMemoryStats();
    // swap
    float swap = updateSwapStats();
    // network
    m_networkDevice = updateNetworkDevice();
    float down = updateDownStats();
    float up = updateUpStats();

    m_stats = tr("CPU: %1").arg(cpu, 5, 'f', 1);
}


int PCStatusPlugin::updateInterval() const
{
    return m_configuration[QString("Update")].toInt();
}


float PCStatusPlugin::updateCPUStats()
{
    float currentUser = 0.0;
    float currentSystem = 0.0;
    float currentIdle = 0.0;

    QFile cpuStatsFile(QString("/proc/stat"));
    if (cpuStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(cpuStatsFile.readAll()).trimmed();
        QStringList totalCpuValues = textData.split(QChar('\n')).at(0).split(QChar(' '));

        // some magic here. See man proc(5) for more details
        if (totalCpuValues.count() >= 5) {
            // user
            float user = totalCpuValues.at(1).toFloat();
            currentUser = user - m_cpuStats.user;
            m_cpuStats.user = user;
            // system
            float system = totalCpuValues.at(3).toFloat();
            currentSystem = system - m_cpuStats.system;
            m_cpuStats.system = system;
            // idle
            float idle = totalCpuValues.at(4).toFloat();
            currentIdle = idle - m_cpuStats.idle;
            m_cpuStats.idle = idle;
        }
        cpuStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/proc/stat");
    }

    // return current value
    return 100.0 * (currentSystem + currentUser) / (currentSystem + currentUser + currentIdle);
}


float PCStatusPlugin::updateDownStats()
{
    float down = 0.0;

    QFile downStatsFile(QString("/sys/class/net/%1/statistics/rx_bytes").arg(m_networkDevice));
    if (downStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(downStatsFile.readAll()).trimmed();
        float currentDown = textData.toFloat();
        down = (currentDown - m_downStats) / (1024.0 * m_configuration[QString("Update")].toFloat());
        m_downStats = currentDown;

        downStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/sys/class/net/%1/statistics/rx_bytes").arg(m_networkDevice);
    }

    return down;
}


float PCStatusPlugin::updateMemoryStats()
{
    float total = 0.0;
    float free = 0.0;
    float buffers = 0.0;
    float cached = 0.0;

    QFile memStatsFile(QString("/proc/meminfo"));
    if (memStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(memStatsFile.readAll()).trimmed();
        QStringList memValues = textData.split(QChar('\n'));
        for (auto data : memValues) {
            if (data.startsWith(QString("MemTotal:")))
                total = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
            else if (data.startsWith(QString("MemFree:")))
                free = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
            else if (data.startsWith(QString("Buffers:")))
                buffers = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
            else if (data.startsWith(QString("Cached:")))
                cached = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
        }

        memStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/proc/meminfo");
    }

    return 100.0 * (total - free - buffers - cached) / total;
}


QString PCStatusPlugin::updateNetworkDevice()
{
    QString device = QString("lo");
    QList<QNetworkInterface> rawInterfaceList = QNetworkInterface::allInterfaces();
    qCInfo(LOG_PL) << "Devices" << rawInterfaceList;
    for (auto interface : rawInterfaceList) {
        if ((interface.flags().testFlag(QNetworkInterface::IsLoopBack))
            || (interface.flags().testFlag(QNetworkInterface::IsPointToPoint)))
            continue;
        if (interface.addressEntries().isEmpty())
            continue;
        device = interface.name();
        break;
    }

    return device;
}


float PCStatusPlugin::updateSwapStats()
{
    float total = 0.0;
    float free = 0.0;

    QFile memStatsFile(QString("/proc/meminfo"));
    if (memStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(memStatsFile.readAll()).trimmed();
        QStringList memValues = textData.split(QChar('\n'));
        for (auto data : memValues) {
            if (data.startsWith(QString("SwapTotal:")))
                total = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
            else if (data.startsWith(QString("SwapFree:")))
                free = data.split(QChar(' '), QString::SkipEmptyParts).at(1).toFloat();
        }

        memStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/proc/meminfo");
    }

    return 100.0 * (total - free) / total;
}


float PCStatusPlugin::updateUpStats()
{
    float up = 0.0;

    QFile upStatsFile(QString("/sys/class/net/%1/statistics/tx_bytes").arg(m_networkDevice));
    if (upStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(upStatsFile.readAll()).trimmed();
        float currentUp = textData.toFloat();
        up = (currentUp - m_upStats) / (1024.0 * m_configuration[QString("Update")].toFloat());
        m_downStats = currentUp;

        upStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/sys/class/net/%1/statistics/tx_bytes").arg(m_networkDevice);
    }

    return up;
}
