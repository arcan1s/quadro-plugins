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


#include "YandexNewsPluginProvider.h"

#include <QXmlStreamReader>

#include <quadrocore/quadrodebug.h>


YandexNewsPluginProvider::YandexNewsPluginProvider(QObject *parent,
                                                   const QString &type)
    : NewsPluginProvider(parent, type)
    , m_type(type)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}

YandexNewsPluginProvider::~YandexNewsPluginProvider()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QList<NewsPluginMetadata> YandexNewsPluginProvider::data() const
{
    return m_metadata;
}


QString YandexNewsPluginProvider::url() const
{
    return QString("%1%2.rss").arg(YANDEX_API_URL).arg(m_type);
}


void YandexNewsPluginProvider::replyReceived(QNetworkReply *reply)
{
    qCDebug(LOG_PL) << "Return code" << reply->error() << "reply error message"
                    << reply->errorString();

    QXmlStreamReader xml;
    xml.addData(reply->readAll());
    if ((reply->error() != QNetworkReply::NoError) || (xml.error())) {
        qCWarning(LOG_LIB) << "Parse error" << xml.errorString();
        return;
    }

    m_metadata.clear();
    QString currentTag;
    QString title;
    QString url;
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()) {
            if (xml.name() == "item") {
                title.clear();
                url.clear();
            }
            currentTag = xml.name().toString();
        } else if (xml.isEndElement()) {
            if (xml.name() == "item") {
                NewsPluginMetadata singleData;
                singleData.image = QString();
                singleData.text = title;
                singleData.url = url;
                m_metadata.append(singleData);
            }
        } else if ((xml.isCharacters()) && (!xml.isWhitespace())) {
            if (currentTag == "title")
                title += xml.text().toString();
            else if (currentTag == "link")
                url += xml.text().toString();
        }
    }
}
