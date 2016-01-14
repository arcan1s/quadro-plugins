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


#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QMainWindow>

#include <quadrocore/quadro.h>


class WebPageSettings;
class WebAppWidget;

class WebPage : public QMainWindow, TabPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "core.quadro.webpage/1.0")
    Q_INTERFACES(TabPluginInterface)

public:
    virtual ~WebPage();
    WebPage *createInstance();
    QWidget *configWidget();
    QString name() const;
    QWidget *widget();
    void init();
    void quit(const QString configPath);
    void readSettings(const QString configPath);
    void saveSettings();
    bool writeSettings(const QString configPath) const;
    void setArgs(QuadroCore *);

private:
    // ui
    WebPageSettings *m_config = nullptr;
    WebAppWidget *m_widget = nullptr;
    // backend
    QVariantHash m_configuration;
};


#endif /* WEBPAGE_H */
