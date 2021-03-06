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


#ifndef APPLAUNCHER_H
#define APPLAUNCHER_H

#include <QMainWindow>

#include <quadrocore/Quadro.h>


class QAction;
class QStackedWidget;
class QToolBar;

namespace Quadro
{
class QuadroWidget;
class SearchBar;
};

class AppLauncher : public QMainWindow, Quadro::TabPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.applauncher/1.0")
    Q_INTERFACES(Quadro::TabPluginInterface)

public:
    virtual ~AppLauncher();
    AppLauncher *createInstance();
    QWidget *configWidget() { return nullptr; };
    QString name() const;
    QWidget *widget();
    void init();
    void quit(const QString &configPath);
    void readSettings(const QString &configPath);
    void saveSettings(){};
    bool writeSettings(const QString &) const { return true; };
    void setArgs(Quadro::QuadroCore *core);

public slots:
    void changeCategory(const int index);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void changeCategoryByAction(QAction *action);
    void hideMainWindow();
    void runApplication();
    void runCustomApplication();
    void runStandaloneApplication(const QStringList &exec, const QString &name);
    void showSearchResults(const QString &search);

private:
    // ui
    QList<QAction *> m_categoryButtons;
    QList<Quadro::QuadroWidget *> m_categoryWidgets;
    // core ui
    Quadro::SearchBar *m_searchBar = nullptr;
    QStackedWidget *m_stackedWidget = nullptr;
    QToolBar *m_toolBar = nullptr;
    Qt::ToolBarArea m_toolBarArea = Qt::TopToolBarArea;
    // backend
    Quadro::QuadroCore *m_core = nullptr;
    void createActions();
    void initCategory(const QString &category, QWidget *widget);
};


#endif /* APPLAUNCHER_H */
