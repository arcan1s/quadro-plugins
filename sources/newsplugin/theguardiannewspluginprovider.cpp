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


#include "theguardiannewspluginprovider.h"
#include "newspluginprovider.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

#include <quadrocore/quadrodebug.h>


TheGuardianNewsPluginProvider::TheGuardianNewsPluginProvider(QObject *parent)
    : NewsPluginProvider(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(replyReceived(QNetworkReply *)));
}


TheGuardianNewsPluginProvider::~TheGuardianNewsPluginProvider()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    disconnect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
               SLOT(replyReceived(QNetworkReply *)));

    m_manager->deleteLater();
}


QList<NewsPluginMetadata> TheGuardianNewsPluginProvider::retrieve()
{
    m_manager->get(QNetworkRequest(QUrl(QString(GUARDIAN_API_URL))));

    return m_metadata;
}


void TheGuardianNewsPluginProvider::replyReceived(QNetworkReply *reply)
{
    qCDebug(LOG_PL) << "Return code" << reply->error();
    qCDebug(LOG_PL) << "Reply error message" << reply->errorString();

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll(), &error);
    reply->deleteLater();
    if ((reply->error() != QNetworkReply::NoError)
        || (error.error != QJsonParseError::NoError)) {
        qCWarning(LOG_LIB) << "Parse error" << error.errorString();
        return;
    }

    QVariantList jsonNews = jsonDoc.toVariant().toMap()[QString("response")].toMap()[QString("results")].toList();
    m_metadata.clear();

    for (auto news : jsonNews) {
        NewsPluginMetadata singleData;
        singleData.image = QString();
        singleData.text = news.toMap()[QString("webTitle")].toString();
        singleData.url = news.toMap()[QString("webUrl")].toString();
        m_metadata.append(singleData);
    }
}
