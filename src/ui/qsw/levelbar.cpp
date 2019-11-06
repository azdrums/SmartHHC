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
#include <QPaintEvent>
#include <QPainter>
#include <QStyleOption>

#include "levelbar.h"

LevelBar::LevelBar(QWidget *parent)
:
    QWidget(parent),
    minimum_(0),
    maximum_(100),
    value_(0),
    penWidth_(1),
    orientation_(Qt::Vertical)
{
    updateRect();

    QColor enabled  = palette().color(QPalette::Active,   QPalette::Midlight);
    QColor disabled = palette().color(QPalette::Disabled, QPalette::Midlight);
    setStyleSheet(QString("LevelBar {border-radius: 3px}"
                  ":enabled  {border: 1px solid rgb(%1, %2, %3)}"
                  ":disabled {border: 1px solid rgb(%4, %5, %6)}")
                  .arg(enabled.red()) .arg(enabled.green()) .arg(enabled.blue())
                  .arg(disabled.red()).arg(disabled.green()).arg(disabled.blue()));
}
void LevelBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

//  Required by setStyleSheet to work on a subclassed widget
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
    style()->drawPrimitive(QStyle::PE_Frame, &option, &painter, this);

    painter.setRenderHint(QPainter::Antialiasing);

//  Draw chunk
    qreal valueHeight, valueWidth;
    QPainterPath path;
    QRectF chunkRect;
    QPen pen(Qt::darkGreen);
    painter.setPen(pen);

    if (orientation_== Qt::Vertical)
    {
        valueHeight = rect_.height() / maximum_ * value_ - 2;
        chunkRect   = QRect(1, rect_.height() - valueHeight,
                            rect_.width() - 2, valueHeight);
    }
    else
    {
        valueWidth = rect_.width() / maximum_ * value_- 2;
        chunkRect  = QRect(1, 1, valueWidth, rect_.height() - 2);
    }
    path.addRoundedRect(chunkRect, 3, 3);

    painter.fillPath(path, Qt::darkGreen);
    painter.drawPath(path);

//  Draw levels
    if (isEnabled())
    {
        pen.setWidth(penWidth_);
        for (int i = 0; i < values_.size(); ++i)
        {
            pen.setColor(colors_.at(i));
            painter.setPen(pen);

            QPointF p1, p2;
            if (orientation_== Qt::Vertical)
            {
                valueHeight = rect_.height() / maximum_ * values_.at(i) - 2;
                p1 = QPointF(2,                 rect_.height() - valueHeight);
                p2 = QPointF(rect_.width() - 2, rect_.height() - valueHeight);
            }
            else
            {
                valueWidth = rect_.width() / maximum_ * values_.at(i) - 2;
                p1 = QPointF(valueWidth, 1);
                p2 = QPointF(valueWidth, rect_.height() - 2);
            }
            painter.drawLine(p1, p2);
        }
    }
}
void LevelBar::resizeEvent(QResizeEvent *)
{
    updateRect();
}
void LevelBar::updateRect()
{
    rect_ = QRectF(rect().x(), rect().y(),rect().width(), rect().height());
}
int LevelBar::minimum() const
{
    return minimum_;
}
int LevelBar::maximum() const
{
    return maximum_;
}
int LevelBar::value() const
{
    return value_;
}
void LevelBar::setMinimum(int minimum)
{
    if (minimum == minimum_)
    {
        return;
    }
    else if (minimum > value_)
    {
        if (minimum > maximum_)
            maximum_ = minimum;

        value_ = minimum;
    }
    minimum_= minimum;
}
void LevelBar::setMaximum(int maximum)
{
    if (maximum == maximum_)
    {
        return;
    }
    else if (maximum < value_)
    {
        if (maximum < minimum_)
            minimum_ = maximum;

        value_ = maximum;
    }
    maximum_= maximum;
}
void LevelBar::setValue(int value)
{
    if (value < minimum_|| value > maximum_|| value == value_)
        return;

    value_= value;
    update();
}
void LevelBar::addLevel(int value, const QColor &color)
{
    if (value > maximum_)
        value = maximum_;
    else if (value < minimum_)
        value = minimum_;

    values_.push_back(value);
    colors_.push_back(color);
}
void LevelBar::setLevel(int index, int value)
{
    if (index >= values_.size())
        return;

    values_[index] = value;
    update();
}
Qt::Orientation LevelBar::orientation() const
{
    return orientation_;
}
void LevelBar::setOrientation(Qt::Orientation orientation)
{
    orientation_ = orientation;
}
