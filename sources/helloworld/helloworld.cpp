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


#include "helloworld.h"

#include <QMessageBox>

#include "helloworldsettings.h"


HelloWorld::~HelloWorld()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
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


void HelloWorld::init()
{
    m_config = new HelloWorldSettings(nullptr);
}


void HelloWorld::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    m_configuration = m_config->readSettings(configPath);
}


bool HelloWorld::saveSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->saveSettings(configPath, m_configuration);
}


int HelloWorld::updateInterval() const
{
    return 0;
}
