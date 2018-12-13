/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
