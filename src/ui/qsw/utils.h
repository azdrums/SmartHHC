/*
	SmartWidgets - Custom Android like Qt based Widgets Library
	Copyright (C) 2018-2019 Andrea Zanellato

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/**
    @file   utils.h
    @author Andrea Zanellato
*/
#ifndef UTILS_H
#define UTILS_H

#include <QColor>
#include <QIcon>

namespace qsw
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
