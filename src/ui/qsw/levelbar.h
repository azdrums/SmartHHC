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
    @file   levelbar.h
    @author Andrea Zanellato
*/
#ifndef LEVELBAR_H
#define LEVELBAR_H

#include <QWidget>
#include <QVector>

class LevelBar : public QWidget
{
    Q_OBJECT

public:
/**
    Constructor.
*/
    explicit LevelBar(QWidget *parent = nullptr);
/**
    Adds a level with the given value and color.
*/
    void addLevel(int value = 0, const QColor &color = Qt::black);
/**
    Sets the specified level value to the given level index.
*/
    void setLevel(int index, int value);
/**

*/
    int minimum() const;
/**

*/
    int maximum() const;
/**

*/
    int value()   const;
/**

*/
    void setMinimum(int);
/**

*/
    void setMaximum(int);
/**

*/
    void setValue(int);
/**

*/
    Qt::Orientation orientation() const;
/**

*/
    void setOrientation(Qt::Orientation);

private:
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);

    void updateRect();

    QVector<QColor> colors_;
    QVector<int>    values_;

    int minimum_;
    int maximum_;
    int value_;
    int penWidth_;

    QRectF rect_;

    Qt::Orientation orientation_;
};
#endif // LEVELBAR_H
