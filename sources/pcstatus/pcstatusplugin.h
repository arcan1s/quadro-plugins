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


#ifndef PCSTATUSPLUGIN_H
#define PCSTATUSPLUGIN_H

#include <QMainWindow>

#include <quadrocore/quadro.h>


class PCStatusPlugin : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.pcstatus/1.0")
    Q_INTERFACES(PluginInterface)

public:
    virtual ~PCStatusPlugin();
    QString background() const { return QString(); };
    QWidget *configWidget();
    QString data() const;
    QString name() const;
    void action() const {};
    void init() {};
    void quit() {};
    void readSettings(const QString configPath);
    bool saveSettings(const QString configPath);
    void update();
    int updateInterval() const;

private:
    QVariantHash m_configuration;
    struct {
        float user = 0.0;
        float system = 0.0;
        float idle = 0.0;
    } m_cpuStats;
    float m_downStats = 0.0;
    QString m_networkDevice;
    QString m_stats;
    float m_upStats = 0.0;
    // methods
    float updateCPUStats();
    float updateDownStats();
    float updateMemoryStats();
    QString updateNetworkDevice();
    float updateSwapStats();
    float updateUpStats();
};


#endif /* PCSTATUSPLUGIN_H */
