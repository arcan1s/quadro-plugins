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

    delete m_container;
    delete m_documents;
    delete m_favorites;
    delete m_recent;
    delete m_searchBar;
    delete m_searchWidget;
    delete m_standaloneApp;
    delete m_recentWidget;
}


FavLauncher *FavLauncher::createInstance()
{
    return new FavLauncher();
}


QSize FavLauncher::itemSize()
{
    return QSize(m_core->config()->property("GridSize").toInt() ,
                 m_core->config()->property("GridSize").toInt() );
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

    // favorites
    QMap<QString, ApplicationItem *> apps = m_core->favorites()->applications();
    for (auto app : apps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), m_favorites);
        m_favorites->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
        connect(wItem, SIGNAL(applicationIsRunning()), this, SLOT(hideMainWindow()));
        connect(wItem, SIGNAL(standaloneApplicaitonRequested(const QStringList, const QString)),
                this, SLOT(runStandaloneApplication(const QStringList, const QString)));
    }

    // recent apps
    apps = m_core->recently()->applications();
    for (auto app : apps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), m_recent);
        m_recent->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
        connect(wItem, SIGNAL(applicationIsRunning()), this, SLOT(hideMainWindow()));
        connect(wItem, SIGNAL(standaloneApplicaitonRequested(const QStringList, const QString)),
                this, SLOT(runStandaloneApplication(const QStringList, const QString)));
    }

    // recent docs
    apps = m_core->documents()->applications();
    for (auto app : apps.values()) {
        QWidget *wItem = new FileIconWidget(app->url(), app->icon(), itemSize(), m_documents);
        m_documents->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(openFile(const QFileInfo)), this,
                SLOT(openFile(const QFileInfo)));
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
    // favorites apps
    m_favorites = new QuadroWidget(widget, m_core->config()->property("GridSize").toInt(),
                                   QString(), Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff);
    m_favorites->setMinimumWidth(m_core->config()->property("GridSize").toInt()  + 10);
    // recent apps and documents
    m_recent = new QuadroWidget(widget, m_core->config()->property("GridSize").toInt(),
                                QString(), Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff);
    m_documents = new QuadroWidget(widget, m_core->config()->property("GridSize").toInt(),
                                   QString(), Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff);
    m_recentWidget = new QWidget(widget);
    m_recentWidget->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *recentLayout = new QVBoxLayout(m_recentWidget);
    recentLayout->setContentsMargins(0, 0, 0, 0);
    recentLayout->addWidget(m_recent);
    recentLayout->addWidget(m_documents);
    m_recentWidget->setLayout(recentLayout);
    // search information
    m_searchBar = new SearchBar(widget);
    m_searchBar->setPlaceholderText(tr("Type for search"));
    m_searchWidget = new QuadroWidget(widget, m_core->config()->property("GridSize").toInt(),
                                      QString(), Qt::ScrollBarAlwaysOff, Qt::ScrollBarAlwaysOff);
    m_searchWidget->hide();

    // layout
    // generic
    QVBoxLayout *genericLayout = new QVBoxLayout(widget);
    genericLayout->setContentsMargins(0, 0, 0, 0);
    QWidget *genericWidget = new QWidget(widget);
    genericLayout->addWidget(m_searchBar);
    genericLayout->addWidget(genericWidget);
    // data
    QHBoxLayout *layout = new QHBoxLayout(genericWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_container);
    layout->addWidget(m_recentWidget);
    layout->addWidget(m_searchWidget);
    layout->addWidget(m_favorites);
    genericWidget->setLayout(layout);

    setCentralWidget(widget);
    // handle child events
    m_container->installEventFilter(this);
    m_documents->installEventFilter(this);
    m_favorites->installEventFilter(this);
    m_recent->installEventFilter(this);
    m_searchBar->installEventFilter(this);
    m_searchWidget->installEventFilter(this);
    m_recentWidget->installEventFilter(this);

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



void FavLauncher::hideMainWindow()
{
    DBusOperations::sendRequestToUi(QString("Hide"));
}


void FavLauncher::openFile(const QFileInfo entry)
{
    qCDebug(LOG_PL) << "Open" << entry.absoluteFilePath();

    m_core->filemanager()->openFile(entry);
    m_core->documents()->addItem(entry.absoluteFilePath());
    hideMainWindow();
}


void FavLauncher::removeContainer()
{
    disconnect(m_standaloneApp, SIGNAL(destroyWindow(const int)), this, SLOT(removeContainer()));
    auto item = static_cast<QHBoxLayout *>(centralWidget()->layout())->takeAt(1);
    item->widget()->deleteLater();
    delete item;
    m_standaloneApp = nullptr;

    m_recentWidget->show();
}


void FavLauncher::runApplication()
{
    m_core->recently()->addItem(static_cast<AppIconWidget *>(sender())->associatedItem());
}


void FavLauncher::runCustomApplication()
{
    QString exec = m_searchBar->text();
    if (exec.isEmpty()) {
        qCWarning(LOG_PL) << "Empty cmd, nothing to do";
        return;
    }
    m_searchBar->clear();

    ApplicationItem *item = new ApplicationItem(this, exec);
    item->setExec(exec);

    if (item->launch()) {
        m_core->recently()->addItem(item);
        DBusOperations::sendRequestToUi(QString("Hide"));
    } else {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Could not run application %1").arg(exec));
    }
}


void FavLauncher::runStandaloneApplication(const QStringList exec, const QString)
{
    if (m_standaloneApp != nullptr) {
        qCWarning(LOG_PL) << "Standalone application is already registered";
        return;
    }
    qCDebug(LOG_PL) << "Exec" << exec;

    m_standaloneApp = new StandaloneAppWidget(this, exec, 0);
    connect(m_standaloneApp, SIGNAL(destroyWindow(const int)), this, SLOT(removeContainer()));

    m_recentWidget->hide();
    static_cast<QHBoxLayout *>(centralWidget()->layout())->insertWidget(1, m_standaloneApp);
}


void FavLauncher::showSearchResults(const QString search)
{
    qCDebug(LOG_PL) << "Search substring" << search;

    m_searchWidget->clearLayout();

    // return if none to do here
    if (search.isEmpty())
        return m_searchWidget->hide();
    // add items
    QMap<QString, ApplicationItem *> apps = m_core->recently()->applicationsBySubstr(search);
    QMap<QString, ApplicationItem *> launcherApps = m_core->launcher()->applicationsBySubstr(search);
    for (auto app : apps.values() + launcherApps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), m_searchWidget->widget());
        m_searchWidget->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
        connect(wItem, SIGNAL(applicationIsRunning()), this, SLOT(hideMainWindow()));
        connect(wItem, SIGNAL(standaloneApplicaitonRequested(const QStringList, const QString)),
                this, SLOT(runStandaloneApplication(const QStringList, const QString)));
    }

    return m_searchWidget->show();
}


void FavLauncher::createActions()
{
    connect(m_searchBar, SIGNAL(returnPressed()), this, SLOT(runCustomApplication()));
    connect(m_searchBar, SIGNAL(textChanged(QString)), this, SLOT(showSearchResults(QString)));
}
