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


#include "favlauncher.h"

#include <QHBoxLayout>

#include <quadroui/quadroui.h>


FavLauncher::~FavLauncher()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;
}


QSize FavLauncher::itemSize()
{
    return QSize(m_appConfiguration[QString("GridSize")].toFloat(),
                 m_appConfiguration[QString("GridSize")].toFloat());
}


QString FavLauncher::name() const
{
    return tr("Favorites");
}


QWidget *FavLauncher::widget()
{
    return this;
}


void FavLauncher::init()
{
    // plugins
    // TODO read from configuration
    QStringList plugins = QStringList() << "datetime" << "pcstatus" << "newsplugin" << "pcstatus" << "yahooweather";
    for (auto plugin : plugins) {
        int index = m_core->plugin()->loadPlugin(plugin);
        if (index == -1) {
            qCWarning(LOG_PL) << "Could not find plugin" << plugin;
        } else {
            m_core->plugin()->initPlugin(index, m_appConfiguration,
                                         QString("%1.favlauncher-%2.conf")
                                             .arg(plugin)
                                             .arg(plugins.indexOf(plugin)));
            auto item = m_core->plugin()->plugin<PluginInterface>(index);
            m_container->addPlugin(item);
            qCInfo(LOG_PL) << "Plugin" << plugin << "has been successfully added";
            // generate metadata
            QVariantHash metadata;
            metadata[QString("Index")] = index;
            metadata[QString("Plugin")] = plugin;
            m_plugins.append(metadata);
        }
    }

    // favorites
    QMap<QString, ApplicationItem *> apps = m_core->favorites()->applications();
    for (auto app : apps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), m_favorites);
        m_favorites->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
//        connect(wItem, SIGNAL(applicationIsRunning()), this, SLOT(hideMainWindow()));
//        connect(wItem, SIGNAL(standaloneApplicaitonRequested(const QStringList, const QString)),
//                this, SLOT(runStandaloneApplication(const QStringList, const QString)));
    }
}


void FavLauncher::setArgs(QuadroCore *core, const QVariantHash settings)
{
    qCDebug(LOG_PL) << "Application settings" << settings;

    m_appConfiguration = settings;
    m_core = core;

    // create ui
    QWidget *widget = new QWidget(this);
    m_container = new PluginContainer(widget, m_appConfiguration[QString("GridSize")].toFloat());
    m_container->setMaximumWidth(2 * m_appConfiguration[QString("GridSize")].toFloat() + 30);
    m_favorites = new QuadroWidget(widget, m_appConfiguration[QString("GridSize")].toFloat());

    // layout
    QHBoxLayout *layout = new QHBoxLayout(widget);
    layout->addWidget(m_container);
    layout->addSpacing(m_appConfiguration[QString("GridSize")].toInt());
    layout->addWidget(m_favorites);
    widget->setLayout(layout);

    setCentralWidget(widget);
}
