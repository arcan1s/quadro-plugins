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


#include "pcstatushelper.h"

#include <QFile>
#include <QNetworkInterface>
#include <QTextCodec>

#include <quadrocore/quadrodebug.h>


PCStatusHelper::PCStatusHelper(QObject *parent)
    : QObject(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


PCStatusHelper::~PCStatusHelper()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


float PCStatusHelper::updateCPUStats()
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


float PCStatusHelper::updateDownStats(const QString device, const int interval)
{
    qCDebug(LOG_PL) << "Network device" << device << "with interval" << interval;

    float down = 0.0;
    QFile downStatsFile(QString("/sys/class/net/%1/statistics/rx_bytes").arg(device));
    if (downStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(downStatsFile.readAll()).trimmed();
        float currentDown = textData.toFloat();
        down = (currentDown - m_downStats) / (1024.0 * interval);
        m_downStats = currentDown;

        downStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/sys/class/net/%1/statistics/rx_bytes").arg(device);
    }

    return down;
}


float PCStatusHelper::updateMemoryStats()
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


QString PCStatusHelper::updateNetworkDevice()
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


float PCStatusHelper::updateSwapStats()
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


float PCStatusHelper::updateUpStats(const QString device, const int interval)
{
    qCDebug(LOG_PL) << "Network device" << device << "with interval" << interval;

    float up = 0.0;
    QFile upStatsFile(QString("/sys/class/net/%1/statistics/tx_bytes").arg(device));
    if (upStatsFile.open(QIODevice::ReadOnly)) {
        QString textData = QTextCodec::codecForMib(106)->toUnicode(upStatsFile.readAll()).trimmed();
        float currentUp = textData.toFloat();
        up = (currentUp - m_upStats) / (1024.0 * interval);
        m_downStats = currentUp;

        upStatsFile.close();
    } else {
        qCWarning(LOG_PL) << "Could not open" << QString("/sys/class/net/%1/statistics/tx_bytes").arg(device);
    }

    return up;
}
