/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#ifndef CENTERINFOWIDGET_H
#define CENTERINFOWIDGET_H

#include <QWidget>
#include <QMovie>

namespace Ui {
class CenterInfoWidget;
}

/**
  * @class CenterInfoWidget
  *
  * Handle the information communication with the user on the main screen
  */
class CenterInfoWidget : public QWidget
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      */
    explicit CenterInfoWidget(QWidget *parent = 0);
    /**
      * Destruction
      */
    ~CenterInfoWidget();

    /**
      * Display the no device message & pixmap
      */
    void setNoDeviceMode();
    /**
      * Display the bonjour error message & pixmap
      *
      * @param message Message displayed
      */
    void setBonjourErrorMode(const QString &message);
    
private:
    /// GUI
    Ui::CenterInfoWidget *ui;
    /// Animated filesdnd logo
    static QMovie *AnimatedLogo;
    /// Animated bonjour logo
    static QMovie *AnimatedBonjour;
};

#endif // CENTERINFOWIDGET_H
