/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file   utils.h
    @author Andrea Zanellato
*/
#ifndef UTILS_H
#define UTILS_H

#include <QColor>
#include <QIcon>

namespace qaw
{
namespace color
{
    bool isDark( const QColor &color )
    {
        int average = ( color.red() + color.green() + color.blue() ) / 3;
        if ( average < 128 )
            return true;

        return false;
    }
    QIcon changeIconColor(const QIcon &icon, const QSize &iconSize, QColor color)
    {
        QPixmap pixmap = icon.pixmap(iconSize);
        QImage tmp = pixmap.toImage();

        for(int y = 0; y < tmp.height(); y++)
        {
            for(int x= 0; x < tmp.width(); x++)
            {
                color.setAlpha(tmp.pixelColor(x,y).alpha());
                tmp.setPixelColor(x, y, color);
            }
        }
        pixmap = QPixmap::fromImage(tmp);
        return QIcon(pixmap);
    }
}
}

#endif // UTILS_H
