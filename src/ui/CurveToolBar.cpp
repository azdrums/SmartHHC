/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QHBoxLayout>
#include <QMenu>
#include <QToolButton>

#include "CurveToolBar.h"
#include "spinbox.h"

CurveToolBar::CurveToolBar(QWidget *parent)
:
    QWidget(parent),
    layout(new QHBoxLayout(this)),
    tbnSlot(new QToolButton(this)),
    tbnCancel(new QToolButton(this)),
    tbnOk(new QToolButton(this)),
    tbnSave(new QToolButton(this)),
    sbx(new SpinBox(this)),
    mnuDropDown(new QMenu(tbnSlot))
{
    layout->setObjectName("layout");
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSizeConstraint(QLayout::SetNoConstraint);

    mnuDropDown->setObjectName("mnuDropDown");
    mnuDropDown->addAction(tr("Stored"));
    mnuDropDown->addAction(tr("Slot 1"));
    mnuDropDown->addAction(tr("Slot 2"));
    mnuDropDown->addAction(tr("Slot 3"));

    QFont font;
    font.setPixelSize(24);

    tbnSlot->setFont(font);
    tbnSlot->setObjectName("tbnSlot");
    tbnSlot->setMenu(mnuDropDown);
    tbnSlot->setIcon(QIcon(":/icons/microchip.png"));
    tbnSlot->setIconSize(QSize(24, 24));
    tbnSlot->setMinimumSize(48, 48);
    tbnSlot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnSlot->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tbnSlot->setPopupMode(QToolButton::InstantPopup);
    tbnSlot->setAutoRaise(true);
    tbnSlot->setEnabled(false);
    layout->addWidget(tbnSlot);

    tbnCancel->setObjectName("tbnCancel");
    tbnCancel->setIcon(QIcon(":/icons/cancel.png"));
    tbnCancel->setIconSize(QSize(24, 24));
    tbnCancel->setMinimumSize(48, 48);
    tbnCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnCancel->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tbnCancel->setAutoRaise(true);
    layout->addWidget(tbnCancel);

    tbnOk->setObjectName("tbnOk");
    tbnOk->setIcon(QIcon(":/icons/check.png"));
    tbnOk->setIconSize(QSize(24, 24));
    tbnOk->setMinimumSize(48, 48);
    tbnOk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnOk->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tbnOk->setAutoRaise(true);
    layout->addWidget(tbnOk);

    tbnSave->setObjectName("tbnSave");
    tbnSave->setIcon(QIcon(":/icons/savechip.png"));
    tbnSave->setIconSize(QSize(24, 24));
    tbnSave->setMinimumSize(48, 48);
    tbnSave->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnSave->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tbnSave->setAutoRaise(true);
    layout->addWidget(tbnSave);

    sbx->setObjectName("sbx");
    sbx->setMinimumSize(0, 48);
    sbx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sbx->setMaximum(127);
    layout->addWidget(sbx);

    onActionChanged(mnuDropDown->actions().at(0));
    setSetupChanged(false);
    hideSpinBox();

#ifdef Q_OS_ANDROID
    QColor enabled  = palette().color(QPalette::Active,   QPalette::ButtonText);
    QColor disabled = palette().color(QPalette::Disabled, QPalette::ButtonText);
    setStyleSheet(QString("QToolButton:enabled  {color: rgb(%1, %2, %3)}"
                          "QToolButton:disabled {color: rgb(%4, %5, %6)}")
                          .arg(enabled.red())
                          .arg(enabled.green())
                          .arg(enabled.blue())
                          .arg(disabled.red())
                          .arg(disabled.green())
                          .arg(disabled.blue()));
#endif
    connect(tbnOk,      &QToolButton::clicked, this, &CurveToolBar::onOkClicked);
    connect(tbnCancel,  &QToolButton::clicked, this, &CurveToolBar::onCancelClicked);
    connect(tbnSave,    &QToolButton::clicked, this, &CurveToolBar::onSaveClicked);

    connect(sbx, &SpinBox::valueChanged, [=](int)
    {
        if (!isSetupChanged())
            setSetupChanged(true);
    });
    connect(mnuDropDown, &QMenu::triggered, this, &CurveToolBar::onActionChanged);
}
CurveToolBar::~CurveToolBar()
{
}
int CurveToolBar::slotCount() const
{
    return mnuDropDown->actions().size();
}
void CurveToolBar::setCurrentSlotIndex(int index)
{
    if (index)
    {
        tbnSlot->setToolButtonStyle(Qt::ToolButtonTextOnly);
        tbnSlot->setText(QString::number(index));
    }
    else
    {
        tbnSlot->setToolButtonStyle(Qt::ToolButtonIconOnly);
        tbnOk     ->setEnabled(false);
        tbnCancel ->setEnabled(false);
        tbnSave   ->setEnabled(false);
        hideSpinBox();
    }
}
void CurveToolBar::hideSpinBox()
{
    sbx->setVisible(false);
    layout->addStretch();
}
void CurveToolBar::showSpinBox()
{
    sbx->setVisible(true);
}
bool CurveToolBar::isSpinBoxHidden() const
{
    return sbx->isHidden();
}
QToolButton *CurveToolBar::buttonSlot() const
{
    return tbnSlot;
}
QToolButton *CurveToolBar::buttonCancel() const
{
    return tbnCancel;
}
QToolButton *CurveToolBar::buttonOk() const
{
    return tbnOk;
}
QToolButton *CurveToolBar::buttonSave() const
{
    return tbnSave;
}
QMenu *CurveToolBar::dropDownMenu() const
{
    return mnuDropDown;
}
SpinBox *CurveToolBar::spinBox() const
{
    return sbx;
}
void CurveToolBar::onActionChanged(QAction *action)
{
    int index = mnuDropDown->actions().indexOf(action);
    setCurrentSlotIndex(index);

    emit sigSlotButtonMenuItemChanged(index);
}
void CurveToolBar::onOkClicked()
{
    setSetupUnsaved();
    hideSpinBox();
}
void CurveToolBar::onCancelClicked()
{
    setSetupChanged(false);
    hideSpinBox();
}
void CurveToolBar::onSaveClicked()
{
    setSetupChanged(false);
    hideSpinBox();
}
bool CurveToolBar::isSetupChanged()
{
    return tbnCancel->isEnabled();
}
void CurveToolBar::setSetupChanged(bool changed)
{
    tbnSlot   ->setEnabled(!changed);
    tbnCancel ->setEnabled(changed);
    tbnOk     ->setEnabled(changed);
    tbnSave   ->setEnabled(changed);
}
void CurveToolBar::setSetupUnsaved()
{
    tbnCancel ->setEnabled(false);
    tbnOk     ->setEnabled(false);
    tbnSave   ->setEnabled(true);
}
