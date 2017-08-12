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


#ifndef FAVLAUNCHER_H
#define FAVLAUNCHER_H

#include <QMainWindow>

#include <quadrocore/Quadro.h>


class QStackedWidget;

namespace Quadro
{
class PluginContainer;
class QuadroWidget;
class SearchBar;
class StandaloneAppWidget;
}

class FavLauncher : public QMainWindow, Quadro::TabPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.favlauncher/1.0")
    Q_INTERFACES(Quadro::TabPluginInterface)

public:
    virtual ~FavLauncher();
    FavLauncher *createInstance();
    QWidget *configWidget() { return nullptr; };
    QString name() const;
    QWidget *widget();
    void init();
    void quit(const QString &configPath){};
    void readSettings(const QString &configPath){};
    void saveSettings(){};
    bool writeSettings(const QString &) const { return true; };
    void setArgs(Quadro::QuadroCore *core);

protected:
    bool eventFilter(QObject *object, QEvent *event);

private:
    // core ui
    Quadro::PluginContainer *m_container = nullptr;
    Quadro::SearchBar *m_searchBar = nullptr;
    // backend
    Quadro::QuadroCore *m_core = nullptr;
    QList<QVariantHash> m_plugins;
    void createActions();
};


#endif /* FAVLAUNCHER_H */
