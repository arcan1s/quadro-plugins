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


#include "yahooweatherhelper.h"

#include <QJsonDocument>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QTimer>

#include <quadrocore/quadrodebug.h>


YahooWeatherHelper::YahooWeatherHelper(QObject *parent, const QString city,
                                       const QString country)
    : QObject(parent)
    , m_city(city)
    , m_country(country)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_manager = new QNetworkAccessManager(nullptr);
    connect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
            SLOT(replyReceived(QNetworkReply *)));

    m_timer = new QTimer(this);
    m_timer->setInterval(1000 * 60 * 60);
    m_timer->setSingleShot(false);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));

    update();
}


YahooWeatherHelper::~YahooWeatherHelper()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    disconnect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    disconnect(m_manager, SIGNAL(finished(QNetworkReply *)), this,
               SLOT(replyReceived(QNetworkReply *)));

    m_manager->deleteLater();
    delete m_timer;
}


QVariantList YahooWeatherHelper::data() const
{
    return m_metadata;
}


void YahooWeatherHelper::replyReceived(QNetworkReply *reply)
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

    // convert to map
    QVariantMap json = jsonDoc.toVariant().toMap()[QString("query")].toMap();
    if (json[QString("count")].toInt() != 1) {
        qCWarning(LOG_LIB) << "Found data count" << json[QString("count")].toInt() << "is not 1";
        return;
    }
    QVariantMap results = json[QString("results")].toMap()[QString("channel")].toMap();
    QVariantMap item = results[QString("item")].toMap();

    m_metadata.clear();
    // current
    QVariantHash current;
    current[QString("id")] = item[QString("condition")].toMap()[QString("code")].toInt();
    current[QString("temperature")] = item[QString("condition")].toMap()[QString("temp")].toInt();
    current[QString("humidity")] = results[QString("atmosphere")].toMap()[QString("humidity")].toInt();
    current[QString("pressure")] = results[QString("atmosphere")].toMap()[QString("pressure")].toFloat();
    m_metadata.append(current);

    // forecast
    for (auto weather : item[QString("forecast")].toList()) {
        QVariantMap weatherMap = weather.toMap();
        QVariantHash forecast;
        forecast[QString("id")] = weatherMap[QString("code")].toInt();
        forecast[QString("date")] = weatherMap[QString("date")].toString();
        forecast[QString("high")] = weatherMap[QString("high")].toInt();
        forecast[QString("low")] = weatherMap[QString("low")].toInt();
        m_metadata.append(forecast);
    }
}


void YahooWeatherHelper::update()
{
    m_manager->get(QNetworkRequest(QUrl(url())));
}


QString YahooWeatherHelper::url() const
{
    QString apiUrl = QString("%1").arg(YAHOO_FORECAST_URL).arg(m_city).arg(m_country);

    qCInfo(LOG_PL) << "Using url" << apiUrl;
    return apiUrl;
}
