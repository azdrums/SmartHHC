/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
