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


#include "yahooweather.h"

#include "yahooweatherhelper.h"
#include "yahooweathersettings.h"


YahooWeather::~YahooWeather()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
    delete m_helper;
}


QString YahooWeather::background() const
{
    // TODO
    return QString();
}


QWidget *YahooWeather::configWidget()
{
    return m_config;
}


QString YahooWeather::data() const
{
    // TODO
    return QString();
}


QString YahooWeather::name() const
{
    return tr("Weather");
}


void YahooWeather::init()
{
    m_config = new YahooWeatherSettings(nullptr);
}


QSize YahooWeather::minimalSize() const
{
    return QSize(1, 1);
}


void YahooWeather::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    m_configuration = m_config->readSettings(configPath);

    delete m_helper;
    m_helper = nullptr;
    m_helper = new YahooWeatherHelper(this,
                                      m_configuration[QString("City")].toString(),
                                      m_configuration[QString("Country")].toString());
}


void YahooWeather::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool YahooWeather::writeSettings(const QString configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


int YahooWeather::updateInterval() const
{
    return 60 * 1000;
}
