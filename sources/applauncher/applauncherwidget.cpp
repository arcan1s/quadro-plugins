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


#include "applauncherwidget.h"

#include <QAction>
#include <QApplication>
#include <QKeyEvent>
#include <QLineEdit>
#include <QMessageBox>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

#include <quadroui/quadroui.h>


AppLauncher::~AppLauncher()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_categoryButtons.clear();
    m_categoryWidgets.clear();
    // ui objects are already removed
}


QSize AppLauncher::itemSize()
{
    return QSize(m_appConfiguration[QString("GridSize")].toFloat(),
                 m_appConfiguration[QString("GridSize")].toFloat());
}


QString AppLauncher::name() const
{
    return QApplication::translate("AppLauncher", "Launcher");
}


QWidget *AppLauncher::widget()
{
    return m_mainWindow;
}


void AppLauncher::init()
{
    // clear
    QWidget *wid;
    while ((wid = m_stackedWidget->currentWidget()))
        m_stackedWidget->removeWidget(wid);
    m_toolBar->clear();
    m_categoryButtons.clear();
    m_categoryWidgets.clear();

    QStringList categories = m_core->launcher()->availableCategories();
    foreach (const QString cat, categories) {
        m_categoryButtons.append(m_toolBar->addAction(cat));
        m_categoryWidgets.append(new QuadroWidget(m_mainWindow, itemSize().width()));
        m_stackedWidget->addWidget(m_categoryWidgets.last());
        initCategory(cat, m_categoryWidgets.last()->widget());
    }

    // search widget
    m_categoryWidgets.append(new QuadroWidget(m_mainWindow, itemSize().width()));
    m_stackedWidget->addWidget(m_categoryWidgets.last());
}


void AppLauncher::setArgs(QuadroCore *core, const QVariantHash settings)
{
    qCDebug(LOG_PL) << "Application settings" << settings;

    m_appConfiguration = settings;
    m_core = core;

    // create ui
    m_mainWindow = new QMainWindow(nullptr);
    m_toolBar = new QToolBar(m_mainWindow);
    m_mainWindow->addToolBar(m_toolBar);

    // ui
    QWidget *widget = new QWidget(m_mainWindow);
    m_searchBar = new QLineEdit(widget);
    m_searchBar->setPlaceholderText(QApplication::translate("AppLauncher", "Type application name here"));
    m_stackedWidget = new QStackedWidget(widget);
    // layout
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(m_searchBar);
    layout->addWidget(m_stackedWidget);
    widget->setLayout(layout);

    m_mainWindow->setCentralWidget(widget);
    // handle child events
    m_mainWindow->installEventFilter(this);
    m_toolBar->installEventFilter(this);
    m_searchBar->installEventFilter(this);
    m_stackedWidget->installEventFilter(this);

    createActions();
}


void AppLauncher::changeCategory(const int index)
{
    qCDebug(LOG_PL) << "Index" << index;
    if ((index == -1) || (index >= m_stackedWidget->count())) return;

    return m_stackedWidget->setCurrentIndex(index);
}


bool AppLauncher::eventFilter(QObject *object, QEvent *event)
{
    // move event if key pressed
    if ((event->type() == QEvent::KeyPress)
        && (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Return)) {
        m_searchBar->setFocus();
    }

    return QObject::eventFilter(object, event);
}


void AppLauncher::changeCategoryByAction(QAction *action)
{
    return changeCategory(m_categoryButtons.indexOf(action));
}


void AppLauncher::runApplication()
{
    m_core->recently()->addItem(static_cast<AppIconWidget *>(sender())->associatedItem());
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

    if (item->launch())
        m_core->recently()->addItem(item);
    else
        QMessageBox::critical(m_mainWindow, QApplication::translate("AppLauncher", "Error"),
                              QApplication::translate("AppLauncher", "Could not run application %1").arg(exec));

    DBusOperations::sendRequestToUi(QString("Hide"));
}


void AppLauncher::showSearchResults(const QString search)
{
    qCDebug(LOG_PL) << "Search substring" << search;

    // clear
    QLayoutItem *item;
    while ((item = m_categoryWidgets.last()->widget()->layout()->takeAt(0))) {
        delete item->widget();
        delete item;
    }

    // return if none to do here
    if (search.isEmpty()) return m_stackedWidget->setCurrentIndex(0);
    // add items
    QMap<QString, ApplicationItem *> apps = m_core->recently()->applicationsBySubstr(search);
    QMap<QString, ApplicationItem *> launcherApps = m_core->launcher()->applicationsBySubstr(search);
    foreach (ApplicationItem *app, apps.values() + launcherApps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), m_categoryWidgets.last()->widget());
        m_categoryWidgets.last()->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
    }

    return m_stackedWidget->setCurrentIndex(m_stackedWidget->count() - 1);
}


void AppLauncher::createActions()
{
    connect(m_toolBar, SIGNAL(actionTriggered(QAction *)),
            this, SLOT(changeCategoryByAction(QAction *)));
    connect(m_searchBar, SIGNAL(returnPressed()), this, SLOT(runCustomApplication()));
    connect(m_searchBar, SIGNAL(textChanged(QString)), this, SLOT(showSearchResults(QString)));
}


void AppLauncher::initCategory(const QString category, QWidget *widget)
{
    qCDebug(LOG_PL) << "Category" << category;

    QMap<QString, ApplicationItem *> apps = m_core->launcher()->applicationsByCategory(category);
    foreach (ApplicationItem *app, apps.values()) {
        QWidget *wItem = new AppIconWidget(app, itemSize(), widget);
        widget->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(widgetPressed()), this, SLOT(runApplication()));
    }
}
