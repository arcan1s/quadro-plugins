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


#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QMainWindow>

#include <quadrocore/Quadro.h>


class QAction;
class FileManagerSettings;
class QStackedWidget;
class QToolBar;

namespace Quadro
{
class QuadroWidget;
class SearchBar;
};

class FileManager : public QMainWindow, Quadro::TabPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.filemanager/1.0")
    Q_INTERFACES(Quadro::TabPluginInterface)

    const int MIN_SUBSTRING_LENGTH = 3;

public:
    virtual ~FileManager();
    FileManager *createInstance();
    QWidget *configWidget();
    QString name() const;
    QWidget *widget();
    void init();
    void quit(const QString &configPath);
    void readSettings(const QString &configPath);
    void saveSettings();
    bool writeSettings(const QString &configPath) const;
    void setArgs(Quadro::QuadroCore *core);

public slots:
    void changePage(const int index);
    void createPage(const QString &path);
    void loadPage(const QString &path);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void changePageByAction(QAction *action);
    void openEntry(const QFileInfo &entry);
    void openEntryInNewTab(const QFileInfo &entry);
    void showSearchResults(const QString &search);

private:
    // ui
    QList<QAction *> m_pageButtons;
    QList<Quadro::QuadroWidget *> m_pageWidgets;
    FileManagerSettings *m_config = nullptr;
    // core ui
    Quadro::SearchBar *m_searchBar = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;
    QToolBar *m_toolBar = nullptr;
    // backend
    QVariantHash m_configuration;
    Quadro::QuadroCore *m_core = nullptr;
    int m_searchIndex = 0;
    // backend
    void createActions();
    void updateToolbar();
};


#endif /* FILEMANAGER_H */
