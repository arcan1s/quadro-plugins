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


#include "NewsPluginProvider.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <quadrocore/quadrodebug.h>


NewsPluginProvider::NewsPluginProvider(QObject *parent, const QString &type)
    : QObject(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(replyReceived(QNetworkReply *)));
}


NewsPluginProvider::~NewsPluginProvider()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    disconnect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
               SLOT(replyReceived(QNetworkReply *)));

    m_manager->deleteLater();
}


void NewsPluginProvider::retrieve()
{
    m_manager->get(QNetworkRequest(QUrl(url())));
}
