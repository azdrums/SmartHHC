/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QMenu>
#include <QToolButton>

#include "CurveQToolBar.h"
#include "spinbox.h"

CurveToolBar::CurveToolBar(QWidget *parent)
:
    QToolBar(parent),
    tbnSlot(new QToolButton(this)),
    tbnCancel(new QToolButton(this)),
    tbnOk(new QToolButton(this)),
    tbnRestore(new QToolButton(this)),
    tbnSave(new QToolButton(this)),
    sbx(new SpinBox(this)),
    mnuDropDown(new QMenu(tbnSlot))
{
    mnuDropDown->setObjectName("mnuDropDown");
    mnuDropDown->addAction(tr("Stored"));
    mnuDropDown->addAction(tr("Slot 1"));
    mnuDropDown->addAction(tr("Slot 2"));
    mnuDropDown->addAction(tr("Slot 3"));

    QFont font;
    font.setFamily("FontAwesome");
    font.setPixelSize(24);

    tbnSlot->setObjectName("tbnSlot");
    tbnSlot->setMenu(mnuDropDown);
    tbnSlot->setFont(font);
    tbnSlot->setMinimumSize(48, 48);
    tbnSlot->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnSlot->setToolButtonStyle(Qt::ToolButtonTextOnly);
    tbnSlot->setPopupMode(QToolButton::InstantPopup);
    addWidget(tbnSlot);

    tbnCancel->setObjectName("tbnCancel");
    tbnCancel->setFont(font);
    tbnCancel->setText("\uf00d");
    tbnCancel->setMinimumSize(48, 48);
    tbnCancel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnCancel->setToolButtonStyle(Qt::ToolButtonTextOnly);
    addWidget(tbnCancel);

    tbnOk->setObjectName("tbnOk");
    tbnOk->setFont(font);
    tbnOk->setText("\uf00c");
    tbnOk->setMinimumSize(48, 48);
    tbnOk->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnOk->setToolButtonStyle(Qt::ToolButtonTextOnly);
    addWidget(tbnOk);

    tbnRestore->setObjectName("tbnRestore");
    tbnRestore->setFont(font);
    tbnRestore->setText("\uf0e2");
    tbnRestore->setMinimumSize(48, 48);
    tbnRestore->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnRestore->setToolButtonStyle(Qt::ToolButtonTextOnly);
    addWidget(tbnRestore);

    tbnSave->setObjectName("tbnSave");
    tbnSave->setFont(font);
    tbnSave->setText("\uf019"); // f2db
    tbnSave->setMinimumSize(48, 48);
    tbnSave->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    tbnSave->setToolButtonStyle(Qt::ToolButtonTextOnly);
    addWidget(tbnSave);

    sbx->setObjectName("sbx");
    sbx->setMinimumSize(0, 48);
    sbx->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sbx->setMaximum(127);
    addWidget(sbx);

    spacer = new QWidget(this);
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    addWidget(spacer);

    onActionChanged(mnuDropDown->actions().at(0));
    setSetupChanged(false);
    hideSpinBox();

    connect(tbnOk,      &QToolButton::clicked, this, &CurveToolBar::onOkClicked);
    connect(tbnCancel,  &QToolButton::clicked, this, &CurveToolBar::onCancelClicked);
    connect(tbnRestore, &QToolButton::clicked, this, &CurveToolBar::onRestoreClicked);
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
void CurveToolBar::hideSpinBox()
{
    actions().at(5)->setVisible(false);
    actions().at(6)->setVisible(true);
}
void CurveToolBar::showSpinBox()
{
    actions().at(5)->setVisible(true);
    actions().at(6)->setVisible(false);
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
QToolButton *CurveToolBar::buttonRestore() const
{
    return tbnRestore;
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
    if (index > 0)
    {
        tbnSlot->setText(QString::number(index));
    }
    else
    {
        tbnSlot   ->setText("\uf093");
        tbnOk     ->setEnabled(false);
        tbnCancel ->setEnabled(false);
        tbnRestore->setEnabled(false);
        tbnSave   ->setEnabled(false);
        hideSpinBox();
    }
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
void CurveToolBar::onRestoreClicked()
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
    tbnCancel ->setEnabled(changed);
    tbnRestore->setEnabled(changed);
    tbnOk     ->setEnabled(changed);
    tbnSave   ->setEnabled(changed);
}
void CurveToolBar::setSetupUnsaved()
{
    tbnCancel ->setEnabled(false);
    tbnOk     ->setEnabled(false);
    tbnRestore->setEnabled(true);
    tbnSave   ->setEnabled(true);
}
