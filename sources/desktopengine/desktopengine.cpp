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


#include "desktopengine.h"

#include <QProcess>
#include <QTextCodec>

#include "desktopenginesettings.h"


DesktopEngine::~DesktopEngine()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    delete m_config;
    if (m_process != nullptr) {
        m_process->kill();
        m_process->deleteLater();
    }
}


DesktopEngine *DesktopEngine::createInstance()
{
    return new DesktopEngine();
}


QString DesktopEngine::background() const
{
    return m_configuration[QString("Background")].toString();
}


QWidget *DesktopEngine::configWidget()
{
    return m_config;
}


QString DesktopEngine::data() const
{
    return m_data;
}


QString DesktopEngine::name() const
{
    return tr("Desktop engine");
}

void DesktopEngine::action() const
{
    QProcess::startDetached(m_configuration[QString("Action")].toString());
}


void DesktopEngine::init()
{
    m_process = new QProcess(nullptr);
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this,
            SLOT(updateData(int, QProcess::ExitStatus)));
    m_process->waitForFinished(0);
}


void DesktopEngine::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    delete m_config;
    m_config = nullptr;
    m_config = new DesktopEngineSettings(nullptr);

    m_configuration = m_config->readSettings(configPath);
}


void DesktopEngine::saveSettings()
{
    m_configuration = m_config->saveSettings();
}


bool DesktopEngine::writeSettings(const QString &configPath) const
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->writeSettings(configPath, m_configuration);
}


void DesktopEngine::update()
{
    if (m_process->state() != QProcess::NotRunning)
        return;

    m_process->start(m_configuration[QString("Executable")].toString());
}


int DesktopEngine::updateInterval() const
{
    return m_configuration[QString("Interval")].toInt();
}


void DesktopEngine::updateData(int exitCode, QProcess::ExitStatus exitStatus)
{
    qCDebug(LOG_PL) << "Exit code" << exitCode << "with status" << exitStatus;

    m_data = QTextCodec::codecForMib(106)->toUnicode(
        m_process->readAllStandardError()).trimmed();
}


QSize DesktopEngine::widgetSize() const
{
    return QSize(m_configuration[QString("Width")].toInt(),
                 m_configuration[QString("Height")].toInt());
}
