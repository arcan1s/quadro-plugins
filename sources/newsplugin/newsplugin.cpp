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


#include "newsplugin.h"

#include "newspluginhelper.h"
#include "newspluginsettings.h"


NewsPlugin::~NewsPlugin()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
}


NewsPlugin *NewsPlugin::createInstance()
{
    return new NewsPlugin();
}


QString NewsPlugin::background() const
{
    return m_helper->news().image;
}


QWidget *NewsPlugin::configWidget()
{
    return m_config;
}


QString NewsPlugin::data() const
{
    return m_helper->news().text;
}


QString NewsPlugin::name() const
{
    return tr("News");
}


void NewsPlugin::action() const
{
    DBusOperations::sendRequestToUi(QString("RunWebContainer"),
                                    QVariantList() << m_helper->news().url << true);
}


void NewsPlugin::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    delete m_config;
    m_config = nullptr;
    m_config = new NewsPluginSettings(nullptr);

    m_configuration = m_config->readSettings(configPath);

    delete m_helper;
    m_helper = nullptr;
    m_helper = new NewsPluginHelper(this, m_configuration[QString("Provider")].toInt(),
                                    m_configuration[QString("Type")].toString(),
                                    m_configuration[QString("Retrieve")].toInt());
}


void NewsPlugin::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool NewsPlugin::writeSettings(const QString &configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


void NewsPlugin::update()
{
    m_helper->bump();
}


int NewsPlugin::updateInterval() const
{
    return m_configuration[QString("Update")].toInt();
}


QSize NewsPlugin::widgetSize() const
{
    return QSize(2, 1);
}
