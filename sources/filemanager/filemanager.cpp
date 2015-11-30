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


#include "filemanager.h"

#include <QAction>
#include <QApplication>
#include <QKeyEvent>
#include <QLineEdit>
#include <QSettings>
#include <QStackedWidget>
#include <QToolBar>
#include <QVBoxLayout>

#include <quadroui/quadroui.h>

#include "filemanagersettings.h"


FileManager::~FileManager()
{
    qCDebug(LOG_PL) << __PRETTY_FUNCTION__;

    m_pageButtons.clear();
    m_pageWidgets.clear();

    delete m_toolBar;
    delete m_searchBar;
    delete m_stackedWidget;

    delete m_config;
}


QSize FileManager::itemSize()
{
    return QSize(m_appConfiguration[QString("GridSize")].toFloat(),
                 m_appConfiguration[QString("GridSize")].toFloat());
}


QWidget *FileManager::configWidget()
{
    return m_config;
}


QString FileManager::name() const
{
    return tr("Files");
}


QWidget *FileManager::widget()
{
    return this;
}


void FileManager::init()
{
    addToolBar(static_cast<Qt::ToolBarArea>(
                   m_configuration[QString("BarPosition")].toInt()), m_toolBar);

    // search widget
    m_pageWidgets.append(new QuadroWidget(this, itemSize().width()));
    m_pageButtons.append(m_toolBar->addAction("search"));
    m_pageButtons.last()->setVisible(false);
    m_stackedWidget->addWidget(m_pageWidgets.last());

    return createPage(m_configuration.value(QString("path"), QDir::homePath()).toString());
}


void FileManager::quit(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    QSettings settings(configPath, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");

    settings.beginGroup(QString("UI"));
    settings.setValue(QString("BarPosition"), toolBarArea(m_toolBar));
    settings.endGroup();

    settings.sync();
}


void FileManager::readSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    m_configuration = m_config->readSettings(configPath);
}


bool FileManager::saveSettings(const QString configPath)
{
    qCDebug(LOG_PL) << "Configuration path" << configPath;

    return m_config->saveSettings(configPath, m_configuration);
}


void FileManager::setArgs(QuadroCore *core, const QVariantHash settings)
{
    qCDebug(LOG_PL) << "Application settings" << settings;

    m_appConfiguration = settings;
    m_core = core;
    m_config = new FileManagerSettings(this);

    // create ui
    m_toolBar = new QToolBar(this);

    // ui
    QWidget *widget = new QWidget(this);
    m_searchBar = new QLineEdit(widget);
    m_searchBar->setPlaceholderText(tr("Type filename here"));
    m_stackedWidget = new QStackedWidget(widget);
    // layout
    QVBoxLayout *layout = new QVBoxLayout(widget);
    layout->addWidget(m_searchBar);
    layout->addWidget(m_stackedWidget);
    widget->setLayout(layout);

    setCentralWidget(widget);
    // handle child events
    m_searchBar->installEventFilter(this);
    m_stackedWidget->installEventFilter(this);
    m_toolBar->installEventFilter(this);

    createActions();
}


void FileManager::changePage(const int index)
{
    qCDebug(LOG_PL) << "Index" << index;
    if ((index == -1) || (index >= m_stackedWidget->count())) return;

    return m_stackedWidget->setCurrentIndex(index);
}


void FileManager::createPage(const QString path)
{
    qCDebug(LOG_PL) << "Create page with path" << path;

    // create
    int index = m_stackedWidget->count() == 1 ? 0 : m_stackedWidget->currentIndex() + 1;
    m_pageWidgets.insert(index, new QuadroWidget(this, itemSize().width(), path));
    m_stackedWidget->insertWidget(index, m_pageWidgets[index]);
    QAction *act = new QAction(m_toolBar);
    m_toolBar->insertAction(m_pageButtons[index], act);
    m_pageButtons.insert(index, act);

    QFileInfoList entries = m_core->filemanager()->directoryEntries(path, m_configuration[QString("ShowHidden")].toBool());
    for (auto entry : entries) {
        QWidget *wItem = new FileIconWidget(entry, m_core->filemanager()->iconByFileName(entry.absoluteFilePath()),
                                            itemSize(), m_pageWidgets[index]->widget());
        m_pageWidgets[index]->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(openFile(const QFileInfo)), this, SLOT(openEntry(const QFileInfo)));
        connect(wItem, SIGNAL(openDirInNewTab(const QFileInfo)), this,
                SLOT(openEntryInNewTab(const QFileInfo)));
    }

    m_stackedWidget->setCurrentIndex(index);
    updateToolbar();
}


