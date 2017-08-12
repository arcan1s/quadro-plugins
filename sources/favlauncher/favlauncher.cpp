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
#include <QKeyEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <quadroui/quadroui.h>


FavLauncher::~FavLauncher()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    for (auto plugin : m_plugins) {
        int index = plugin[QString("Index")].toInt();
        QString name = plugin[QString("Plugin")].toString();
        m_core->plugin()->unloadPlugin(
            index, QString("%1.favlauncher-%2.conf").arg(name).arg(index));
    }
}


FavLauncher *FavLauncher::createInstance()
{
    return new FavLauncher();
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
            m_core->plugin()->initPlugin(index, QString("%1.favlauncher-%2.conf")
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
}


void FavLauncher::setArgs(QuadroCore *core)
{
    m_core = core;

    // create ui
    QWidget *widget = new QWidget(this);
    // plugin container
    m_container = new PluginContainer(widget, m_core->config()->property("GridSize").toInt(),
                                      QString(), Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff);
    m_container->setFixedWidth(2 * (m_core->config()->property("GridSize").toInt()  + 10));
    // search
    m_searchBar = new SearchBar(widget);
    m_searchBar->setPlaceholderText(tr("Type for search"));

    // layout
    // generic
    QVBoxLayout *genericLayout = new QVBoxLayout(widget);
    genericLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *genericWidget = new QWidget(widget);
    genericLayout->addWidget(m_searchBar);
    genericLayout->addWidget(genericWidget);
    // main
    QHBoxLayout *layout = new QHBoxLayout(genericWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_container);
    genericWidget->setLayout(layout);

    setCentralWidget(widget);

    createActions();
}


bool FavLauncher::eventFilter(QObject *object, QEvent *event)
{
    // move event if key pressed
    if ((event->type() == QEvent::KeyPress)
        && (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Return)) {
        m_searchBar->keyPressed(static_cast<QKeyEvent *>(event));
    }

    return QObject::eventFilter(object, event);
}


void FavLauncher::createActions()
{
    connect(m_searchBar, SIGNAL(returnPressed()), this, SLOT(runCustomApplication()));
    connect(m_searchBar, SIGNAL(textChanged(QString)), this, SLOT(showSearchResults(QString)));
}
