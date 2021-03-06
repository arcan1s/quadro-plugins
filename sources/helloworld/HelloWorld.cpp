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


#include "HelloWorld.h"

#include <QMessageBox>

#include "HelloWorldSettings.h"


HelloWorld::~HelloWorld()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
}


HelloWorld *HelloWorld::createInstance()
{
    return new HelloWorld();
}


QString HelloWorld::background() const
{
    return m_configuration[QString("Background")].toString();
}


QWidget *HelloWorld::configWidget()
{
    return m_config;
}


QString HelloWorld::data() const
{
    return m_configuration[QString("Text")].toString();
}


QString HelloWorld::name() const
{
    return tr("Hello world");
}


void HelloWorld::action() const
{
    QMessageBox::information(nullptr, tr("Hello"), tr("Hello world!"));
}


void HelloWorld::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    delete m_config;
    m_config = nullptr;
    m_config = new HelloWorldSettings(nullptr);

    m_configuration = m_config->readSettings(configPath);
}


void HelloWorld::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool HelloWorld::writeSettings(const QString &configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


int HelloWorld::updateInterval() const
{
    return 0;
}


QSize HelloWorld::widgetSize() const
{
    return QSize(1, 1);
}
