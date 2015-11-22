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


#ifndef PCSTATUSHELPER_H
#define PCSTATUSHELPER_H

#include <QObject>


class PCStatusHelper : public QObject
{
    Q_OBJECT

public:
    explicit PCStatusHelper(QObject *parent);
    virtual ~PCStatusHelper();
    float updateCPUStats();
    float updateDownStats(const QString device, const int interval);
    float updateMemoryStats();
    QString updateNetworkDevice();
    float updateSwapStats();
    float updateUpStats(const QString device, const int interval);

private:
    struct {
        float user = 0.0;
        float system = 0.0;
        float idle = 0.0;
    } m_cpuStats;
    float m_downStats = 0.0;
    float m_upStats = 0.0;
};


#endif /* PCSTATUSHELPER_H */
