/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file   tablayout.cpp
    @author Andrea Zanellato
*/
#include "tablayout.h"

TabLayout::TabLayout(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    setLayout(layout);

    tabBar = new QWidget(this);
    tabBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tabBar->setMinimumHeight(48);
    layout->addWidget(tabBar);

    layTabBar = new QHBoxLayout(tabBar);
    layTabBar->setSpacing(0);
    layTabBar->setMargin(0);
    layTabBar->setContentsMargins(0, 0, 0, 0);
    layTabBar->setSizeConstraint(QLayout::SetNoConstraint);
    tabBar->setLayout(layTabBar);

    stkMain = new QStackedWidget(this);
    stkMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(stkMain);

    setBackgroundColor(_backgroundColor);
}
TabLayout::~TabLayout()
{
}
int TabLayout::addTab(QWidget *widget, const QString &label)
{
    if (!widget)
        return -1;

    QToolButton *tab = new QToolButton(tabBar);
    tab->setText(label);
    tab->setAutoRaise(true);
    tab->setFocusPolicy(Qt::NoFocus);
    tab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(tab, &QToolButton::clicked, this, &TabLayout::onTabClicked);

    tabButtons.append(tab);
    layTabBar->addWidget(tab);

    int i = stkMain->addWidget(widget);
    updateTabStyleSheet(tab);

    return i;
}
int TabLayout::currentIndex() const
{
    return stkMain->currentIndex();
}
void TabLayout::setCurrentIndex(int index)
{
    if (index >= tabButtons.size())
        return;

    stkMain->setCurrentIndex(index);

    for (int i = 0; i < tabButtons.size(); i++)
        updateTabStyleSheet(tabButtons.at(i));
}
QWidget *TabLayout::currentWidget() const
{
    return stkMain->currentWidget();
}
void TabLayout::setCurrentWidget(QWidget *widget)
{
    stkMain->setCurrentWidget(widget);
}
QWidget *TabLayout::widget(int index) const
{
    return stkMain->widget(index);
}
int TabLayout::indexOf(QWidget *w) const
{
    return stkMain->indexOf(w);
}
int TabLayout::count() const
{
    return stkMain->count();
}
void TabLayout::setBackgroundColor(const QColor &color)
{
    _backgroundColor = color;

    tabBar->setStyleSheet(QString("* {background-color: rgb(%1, %2, %3);"
                                  "border: none}")
                                  .arg(_backgroundColor.red())
                                  .arg(_backgroundColor.green())
                                  .arg(_backgroundColor.blue()));
}
void TabLayout::setTabText(int index, const QString &label)
{
    if (index < 0 || index > stkMain->count() - 1)
        return;

    tabButtons[index]->setText(label);
}
void TabLayout::setTabTextColors(const QColor &normalColor,
                                 const QColor &selectedColor)
{
    _normalColor   = normalColor;
    _selectedColor = selectedColor;
    int index;

    foreach (QToolButton *tab, tabButtons)
    {
        index = tabButtons.indexOf(tab);
        updateTabStyleSheet(tabButtons.at(index));
    }
}
void TabLayout::setSelectedTabIndicatorColor(const QColor &color)
{
    _indicatorColor = color;

    int currentIndex = stkMain->currentIndex();
    if (currentIndex < 0)
        return;

    updateTabStyleSheet(tabButtons[currentIndex]);
}
void TabLayout::onTabClicked()
{
    QToolButton *clicked = qobject_cast<QToolButton *>(sender());
    int index = tabButtons.indexOf(clicked);
    if (index == stkMain->currentIndex())
        return;

    stkMain->setCurrentIndex(index);

    foreach (QToolButton *tab, tabButtons)
    {
        index = tabButtons.indexOf(tab);
        updateTabStyleSheet(tabButtons.at(index));
    }
}
void TabLayout::updateTabStyleSheet(QToolButton *tab)
{
    if (!tab)
        return;

    if (tabButtons.indexOf(tab) == stkMain->currentIndex())
        tab->setStyleSheet(QString("* {background-color: rgb(%1, %2, %3);"
                                      "color: rgb(%4, %5, %6);"
                                      "border: none;"
                                      "border-bottom: 4px solid rgb(%7, %8, %9);"
                                      "padding: 4px}")
                                   .arg(_backgroundColor.red())
                                   .arg(_backgroundColor.green())
                                   .arg(_backgroundColor.blue())
                                   .arg(_selectedColor.red())
                                   .arg(_selectedColor.green())
                                   .arg(_selectedColor.blue())
                                   .arg(_indicatorColor.red())
                                   .arg(_indicatorColor.green())
                                   .arg(_indicatorColor.blue()));
    else
        tab->setStyleSheet(QString("* {background-color: rgb(%1, %2, %3);"
                                      "color: rgb(%4, %5, %6);"
                                      "border: none}")
                                   .arg(_backgroundColor.red())
                                   .arg(_backgroundColor.green())
                                   .arg(_backgroundColor.blue())
                                   .arg(_normalColor.red())
                                   .arg(_normalColor.green())
                                   .arg(_normalColor.blue()));
}
