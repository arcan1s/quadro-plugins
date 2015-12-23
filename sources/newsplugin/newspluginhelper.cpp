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


#include "newspluginhelper.h"

#include <QTimer>

#include <quadrocore/quadrodebug.h>


NewsPluginHelper::NewsPluginHelper(QObject *parent, const int provider,
                                   const int interval)
    : QObject(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    // TODO init provider according to index
    m_timer = new QTimer(this);
    m_timer->setInterval(1000 * interval);
    m_timer->setSingleShot(false);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}


NewsPluginHelper::~NewsPluginHelper()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    delete m_provider;
    delete m_timer;
}


int NewsPluginHelper::bump()
{
    if (m_metadata.isEmpty())
        return -1;

    if (m_current == m_metadata.count() - 1)
        m_current = 0;
    else
        m_current++;

    return m_current;
}


NewsPluginMetadata NewsPluginHelper::news() const
{
    return m_current == -1 ? NewsPluginMetadata() : m_metadata[m_current];
}


void NewsPluginHelper::update()
{
    m_metadata = m_provider->retrieve();
}
