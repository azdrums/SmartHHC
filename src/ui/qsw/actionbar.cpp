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
    @file    actionbar.cpp
    @authors Stefan Frings, Andrea Zanellato
*/
#include "actionbar.h"
#include "utils.h"
#include <QIcon>
#include <QFontMetrics>
#include <QFont>
#include <QApplication>
#include <QStyleOption>
#include <QPainter>
#include <QBitmap>

ActionBar::ActionBar(QWidget *parent)
:
    QWidget(parent),
    layout(new QHBoxLayout(this)),
    tbnNavigation(new QToolButton(this)),
    tbnTitle(new QToolButton(this)),
    tbnOverflow(new QToolButton(this)),
    mnuNavigation(new QMenu(tbnNavigation)),
    mnuOverflow(new QMenu(tbnOverflow))
{
    // Create layout
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizeConstraint(QLayout::SetNoConstraint);

    // App Icon and Up Button
    tbnNavigation->setIcon(icoNavigation);
    tbnNavigation->setAutoRaise(true);
    tbnNavigation->setFocusPolicy(Qt::NoFocus);
    tbnNavigation->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnNavigation->setPopupMode(QToolButton::InstantPopup);
    tbnNavigation->setMenu(mnuNavigation);
    layout->addWidget(tbnNavigation);

    // View Control Button
    tbnTitle->setObjectName("tbnTitle");
    tbnTitle->setText(QApplication::applicationDisplayName());
    tbnTitle->setAutoRaise(true);
    tbnTitle->setFocusPolicy(Qt::NoFocus);
    tbnTitle->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    layout->addWidget(tbnTitle);

    // Spacer
    layout->addStretch();

    // Action Overflow Button
    tbnOverflow->setObjectName("tbnOverflow");
    tbnOverflow->setIcon(icoOverflow);
    tbnOverflow->setAutoRaise(true);
    tbnOverflow->setFocusPolicy(Qt::NoFocus);
    tbnOverflow->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnOverflow->setPopupMode(QToolButton::InstantPopup);
    tbnOverflow->setMenu(mnuOverflow);
    layout->addWidget(tbnOverflow);

    // needed because the icon size cannot be set by a StyleSheet
    mnuOverflow->setStyle(&menuStyle);
    mnuNavigation->setStyle(&menuStyle);
    mnuOverflow->installEventFilter(this);
    mnuOverflow->setObjectName("mnuOverflow");

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);



    updateStyleSheet();
}
ActionBar::~ActionBar()
{
}
bool ActionBar::eventFilter(QObject *object, QEvent *event)
{
//  TODO: Update code for resized buttons (see below)
    if (event->type() == QEvent::Show && object == mnuOverflow)
    {
        QRect rct = rect();

        rct.moveTopRight(this->mapToGlobal(rct.topRight()));
        mnuOverflow->move(rct.right() - mnuOverflow->rect().right(),
                          rct.top() + rct.height());
        return true;
    }
    return false;
}
void ActionBar::paintEvent(QPaintEvent *)
{
     QPainter painter(this);
     QStyleOption option;
     option.initFrom(this);
     style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);
}
void ActionBar::resizeEvent(QResizeEvent *event)
{
    if (event->oldSize().width() != event->size().width() && !isShowHomeAsUp)
    {
    //  TODO: Resize buttons
        adjustContent();
    }
}
void ActionBar::setTitle(const QString &title)
{
    tbnTitle->setText(title);
    if (!isShowHomeAsUp)
        adjustContent();
}
QString ActionBar::title() const
{
    return tbnTitle->text();
}
void ActionBar::setAppIcon(const QIcon &icon)
{
    icoNavigation = icon;
    updateStyleSheet();
}
void ActionBar::setOverflowIcon(const QIcon &icon)
{
    icoOverflow = icon;
    updateStyleSheet();
}
QAction *ActionBar::addNavigation(const QString &text)
{
    return addAction(QIcon(), text, Action::Navigation, Action::Home);
}
QAction *ActionBar::addAction(const QString &text)
{
    QAction     *action = new QAction(tr(qPrintable(text)), this);
    QToolButton *button = new QToolButton(this);
    ActionInfoPtr info(new ActionInfo(action, button));

    button->setText(action->text());
    button->setFocusPolicy(Qt::NoFocus);
    button->setAutoRaise(true);
    lstActions.append(info);
    int index = lstActions.size() + 2;
    layout->insertWidget(index, button);
    connect(action, &QAction::changed, this, &ActionBar::onActionChanged);
    connect(button, &QToolButton::clicked, action, &QAction::trigger);
    return action;
}
QAction *ActionBar::addAction(const QIcon &icon, const QString &text)
{
    return addAction(icon, text, Action::Normal, Action::Home);
}
QAction *ActionBar::addAction(const QIcon &icon, const QString &text,
                              Action::Type type, Action::Scope scope)
{
    QAction *action = new QAction(icon, tr(qPrintable(text)), this);
    addAction(action, type, scope);
    return action;
}
void ActionBar::addAction(QAction *action, Action::Type type, Action::Scope scope)
{
    QToolButton *button = nullptr;

    if (type == Action::Navigation)
    {
        QWidget::addAction(action);
        mnuNavigation->addAction(action);
        return;
    }
    else if (type == Action::Normal)
    {
        button = new QToolButton(this);
        button->setIcon(action->icon());
        button->setText(action->text());
        button->setToolTip(action->toolTip());
        button->setShortcut(action->shortcut());
        button->setFocusPolicy(Qt::NoFocus);
        button->setAutoRaise(true);
        connect(action, &QAction::changed, this, &ActionBar::onActionChanged);
        connect(button, &QToolButton::clicked, action, &QAction::trigger);
    }
//  else if (type == Action::Overflow) does nothing

    ActionInfoPtr info(new ActionInfo(action, button, type, scope));
    lstActions.append(info);
    int index = lstActions.size() + 2;
    layout->insertWidget(index, button);
}
void ActionBar::setDisplayHomeAsUpEnabled(bool showHomeAsUp)
{
    if (showHomeAsUp)
    {
        for (int i = 0; i < lstActions.size(); i++)
        {
            QToolButton *button = lstActions.at(i)->button();
            if (button)
                button->hide();
        }
        tbnOverflow->hide();
        tbnNavigation->setMenu(nullptr);
        tbnNavigation->setIcon(icoAppUp);
        connect(tbnNavigation, &QToolButton::clicked,
                         this, &ActionBar::onNavButtonClicked);
    } else {
        tbnOverflow->show();
        tbnNavigation->setMenu(mnuNavigation);
        tbnNavigation->setIcon(icoNavigation);
        disconnect(tbnNavigation, &QToolButton::clicked,
                            this, &ActionBar::onNavButtonClicked);
        adjustContent();
    }
    isShowHomeAsUp = showHomeAsUp;
}
void ActionBar::updateStyleSheet()
{
//  TODO: For some reason it doesn't works if the stylesheet is not reset
    this->setStyleSheet(QString());

    QColor disabled = palette().color(QPalette::Disabled, QPalette::ButtonText);

//  TODO: This works only if I write: ":disabled {color: 1px solid rgb(%7, %8, %9)}",
//  should be ":disabled {color: rgb(%7, %8, %9)}"
    QString styleSheet =
        QString("QWidget {background-color: rgb(%1, %2, %3);"
                              "color: rgb(%4, %5, %6)}"
                "QToolButton {min-width: 2em; min-height: 2em;"
                             "border: none}"
                "QToolButton:disabled {color: 1px solid rgb(%7, %8, %9)}"
                "QToolButton::menu-indicator {image: none}"
                "QToolButton#tbnTitle {font: bold}"
                "QMenu:disabled {color: 1px solid rgb(%7, %8, %9)}")
                .arg(_backgroundColor.red())
                .arg(_backgroundColor.green())
                .arg(_backgroundColor.blue())
                .arg(_normalColor.red())
                .arg(_normalColor.green())
                .arg(_normalColor.blue())
                .arg(disabled.red())
                .arg(disabled.green())
                .arg(disabled.blue());

    this->setStyleSheet(styleSheet);

    icoNavigation = qsw::color::changeIconColor(icoNavigation, iconSize, _normalColor);
    icoOverflow   = qsw::color::changeIconColor(icoOverflow, iconSize, _normalColor);
    icoAppUp      = qsw::color::changeIconColor(icoAppUp, iconSize, _normalColor);

    tbnNavigation->setIcon(icoNavigation);
    tbnOverflow->setIcon(icoOverflow);
}
void ActionBar::setBackgroundColor(const QColor &color)
{
    _backgroundColor = color;
    updateStyleSheet();
}
void ActionBar::setForegroundColor(const QColor &color)
{
    _normalColor = color;
    updateStyleSheet();
}
void ActionBar::showOverflowMenu()
{
    if (tbnOverflow->isVisible())
        tbnOverflow->click();
}
/*
void ActionBar::initResources()
{
    Q_INIT_RESOURCE(actionbar);
}
*/
void ActionBar::adjustContent()
{
    // Get size of one em (text height in pixels)
    int em = fontMetrics().height();

    mnuNavigation->repaint();

    // update size of app icon and overflow menu button
    tbnNavigation->setIconSize(QSize(1.2 * em, 1.2 * em));
    tbnOverflow->setIconSize(QSize(1.2 * em, 1.2 * em));

    // Check if all action buttons fit into the available space with text beside icon.
    bool needOverflow = false;
    int space = width() - tbnNavigation->sizeHint().width()
                        - tbnTitle->sizeHint().width();

    for (int i = 0; i < lstActions.size(); i++)
    {
        QAction     *action = lstActions.at(i)->action();
        QToolButton *button = lstActions.at(i)->button();

        if (action->isVisible() &&
            lstActions.at(i)->type() != Action::OverflowOnly)
        {
            button->setIconSize(QSize(1.2 * em, 1.2 * em));
            button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
            space -= button->sizeHint().width();
        }
    }
    if (space < 0)
    {
        // Not enough space.
        // Check if all action buttons fit into the available space without text.
        int space = width() - tbnNavigation->sizeHint().width()
                            - tbnTitle->sizeHint().width();

        for (int i = 0; i < lstActions.size(); i++)
        {
            QAction     *action = lstActions.at(i)->action();
            QToolButton *button = lstActions.at(i)->button();

            if (action->isVisible() &&
                lstActions.at(i)->type() != Action::OverflowOnly)
            {
                button->setToolButtonStyle(Qt::ToolButtonIconOnly);
                space -= button->sizeHint().width();
            }
        }
        if (space < 0)
        {
            // The buttons still don't fit, we need an overflow menu.
            needOverflow = true;
        }
    }
    // Calculate space available to display action buttons
    mnuOverflow->clear();

    space = width() - tbnNavigation->sizeHint().width()
                    - tbnTitle->sizeHint().width();
    if (needOverflow)
        space -= tbnOverflow->sizeHint().width();

    // Show/Hide action buttons and overflow menu entries
    for (int i = 0; i < lstActions.size(); i++)
    {
        QAction     *action = lstActions.at(i)->action();
        QToolButton *button = lstActions.at(i)->button();

        if (action->isVisible() &&
            lstActions.at(i)->type() != Action::OverflowOnly)
        {
            space -= button->sizeHint().width();
            if (space >= 0)
            {
                // show as button
                button->setDisabled(!lstActions.at(i)->action()->isEnabled());
                button->show();
            }
            else
            {
                // show as overflow menu entry
                button->hide();
                mnuOverflow->addAction(action);
            }
        }
    }
    // Add OverflowOnly actions if any
    for (int i = 0; i < lstActions.size(); i++)
    {
        if (lstActions.at(i)->type() == Action::OverflowOnly)
        {
            needOverflow = true;
            mnuOverflow->addAction(lstActions.at(i)->action());
        }
    }
    // Show/Hide the overflow menu button
    if (needOverflow)
        tbnOverflow->show();
    else
        tbnOverflow->hide();
}
void ActionBar::onNavButtonClicked()
{
    emit up();
}
void ActionBar::onActionChanged()
{
    for (int i = 0; i < lstActions.size(); i++)
    {
        if (lstActions.at(i)->action() == sender())
        {
            QAction     *action = lstActions.at(i)->action();
            QToolButton *button = lstActions.at(i)->button();

            if (!button)
                return;

            // TODO: autoRepeat, checkable, font, iconText, iconVisibleInMenu,
            // menuRole, shortcut, shortcutContext, shortcutVisibleInContextMenu,
            // statusTip, toolTip, visible, whatsThis?
            button->setEnabled(action->isEnabled());
            button->setIcon(action->icon());
            button->setText(action->text());
            return;
        }
    }
}
