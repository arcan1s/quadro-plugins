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


#include "YahooWeather.h"

#include "YahooWeatherHelper.h"
#include "YahooWeatherSettings.h"

using namespace Quadro;


YahooWeather::~YahooWeather()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
    delete m_helper;
}


YahooWeather *YahooWeather::createInstance()
{
    return new YahooWeather();
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


void YahooWeather::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    delete m_config;
    m_config = nullptr;
    m_config = new YahooWeatherSettings(nullptr);

    m_configuration = m_config->readSettings(configPath);

    delete m_helper;
    m_helper = nullptr;
    m_helper = new YahooWeatherHelper(this, m_configuration["City"].toString(),
                                      m_configuration["Country"].toString());
}


void YahooWeather::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool YahooWeather::writeSettings(const QString &configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


int YahooWeather::updateInterval() const
{
    return 60 * 1000;
}


QSize YahooWeather::widgetSize() const
{
    return QSize(2, 1);
}
