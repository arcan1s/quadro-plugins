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


#include "AppLauncher.h"

#include <QAction>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QSettings>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

#include <quadroui/QuadroUi.h>

using namespace Quadro;


AppLauncher::~AppLauncher()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_categoryButtons.clear();
    m_categoryWidgets.clear();

    delete m_toolBar;
    delete m_searchBar;
    delete m_stackedWidget;
}


AppLauncher *AppLauncher::createInstance()
{
    return new AppLauncher();
}


QString AppLauncher::name() const
{
    return tr("Launcher");
}


QWidget *AppLauncher::widget()
{
    return this;
}


void AppLauncher::init()
{
    addToolBar(m_toolBarArea, m_toolBar);

    // clear
    QWidget *wid;
    while ((wid = m_stackedWidget->currentWidget()))
        m_stackedWidget->removeWidget(wid);
    m_toolBar->clear();
    m_categoryButtons.clear();
    m_categoryWidgets.clear();

    QStringList categories = m_core->launcher()->availableCategories();
    for (auto &cat : categories) {
        m_categoryButtons.append(m_toolBar->addAction(cat));
        m_categoryWidgets.append(
            new QuadroWidget(this, m_core->config()->gridSize()));
        m_stackedWidget->addWidget(m_categoryWidgets.last());
        initCategory(cat, m_categoryWidgets.last()->widget());
    }

    // search widget
    m_categoryWidgets.append(
        new QuadroWidget(this, m_core->config()->gridSize()));
    m_stackedWidget->addWidget(m_categoryWidgets.last());
}


void AppLauncher::quit(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup("UI");
    settings.setValue("BarPosition", toolBarArea(m_toolBar));
    settings.endGroup();

    settings.sync();
}


void AppLauncher::readSettings(const QString &configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup("UI");
    m_toolBarArea = static_cast<Qt::ToolBarArea>(
        settings.value("BarPosition", Qt::TopToolBarArea).toInt());
    settings.endGroup();
}


void AppLauncher::setArgs(QuadroCore *core)
{
    m_core = core;

    // create ui
    m_toolBar = new QToolBar(this);

    // ui
    QWidget *widget = new QWidget(this);
    m_searchBar = new SearchBar(widget);
    m_searchBar->setPlaceholderText(tr("Type for search"));
    m_stackedWidget = new QStackedWidget(widget);
    // layout
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(m_searchBar);
    layout->addWidget(m_stackedWidget);
    widget->setLayout(layout);

    setCentralWidget(widget);
    // handle child events
    m_toolBar->installEventFilter(this);
    m_searchBar->installEventFilter(this);
    m_stackedWidget->installEventFilter(this);

    createActions();
}


void AppLauncher::changeCategory(const int index)
{
    qCDebug(LOG_PL) << "Index" << index;
    if ((index == -1) || (index >= m_stackedWidget->count()))
        return;

    return m_stackedWidget->setCurrentIndex(index);
}


bool AppLauncher::eventFilter(QObject *object, QEvent *event)
{
    // move event if key pressed
    if ((event->type() == QEvent::KeyPress)
        && (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Return)) {
        m_searchBar->keyPressed(static_cast<QKeyEvent *>(event));
    }

    return QObject::eventFilter(object, event);
}


void AppLauncher::changeCategoryByAction(QAction *action)
{
    return changeCategory(m_categoryButtons.indexOf(action));
}


void AppLauncher::hideMainWindow()
{
    DBusOperations::sendRequestToUi("Hide");
}


void AppLauncher::runApplication()
{
    m_core->recently()->addItem(
        static_cast<AppIconWidget *>(sender())->associatedItem());
}


void AppLauncher::runCustomApplication()
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
        hideMainWindow();
    } else {
        QMessageBox::critical(this, tr("Error"),
                              tr("Could not run application %1").arg(exec));
    }
}


void AppLauncher::runStandaloneApplication(const QStringList &exec,
                                           const QString &name)
{
    qCDebug(LOG_PL) << "Exec" << exec << "application name" << name;

    DBusOperations::sendRequestToUi(QString("RunContainer"),
                                    QVariantList() << exec << name);
}


void AppLauncher::showSearchResults(const QString &search)
{
    qCDebug(LOG_PL) << "Search substring" << search;

    m_categoryWidgets.last()->clearLayout();

    // return if none to do here
    if (search.isEmpty())
        return m_stackedWidget->setCurrentIndex(0);
    // add items
    QMap<QString, ApplicationItem *> apps
        = m_core->recently()->applicationsBySubstr(search);
    QMap<QString, ApplicationItem *> launcherApps
        = m_core->launcher()->applicationsBySubstr(search);
    for (auto app : apps.values() + launcherApps.values()) {
        QWidget *wItem = new AppIconWidget(app, m_core->config()->gridSize(),
                                           m_categoryWidgets.last()->widget());
        m_categoryWidgets.last()->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
        connect(wItem, SIGNAL(applicationIsRunning()), this,
                SLOT(hideMainWindow()));
        connect(wItem, SIGNAL(standaloneApplicationRequested(
                           const QStringList &, const QString &)),
                this, SLOT(runStandaloneApplication(const QStringList,
                                                    const QString)));
    }

    m_stackedWidget->setCurrentIndex(m_stackedWidget->count() - 1);
    m_categoryWidgets.last()->resetFocus();
}


void AppLauncher::createActions()
{
    connect(m_toolBar, SIGNAL(actionTriggered(QAction *)), this,
            SLOT(changeCategoryByAction(QAction *)));
    connect(m_searchBar, SIGNAL(returnPressed()), this,
            SLOT(runCustomApplication()));
    connect(m_searchBar, SIGNAL(textChanged(QString)), this,
            SLOT(showSearchResults(QString)));
}


void AppLauncher::initCategory(const QString &category, QWidget *widget)
{
    qCDebug(LOG_PL) << "Category" << category;

    QMap<QString, ApplicationItem *> apps
        = m_core->launcher()->applicationsByCategory(category);
    for (auto app : apps.values()) {
        QWidget *wItem
            = new AppIconWidget(app, m_core->config()->gridSize(), widget);
        widget->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
        connect(wItem, SIGNAL(applicationIsRunning()), this,
                SLOT(hideMainWindow()));
        connect(wItem, SIGNAL(standaloneApplicationRequested(
                           const QStringList &, const QString &)),
                this, SLOT(runStandaloneApplication(const QStringList,
                                                    const QString)));
    }
}
