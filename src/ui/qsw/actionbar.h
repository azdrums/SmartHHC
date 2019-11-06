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
    @file    actionbar.h
    @authors Stefan Frings, Andrea Zanellato
*/
#ifndef ACTIONBAR_H
#define ACTIONBAR_H

#include "menustyle.h"
#include <QWidget>
#include <QList>
#include <QAction>
#include <QMenu>
#include <QHBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSharedPointer>
#include <QToolButton>
#include <QProxyStyle>

/**
    Type and Scope enums.
*/
namespace Action
{
/**
    Whether this action is displayed in both the ActionBar and in the overflow,
    in the overflow menu only or in the navigation menu.
*/
    enum Type
    {
        Normal = 0,
        OverflowOnly = 1,
        Navigation = 2
    };
/**
    Whether this action is in the Home scope or a different page.
    In a different page the up button is used to go back to the home page.
*/
    enum Scope
    {
        Home = 0,
        Other = 1
    };
}
/**
    Toolbar similar to Android's ActionBar, can also be used on Desktop OS.
    The ActionBar shows an icon, a title and any number of action buttons.
    Also the title can have a menu of navigation actions.
    <p>
    If the buttons do not fit into the window, then they are displayed
    as an "overflow" menu on the right side.
    <p>
    See https://developer.android.com/reference/android/support/v7/app/ActionBar.html

    To be used within a vertical box layout this way:
    <pre><code>
    MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setMargin(0);
        layout->setSizeConstraint(QLayout::SetNoConstraint);

        // ActionBar
        ActionBar *actionBar = new ActionBar(this);
        actionBar->setTitle("My App");
        actionBar->addNavigation("News");
        actionBar->addNavigation("Weather");
        actionBar->addAction(QIcon(":/icons/search"), "Search");
        actionBar->addAction(QIcon(":/icons/call"), "Call");
        actionBar->addAction(QIcon(":/icons/settings"), "Settings");
        layout->addWidget(actionBar);

        // Content of main window below the action bar
        layout->addWidget(new QLabel("Hello",this));
        layout->addStretch();
    }
    </code></pre>
    The layout of the main window must use the option QLayout::SetNoConstraint,
    to support screen rotation on mobile devices.
    <p>
    The action bar needs two icons in the resource file:
    <ul>
        <li>QIcon(":icons/app") is used for the initial display.</li>
        <li>QIcon(":icons/app_up") is used when you enable "up" navigation by
            calling setTitle().</li>
    </ul>
*/
class ActionBar : public QWidget
{
    Q_OBJECT

public:
/**
    Creates an Activity bar with icons and title without menu actions.
    The icons are loaded from the resource file.
    The title is taken from QApplication::applicationDisplayName().
*/
    explicit ActionBar(QWidget *parent = nullptr);
/**
    Destructor.
*/
    virtual ~ActionBar();
/**
    Set title of the action bar.
*/
    void setTitle(const QString &);
/**
    Returns the title of the action bar.
*/
    QString title() const;
/**
    Sets the main menu.
*/
    void setAppMenu(QMenu *);
/**
    Sets the overflow menu.
*/
    void setOverflowMenu(QMenu *);
/**
    Sets the main button menu icon.
*/
    void setAppIcon(const QIcon &);
/**
    Sets the overflow button menu icon.
*/
    void setOverflowIcon(const QIcon &);
/**
    Adds a navigation menu item link and, optionally, an icon to the
    ActionBar navigation menu.
*/
    QAction *addNavigation(const QString &);
    using QWidget::addAction;
/**
    This is an overloaded function.

    Creates a new action with the given text.
    This action is added to the end of the ActionBar.
*/
    QAction *addAction(const QString &);
/**
    This is an overloaded function.

    Creates a new action with the given icon and text.
    This action is added to the end of the ActionBar.
*/
    QAction *addAction(const QIcon &, const QString &);
/**
    This is an overloaded function.

    Creates a new action with the given icon, text, type and scope.
    This action is added to the end of the ActionBar.
*/
    QAction *addAction(const QIcon &, const QString &, Action::Type, Action::Scope);
/**
    Adds a given action to the ActionBar.
    This action is added to the end of the ActionBar.
*/
    void addAction(QAction *action, Action::Type type = Action::Normal,
                                    Action::Scope scope = Action::Home);
/**
    Set whether home should be displayed as an "up" affordance.
*/
    void setDisplayHomeAsUpEnabled(bool showHomeAsUp = true);
/**
    Sets the background color.
*/
    void setBackgroundColor(const QColor &);
/**
    Sets the text color.
*/
    void setForegroundColor(const QColor &);
/**
    Show the overflow items from the associated menu.
*/
    void showOverflowMenu();
/**
    Initialize external library resources.
*/
//  static void initResources();

signals:
/**
    Emitted when the user clicks on the app icon (for "up" navigation)
*/
    void up();

protected:
/**
    Adjust the number of buttons in the action bar. Buttons that don't fit go
    into the overflow menu.
    You need to call this method after adding, removing or changing the
    visibility of an action.
*/
    virtual void adjustContent();
/**
    Used to right align the overflow menu to the overflow button.
*/
    virtual bool eventFilter(QObject *, QEvent *);
/**
    Overrides the paintEvent method to draw background color properly.
*/
    virtual void paintEvent(QPaintEvent *);
/**
    Overrides the resizeEvent to adjust the content depending on the new size.
*/
    virtual void resizeEvent(QResizeEvent *);

private:
/**
    ActionBar Action descriptor.
*/
    class ActionInfo
    {
    public:
    /**
        Construct an ActionInfo instance, giving the specified action, button,
        type and scope.
    */
        explicit ActionInfo(QAction *action, QToolButton *button,
                            Action::Type  type  = Action::Normal,
                            Action::Scope scope = Action::Home)
        {
            m_action = action;
            m_button = button;
            m_type   = type;
            m_scope  = scope;
        }
    /**
        Returns the action pointer of this descriptor.
    */
        QAction *action() const {return m_action;}
    /**
        Returns the button pointer of this descriptor.
    */
        QToolButton *button() const {return m_button;}
    /**
        Returns the type of the action in this descriptor.
    */
        Action::Type type() const {return m_type;}
    /**
        Returns the scope of the action in this descriptor.
    */
        Action::Scope scope() const {return m_scope;}
    /**
        Sets the type of the action in this descriptor.
    */
        void setType(Action::Type type) {m_type = type;}
    /**
        Sets the scope of the action in this descriptor.
    */
        void setScope(Action::Scope scope) {m_scope = scope;}

