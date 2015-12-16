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


#ifndef DATETIME_H
#define DATETIME_H

#include <quadrocore/quadro.h>


class DateTimeSettings;

class DateTime : public QObject, PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.datetime/1.0")
    Q_INTERFACES(PluginInterface)

public:
    virtual ~DateTime();
    QString background() const;
    QWidget *configWidget();
    QString data() const;
    QString name() const;
    void action() const;
    void init();
    QSize minimalSize() const;
    void quit(const QString) {};
    void readSettings(const QString configPath);
    void saveSettings();
    bool writeSettings(const QString configPath) const;
    void setArgs(QuadroCore *, const QVariantHash) {};
    void update() {};
    int updateInterval() const;

private:
    QVariantHash m_configuration;
    DateTimeSettings *m_config = nullptr;
};


#endif /* DATETIME_H */
