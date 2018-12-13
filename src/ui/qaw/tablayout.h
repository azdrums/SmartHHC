/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file   tablayout.h
    @author Andrea Zanellato
*/
#ifndef TABLAYOUT_H
#define TABLAYOUT_H

#include <QWidget>
#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>
/**
    TabLayout class
*/
class TabLayout : public QWidget
{
    Q_OBJECT

public:
/**
    Creates an empty TabLayout.
*/
    explicit TabLayout(QWidget *parent = nullptr);
/**
    Destructor.
*/
    virtual ~TabLayout();
/**
    Adds a tab with the given page and label to the tab widget.
    Ownership of page is passed on to the TabLayout.
    Returns the index of the tab in the tab bar.
*/
    int addTab(QWidget *, const QString &);
/**
    This property holds the index position of the current tab page.
    The current index is -1 if there is no current widget.
    By default, this property contains a value of -1 because
    there are initially no tabs in the widget.
*/
    int currentIndex() const;
/**
    Sets the index position of the current tab page.
*/
    void setCurrentIndex(int);
/**
    Returns the current widget, or 0 if there are no child widgets.
*/
    QWidget *currentWidget() const;
/**
    Sets the current widget to be the specified widget.
    The new current widget must already be contained in this stacked widget.
*/
    void setCurrentWidget(QWidget *);
/**
    Returns the tab page at index position index or 0 if the index is out of range.
*/
    QWidget *widget(int) const;
/**
    Returns the index position of the page occupied by the widget w,
    or -1 if the widget cannot be found.
*/
    int indexOf(QWidget *) const;
/**
    This property holds the number of tabs in the tab bar
    By default, this property contains a value of 0.
*/
    int count() const;
/**
    Sets the background color for this view.
*/
    void setBackgroundColor(const QColor &color);
/**
    Defines a new label for the page at position index's tab.
*/
    void setTabText(int, const QString &);
/**
    Sets the text colors for the different states (normal, selected)
    used for the tabs.
*/
    void setTabTextColors(const QColor &normalColor,
                          const QColor &selectedColor);
/**
    Sets the tab indicator's color for the currently selected tab.
*/
    void setSelectedTabIndicatorColor(const QColor &color);

private:
    // Button click slot: updates the current selection and UI.
    void onTabClicked();

    // Updates the stylesheet of the given tab button.
    void updateTabStyleSheet(QToolButton *);

    // Vertical layout of the TabLayout.
    QVBoxLayout *layout = nullptr;

    // Horizontal layout of the TabLayout's header buttons.
    QHBoxLayout *layTabBar = nullptr;

    // Stacked widget for the TabLayout's pages.
    QStackedWidget *stkMain = nullptr;

    // TabLayout's header that stores buttons.
    QWidget *tabBar = nullptr;

    // TabLayout's header stored buttons.
    QList<QToolButton *> tabButtons;

    // Tab related colors.
    QColor _backgroundColor = Qt::black,
           _normalColor     = Qt::GlobalColor::lightGray,
           _selectedColor   = Qt::white,
           _indicatorColor  = QColor("#90ee90"); // lightGreen;
};
#endif // TABLAYOUT_H
