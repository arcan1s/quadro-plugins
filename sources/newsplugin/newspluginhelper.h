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


#ifndef NEWSPLUGINHELPER_H
#define NEWSPLUGINHELPER_H

#include <QObject>

#include "newspluginprovider.h"


class QTimer;

class NewsPluginHelper : public QObject
{
    Q_OBJECT

public:
    explicit NewsPluginHelper(QObject *parent, const int provider,
                              const QString type, const int interval);
    virtual ~NewsPluginHelper();
    int bump();
    NewsPluginMetadata news() const;

public slots:
    void update();

private:
    int m_current = -1;
    NewsPluginProvider *m_provider = nullptr;
    QTimer *m_timer = nullptr;
};


#endif /* NEWSPLUGINHELPER_H */