void FileManager::loadPage(const QString path)
{
    qCDebug(LOG_PL) << "Load path" << path;

    int index = m_stackedWidget->currentIndex();
    m_pageWidgets[index]->clearLayout();

    QFileInfoList entries = m_core->filemanager()->directoryEntries(path, m_configuration[QString("ShowHidden")].toBool());
    for (auto entry : entries) {
        QWidget *wItem = new FileIconWidget(entry, m_core->filemanager()->iconByFileName(entry.absoluteFilePath()),
                                            itemSize(), m_pageWidgets[index]->widget());
        m_pageWidgets[index]->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(openFile(const QFileInfo)), this, SLOT(openEntry(const QFileInfo)));
        connect(wItem, SIGNAL(openDirInNewTab(const QFileInfo)), this,
                SLOT(openEntryInNewTab(const QFileInfo)));
    }

    updateToolbar();
}


bool FileManager::eventFilter(QObject *object, QEvent *event)
{
    if ((event->type() == QEvent::KeyPress)
        && (static_cast<QKeyEvent *>(event)->key() != Qt::Key_Return)) {
        if ((static_cast<QKeyEvent *>(event)->key() == Qt::Key_Backspace)
            && (!m_searchBar->hasFocus())) {
            loadPage(QString("%1/..").arg(static_cast<QuadroWidget *>(m_stackedWidget->currentWidget())->title()));
        } else {
            m_searchBar->setFocus();
        }
    }

    return QObject::eventFilter(object, event);
}


void FileManager::changePageByAction(QAction *action)
{
    return changePage(m_pageButtons.indexOf(action));
}


void FileManager::openEntry(const QFileInfo entry)
{
    qCDebug(LOG_PL) << "Open" << entry.absoluteFilePath();

    if (entry.isDir()) {
        loadPage(entry.absoluteFilePath());
    } else {
        m_core->filemanager()->openFile(entry);
        DBusOperations::sendRequestToUi(QString("Hide"));
    }

    static_cast<QuadroWidget *>(m_stackedWidget->currentWidget())->resetFocus();
    static_cast<QuadroWidget *>(m_stackedWidget->currentWidget())->setFocus();
}


void FileManager::openEntryInNewTab(const QFileInfo entry)
{
    qCDebug(LOG_PL) << "Open" << entry.absoluteFilePath();

    if (entry.isFile())
        return openEntry(entry);

    createPage(entry.absoluteFilePath());
}


void FileManager::showSearchResults(const QString search)
{
    qCDebug(LOG_PL) << "Search substring" << search;

    m_pageWidgets.last()->clearLayout();

    // return if none to do here
    if (search.count() <= MIN_SUBSTRING_LENGTH)
        return m_stackedWidget->setCurrentIndex(m_searchIndex);
    // add items
    QFileInfoList entries = m_core->filemanager()->entriesBySubstr(m_pageWidgets.at(m_searchIndex)->title(),
                                                                   search,
                                                                   m_configuration[QString("ShowHidden")].toBool());

    // counter to limit loaded items
    int count = 0;
    for (auto entry : entries) {
        QWidget *wItem = new FileIconWidget(entry, m_core->filemanager()->iconByFileName(entry.absoluteFilePath()),
                                            itemSize(), m_pageWidgets.last()->widget());
        m_pageWidgets.last()->widget()->layout()->addWidget(wItem);
        connect(wItem, SIGNAL(openFile(const QFileInfo)), this,
                SLOT(openEntryInNewTab(const QFileInfo)));
        connect(wItem, SIGNAL(openDirInNewTab(const QFileInfo)), this,
                SLOT(openEntryInNewTab(const QFileInfo)));
        if (++count >= m_configuration[QString("SearchResult")].toInt()) {
            qCInfo(LOG_PL) << "Found" << count << "entries, breaking";
            break;
        }
    }

    if (m_stackedWidget->currentIndex() != (m_stackedWidget->count() - 1))
        m_searchIndex = m_stackedWidget->currentIndex();

    return m_stackedWidget->setCurrentIndex(m_stackedWidget->count() - 1);
}


void FileManager::createActions()
{
    connect(m_toolBar, SIGNAL(actionTriggered(QAction *)),
            this, SLOT(changePageByAction(QAction *)));
    connect(m_searchBar, SIGNAL(textChanged(QString)), this,
            SLOT(showSearchResults(QString)));
}


void FileManager::updateToolbar()
{
    for (int i=0; i<m_pageButtons.count(); i++)
        m_pageButtons[i]->setText(QFileInfo(m_pageWidgets.at(i)->title()).fileName());

    m_toolBar->setVisible(m_configuration[QString("ShowToolbar")].toBool()
                          || m_pageButtons.count() >= 3);
}
