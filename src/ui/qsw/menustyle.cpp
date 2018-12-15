/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file    menustyle.cpp
    @authors Stefan Frings
*/
#include "menustyle.h"
#include <QFontMetrics>
#include <QApplication>

int MenuStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                                               const QWidget *widget) const
{
    // Get size one em (text height in pixels)
    int em = QApplication::fontMetrics().height();

    // Return the size of menu item icons
    switch (metric)
    {
        case QStyle::PM_SmallIconSize: return 1.5 * em;
        case QStyle::PM_LargeIconSize: return 3 * em;
        default: return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
