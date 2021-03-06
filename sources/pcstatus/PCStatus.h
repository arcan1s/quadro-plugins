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


#ifndef PCSTATUS_H
#define PCSTATUS_H

#include <quadrocore/Quadro.h>


class PCStatusHelper;
class PCStatusSettings;

class PCStatus : public QObject, Quadro::PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.pcstatus/1.0")
    Q_INTERFACES(Quadro::PluginInterface)

public:
    virtual ~PCStatus();
    PCStatus *createInstance();
    QString background() const { return QString(); };
    QWidget *configWidget();
    QString data() const;
    QString name() const;
    void action() const {};
    void init();
    void quit(const QString &){};
    void readSettings(const QString &configPath);
    void saveSettings();
    bool writeSettings(const QString &configPath) const;
    void setArgs(Quadro::QuadroCore *){};
    void update();
    int updateInterval() const;
    QSize widgetSize() const;

private:
    QVariantHash m_configuration;
    QString m_stats;
    PCStatusHelper *m_helper = nullptr;
    PCStatusSettings *m_config = nullptr;
};


#endif /* PCSTATUS_H */
