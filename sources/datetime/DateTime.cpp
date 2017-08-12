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


#include "DateTime.h"

#include <QDateTime>

#include "DateTimeCalendar.h"
#include "DateTimeSettings.h"


DateTime::~DateTime()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
}


DateTime *DateTime::createInstance()
{
    return new DateTime();
}


QString DateTime::background() const
{
    return m_configuration["Background"].toString();
}


QWidget *DateTime::configWidget()
{
    return m_config;
}


QString DateTime::data() const
{
    return QDateTime::currentDateTime().toString(
        m_configuration["Format"].toString());
}


QString DateTime::name() const
{
    return tr("Date and time");
}


void DateTime::action() const
{
    DateTimeCalendar *calendar = new DateTimeCalendar(nullptr);
    int ret = calendar->exec();
    qCInfo(LOG_PL) << "Calendar exited with code" << ret;
    delete calendar;
}


void DateTime::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    delete m_config;
    m_config = nullptr;
    m_config = new DateTimeSettings(nullptr);

    m_configuration = m_config->readSettings(configPath);
}


void DateTime::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool DateTime::writeSettings(const QString &configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


int DateTime::updateInterval() const
{
    return 1000;
}


QSize DateTime::widgetSize() const
{
    return QSize(1, 1);
}
