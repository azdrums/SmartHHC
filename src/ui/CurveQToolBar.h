/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef TOOLBARCURVE_H
#define TOOLBARCURVE_H

#include <QToolBar>

class QMenu;
class QToolButton;
class QHBoxLayout;
class SpinBox;
class CurveToolBar : public QToolBar
{
    Q_OBJECT

    friend class CurvePage;
    friend class MainWindow;

public:
    explicit CurveToolBar(QWidget *);
    ~CurveToolBar();
/**
    Returns the number of curve slots/widgets pages.
*/
    int slotCount() const;
/**
    Hides the spinbox used for fine setting the curve, replacing it with a
    spacer item.
*/
    void hideSpinBox();
/**
    Show the spinbox used for fine setting the curve.
*/
    void showSpinBox();
/**
    Returns if the spinbox is hidden.
*/
    bool isSpinBoxHidden() const;
/**
    Returns the button menu pointer, to choose between the original saved curve
    and other 3 temporary curve memory slots.
*/
    QToolButton *buttonSlot() const;
/**
    Returns the cancel button pointer.
    When pressed, cancels all changes, restoring the original curve that was set
    previously on the current selected slot.
*/
    QToolButton *buttonCancel() const;
/**
    Returns the Ok button pointer.
    When pressed, sets all changes in the current selected slot.
*/
    QToolButton *buttonOk() const;
/**
    Returns the restore button pointer.
    When pressed, cancels all changes, restoring the original curve that was
    saved, replacing the current slot previous settings.
*/
    QToolButton *buttonRestore() const;
/**
    Returns the save button pointer.
    When pressed, saves all changes from the current selected slot to the device,
    replacing the curve displayed on the read-only slot 0.
    Other slots remains unchanged.
*/
    QToolButton *buttonSave() const;
/**
    Returns the dropdown menu pointer associated to the slots button menu.
*/
    QMenu *dropDownMenu() const;
/**
    Returns the spinbox pointer used for fine setting the curve.
*/
    SpinBox *spinBox() const;
/**
    Returns whether curve value changes was applied.
*/
    bool isSetupChanged();
/**
    When pressing the ok button to set values, enables the restore and save
    buttons.
*/
    void setSetupUnsaved();
/**
    Enables/disables the toolbar buttons.
*/
    void setSetupChanged(bool);

signals:
/**
    Signal emitted when a menu item was clicked, passing its index as argument.
*/
    void sigSlotButtonMenuItemChanged(int);

private:
    void onActionChanged(QAction *);
    void onOkClicked();
    void onCancelClicked();
    void onRestoreClicked();
    void onSaveClicked();

    QWidget     *spacer;
    QToolButton *tbnSlot;
    QToolButton *tbnCancel;
    QToolButton *tbnOk;
    QToolButton *tbnRestore;
    QToolButton *tbnSave;
    SpinBox     *sbx;

    QMenu   *mnuDropDown;
};
#endif // TOOLBARCURVE_H
