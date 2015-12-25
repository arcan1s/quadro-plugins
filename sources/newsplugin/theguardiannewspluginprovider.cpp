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

#include <QJsonDocument>
#include <QJsonParseError>

#include <quadrocore/quadrodebug.h>


TheGuardianNewsPluginProvider::TheGuardianNewsPluginProvider(QObject *parent, const QString type)
    : NewsPluginProvider(parent, type)
    , m_apiKey(type)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


TheGuardianNewsPluginProvider::~TheGuardianNewsPluginProvider()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QList<NewsPluginMetadata> TheGuardianNewsPluginProvider::data() const
{
    return m_metadata;
}


QString TheGuardianNewsPluginProvider::url() const
{
    return QString("%1?api-key=%2").arg(GUARDIAN_API_URL).arg(m_apiKey);
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
