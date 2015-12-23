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


#include "yandexnewspluginprovider.h"

#include <quadrocore/quadrodebug.h>


YandexNewsPluginProvider::YandexNewsPluginProvider(QObject *parent)
    : NewsPluginProvider(parent)
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


YandexNewsPluginProvider::~YandexNewsPluginProvider()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QList<NewsPluginMetadata> YandexNewsPluginProvider::retrieve()
{
    QList<NewsPluginMetadata> metadata;

    return metadata;
}
