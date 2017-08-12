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


#ifndef YAHOOWEATHERHELPER_H
#define YAHOOWEATHERHELPER_H

#include <QNetworkReply>
#include <QObject>


class QTimer;

class YahooWeatherHelper : public QObject
{
    Q_OBJECT

    const char *YAHOO_FORECAST_URL
        = "https://query.yahooapis.com/v1/public/yql?format=json&q=select * "
          "from weather.forecast where u='c' and woeid in (select woeid from "
          "geo.places(1) where text='%2, %3')";

public:
    explicit YahooWeatherHelper(QObject *parent, const QString &city,
                                const QString &country);
    virtual ~YahooWeatherHelper();
    QVariantList data() const;

private slots:
    void replyReceived(QNetworkReply *reply);
    void update();

private:
    QString m_city;
    QString m_country;
    QVariantList m_metadata;
    QNetworkAccessManager *m_manager = nullptr;
    QTimer *m_timer = nullptr;
    QString url() const;
};


#endif /* YAHOOWEATHERHELPER_H */