    private:
        QAction      *m_action = nullptr;
        QToolButton  *m_button = nullptr;
        Action::Type  m_type   = Action::Normal;
        Action::Scope m_scope  = Action::Home;
    };
    typedef QSharedPointer<ActionInfo> ActionInfoPtr;

    // Updates the ActionBar's stylesheet.
    void updateStyleSheet();

    // Internally used to forward events from the tbnNavigation button.
    void onNavButtonClicked();

    // Internally used to update buttons on QAction::changed()
    void onActionChanged();

    // Horizontal layout of the navigation bar
    QHBoxLayout *layout;

    // The Button that contains the applications icon, used for "up" navigation.
    QToolButton *tbnNavigation;

    // The button that contains the title, used for view navigation.
    QToolButton *tbnTitle;

    // Overflow button, is only visible when there are more buttons than
    // available space.
    QToolButton *tbnOverflow;

    // The menu that appears when the user clicks on the title.
    QMenu *mnuNavigation;

    // The menu that appears when the user clicks on the overflow button.
    QMenu *mnuOverflow;

    // List of actions for the action buttons and overflow menu.
    QList<ActionInfoPtr> lstActions;

    // Used to control the size of icons in menu items.
    MenuStyle menuStyle;

    // Whether the navigation button icon is shown as a left arrow
    // to back on the main Home page.
    bool   isShowHomeAsUp  = false;

    // Tab colors.
    QColor _backgroundColor = Qt::black,
           _normalColor     = Qt::white;

    // Menu button icons and related size.
    QIcon  icoNavigation = QIcon(":/icons/app.png"),
           icoAppUp      = QIcon(":/icons/app_up.png"),
           icoOverflow   = QIcon(":/icons/overflow.png");
    QSize  iconSize      = QSize(192, 192);
};

#endif // ACTIONBAR_H
