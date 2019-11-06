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
    @file   menustyle.h
    @author Stefan Frings
*/
#ifndef MENUSTYLE_H
#define MENUSTYLE_H

#include <QProxyStyle>
#include <QStyleOption>

/**
    This proxy style changes the size of icons relative to the font size.
    I use it to give icons a proper size on high-res displays.
*/
class MenuStyle : public QProxyStyle
{
public:
/**
    Calculate the size of icons relative to the font size
*/
    int pixelMetric(PixelMetric, const QStyleOption *option = nullptr,
                                 const QWidget      *widget = nullptr) const;
};

#endif // MENUSTYLE_H
