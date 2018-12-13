/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file   spinbox.cpp
    @author Andrea Zanellato
*/
#include "spinbox.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIntValidator>
#include <QLineEdit>
#include <QMouseEvent>
#include <QTimer>
#include <QToolButton>

SpinBox::SpinBox(QWidget *parent) : QWidget(parent)
{
    validator = new QIntValidator(mMin, mMax, txtSpin);
    timer     = new QTimer(this);

    tbnMinus = new QToolButton(this);
    tbnMinus->setText("-");
    tbnMinus->installEventFilter(this);
    tbnMinus->setDisabled(true);

    tbnPlus = new QToolButton(this);
    tbnPlus->setText("+");
    tbnPlus->installEventFilter(this);

    txtSpin = new QLineEdit("0", this);
    txtSpin->setAlignment(Qt::AlignCenter);
    txtSpin->setValidator(validator);

    connect(tbnMinus, &QToolButton::pressed,   this, &SpinBox::onButtonPressed);
    connect(tbnMinus, &QToolButton::released,  this, &SpinBox::onButtonReleased);
    connect(tbnPlus,  &QToolButton::pressed,   this, &SpinBox::onButtonPressed);
    connect(tbnPlus,  &QToolButton::released,  this, &SpinBox::onButtonReleased);
    connect(txtSpin,  &QLineEdit::textChanged, this, &SpinBox::onTextChanged);
    connect(timer,    &QTimer::timeout,        this, &SpinBox::onTimer);

    setOrientationHorizontal();
}
void SpinBox::setMaximum(int max)
{
    if (max == mMax)
    {
        return;
    }
    else if (max < mVal)
    {
        if (max < mMin)
        {
            mMin = max;
        }
        mVal = max;
        txtSpin->setText(QString::number(max));
    }
    mMax = max;
    validator->setTop(max);
}
void SpinBox::setMinimum(int min)
{
    if (min == mMin)
    {
        return;
    }
    else if (min > mVal)
    {
        if (min > mMax)
        {
            mMax = min;
        }
        mVal = min;
        txtSpin->setText(QString::number(min));
    }
    mMin = min;
    validator->setTop(min);
}
void SpinBox::setValue(int val)
{
    if (val < mMin || val > mMax || val == mVal )
        return;

    mVal = val;
    txtSpin->setText(QString::number(val));
}
void SpinBox::onButtonPressed()
{
    timer->start(150);
}
void SpinBox::onButtonReleased()
{
    checkButtonsEnabled();
    emit valueChanged(mVal);
    timer->stop();
}
void SpinBox::onTimer()
{
    if (tbnMinus->isDown() && mVal > mMin)
    {
        mVal--;
        txtSpin->setText(QString::number(mVal));
    }
    else if (tbnPlus->isDown() && mVal < mMax)
    {
        mVal++;
        txtSpin->setText(QString::number(mVal));
    }
}
void SpinBox::onTextChanged(const QString &text)
{
    mVal = text.toInt();
    if (this->isEnabled())
        checkButtonsEnabled();

    emit valueChanged(mVal);
}
void SpinBox::checkButtonsEnabled()
{
    if (mVal == mMin)
        tbnMinus->setEnabled(false);

    else if (mVal == mMax)
        tbnPlus->setEnabled(false);

    else if (!tbnMinus->isEnabled())
        tbnMinus->setEnabled(true);

    else if (!tbnPlus->isEnabled())
        tbnPlus->setEnabled(true);
}
QAbstractSpinBox::ButtonSymbols SpinBox::buttonSymbols() const
{
    return _buttonSymbols;
}
void SpinBox::setButtonSymbols(QAbstractSpinBox::ButtonSymbols bs)
{
    if (bs == _buttonSymbols)
    {
        return;
    }
    else if (bs == QAbstractSpinBox::UpDownArrows)
    {
        if (_orientation == Qt::Horizontal)
        {
            tbnMinus->setArrowType(Qt::LeftArrow);
            tbnPlus->setArrowType(Qt::RightArrow);
        }
        else
        {
            tbnMinus->setArrowType(Qt::DownArrow);
            tbnPlus->setArrowType(Qt::UpArrow);
        }
    }
    else
    {
        tbnMinus->setArrowType(Qt::NoArrow);
        tbnPlus->setArrowType(Qt::NoArrow);
    }
}
QToolButton *SpinBox::getButtonMinus() const
{
    return tbnMinus;
}
QToolButton *SpinBox::getButtonPlus() const
{
    return tbnPlus;
}
/*
void SpinBox::mousePressEvent(QMouseEvent *)
{
    // TODO
}
void SpinBox::mouseReleaseEvent(QMouseEvent *)
{
    // TODO
}
*/
bool SpinBox::eventFilter(QObject *object, QEvent *event)
{
    if (!isEnabled())
        return false;

    if (event->type() == QEvent::MouseButtonPress ||
        event->type() == QEvent::MouseButtonDblClick)
    {
        if (object == tbnMinus && mVal > mMin)
        {
            mVal--;
            txtSpin->setText(QString::number(mVal));
        }
        else if (object == tbnPlus  && mVal < mMax)
        {
            mVal++;
            txtSpin->setText(QString::number(mVal));
        }
/*
        // Propagate mouse events to this parent from children
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        SpinBox::mousePressEvent(mouseEvent);
*/
    }
    else if (event->type() == QEvent::MouseButtonRelease)
    {
/*
        // Propagate mouse events to this parent from children
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        SpinBox::mouseReleaseEvent(mouseEvent);
*/
    }
    return QObject::eventFilter(object, event);
}
Qt::Orientation SpinBox::orientation() const
{
    return _orientation;
}
void SpinBox::setOrientation(Qt::Orientation orientation)
{
    if (orientation == _orientation)
        return;

    else if (orientation == Qt::Vertical)
        setOrientationVertical();

    else
        setOrientationVertical();
}
void SpinBox::setOrientationHorizontal()
{
    if (vLayout)
    {
        vLayout->removeWidget(tbnMinus);
        vLayout->removeWidget(txtSpin);
        vLayout->removeWidget(tbnPlus);
        delete vLayout;
    }
    QSizePolicy szPolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    tbnMinus->setSizePolicy(szPolicy);
    tbnMinus->setMinimumSize(QSize(48, 48));
    tbnMinus->setMaximumSize(QSize(48, 48));
    tbnMinus->setArrowType(Qt::LeftArrow);

    tbnPlus->setSizePolicy(szPolicy);
    tbnPlus->setMinimumSize(QSize(48, 48));
    tbnPlus->setMaximumSize(QSize(48, 48));
    tbnPlus->setArrowType(Qt::RightArrow);

    txtSpin->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    txtSpin->setMinimumSize(QSize(48, 48));
    txtSpin->setMaximumSize(QSize(16777215, 48));

    if (!hLayout)
        hLayout = new QHBoxLayout(this);

    hLayout->setMargin(0);
    hLayout->setSpacing(0);

    hLayout->setSizeConstraint(QLayout::SetMinAndMaxSize);

    hLayout->addWidget(tbnMinus);
    hLayout->addWidget(txtSpin);
    hLayout->addWidget(tbnPlus);

    _orientation = Qt::Horizontal;

    setLayout(hLayout);
}
void SpinBox::setOrientationVertical()
{
    if (hLayout)
    {
        hLayout->removeWidget(tbnMinus);
        hLayout->removeWidget(txtSpin);
        hLayout->removeWidget(tbnPlus);
        delete hLayout;
    }
    QSizePolicy szPolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    tbnMinus->setSizePolicy(szPolicy);
    tbnMinus->setMinimumSize(QSize(48, 48));
    tbnMinus->setMaximumSize(QSize(16777215, 48));
    tbnMinus->setArrowType(Qt::DownArrow);

    tbnPlus->setSizePolicy(szPolicy);
    tbnPlus->setMinimumSize(QSize(48, 48));
    tbnPlus->setMaximumSize(QSize(16777215, 48));
    tbnPlus->setArrowType(Qt::UpArrow);

    txtSpin->setSizePolicy(szPolicy);
    txtSpin->setMinimumSize(QSize(48, 48));
    txtSpin->setMaximumSize(QSize(16777215, 48));

    if (!vLayout)
        vLayout = new QVBoxLayout(this);

    vLayout->setMargin(0);
    vLayout->setSpacing(0);

    vLayout->setSizeConstraint(QLayout::SetNoConstraint);

    vLayout->addWidget(tbnPlus);
    vLayout->addWidget(txtSpin);
    vLayout->addWidget(tbnMinus);

    _orientation = Qt::Vertical;

    setLayout(vLayout);
}
