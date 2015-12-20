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


#include "webpage.h"

#include <QSettings>
#include <QUrl>
#include <QVBoxLayout>

#include <quadroui/quadroui.h>

#include "webpagesettings.h"


WebPage::~WebPage()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
    delete m_widget;
}


QWidget *WebPage::configWidget()
{
    return m_config;
}


QString WebPage::name() const
{
    return m_configuration[QString("Title")].toString();
}


QWidget *WebPage::widget()
{
    return this;
}


void WebPage::init()
{
    m_widget->setCache(m_configuration[QString("Cache")].toString());
    m_widget->loadUrl(QUrl::fromUserInput(m_configuration[QString("Url")].toString()));
}


void WebPage::quit(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.setValue(QString("Cache"), m_configuration[QString("Cache")]);

    settings.sync();
}


void WebPage::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    m_configuration = m_config->readSettings(configPath);
}


void WebPage::saveSettings()
{
    QVariantHash config = m_config->saveSettings();
    for (auto key : config.keys())
        m_configuration[key] = config[key];
}


bool WebPage::writeSettings(const QString configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


void WebPage::setArgs(QuadroCore *, const QVariantHash settings)
{
    qCDebug(LOG_PL) << "Application settings" << settings;

    m_appConfiguration = settings;
    m_config = new WebPageSettings();

    // ui
    QWidget *widget = new QWidget(this);
    m_widget = new WebAppWidget(this, -1, false);
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(m_widget);
    widget->setLayout(layout);

    setCentralWidget(widget);
}
