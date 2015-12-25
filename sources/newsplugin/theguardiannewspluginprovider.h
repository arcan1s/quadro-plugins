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


#ifndef THEGUARDIANNEWSPLUGINPROVIDER_H
#define THEGUARDIANNEWSPLUGINPROVIDER_H

#include "newspluginprovider.h"

#ifndef GUARDIAN_API_URL
#define GUARDIAN_API_URL "http://content.guardianapis.com/search"
#endif /* GUARDIAN_API_URL */


class TheGuardianNewsPluginProvider : public NewsPluginProvider
{
    Q_OBJECT

public:
    explicit TheGuardianNewsPluginProvider(QObject *parent, const QString type);
    virtual ~TheGuardianNewsPluginProvider();
    QList<NewsPluginMetadata> data() const;
    QString url() const;

private slots:
    void replyReceived(QNetworkReply *reply);

private:
    QString m_apiKey;
    QList<NewsPluginMetadata> m_metadata;
};


#endif /* THEGUARDIANNEWSPLUGINPROVIDER_H */
