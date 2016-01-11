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

#include <quadrocore/quadro.h>


class PluginContainer;
class QuadroWidget;

class FavLauncher : public QMainWindow, TabPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.favlauncher/1.0")
    Q_INTERFACES(TabPluginInterface)

public:
    virtual ~FavLauncher();
    FavLauncher *createInstance();
    inline QSize itemSize();
    QWidget *configWidget() { return nullptr; };
    QString name() const;
    QWidget *widget();
    void init();
    void quit(const QString configPath) {};
    void readSettings(const QString configPath) {};
    void saveSettings() { };
    bool writeSettings(const QString) const { return true; };
    void setArgs(QuadroCore *core, const QVariantHash settings);

private slots:
    void runApplication();

private:
    // ui
    PluginContainer *m_container = nullptr;
    QuadroWidget *m_favorites = nullptr;
    // backend
    QVariantHash m_appConfiguration;
    QuadroCore *m_core = nullptr;
    QList<QVariantHash> m_plugins;
};


#endif /* FAVLAUNCHER_H */
