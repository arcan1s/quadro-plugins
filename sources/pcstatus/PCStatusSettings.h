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

#ifndef PCSTATUSSETTINGS_H
#define PCSTATUSSETTINGS_H

#include <QWidget>


namespace Ui
{
class PCStatusSettings;
}

class PCStatusSettings : public QWidget
{
    Q_OBJECT

public:
    explicit PCStatusSettings(QWidget *parent = nullptr);
    virtual ~PCStatusSettings();
    QVariantHash readSettings(const QString &configPath);
    QVariantHash saveSettings() const;
    bool writeSettings(const QString &configPath,
                       const QVariantHash &configuration) const;

private:
    Ui::PCStatusSettings *ui;
    void init(const QVariantHash &configuration);
};


#endif /* PCSTATUSSETTINGS_H */
