/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef UIABOUT_H
#define UIABOUT_H

#include <QWidget>

namespace Ui {
class PageAbout;
}
class PageAbout : public QWidget
{
    Q_OBJECT

public:
    explicit PageAbout(QWidget *parent = nullptr);
    ~PageAbout();

private:
    Ui::PageAbout *ui;
};
#endif // UIABOUT_H
