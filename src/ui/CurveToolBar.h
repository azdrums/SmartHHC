/*
	SmartHHC - Electronic Drums HiHat Controller Manager
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
#pragma once
#include <QWidget>

class QMenu;
class QToolButton;
class QHBoxLayout;

namespace qsw {
class SpinBox;
}
class CurveToolBar : public QWidget
{
	Q_OBJECT

public:
	explicit CurveToolBar(QWidget *);
	~CurveToolBar();
/**
	Returns the number of curve slots/widgets pages.
*/
	int slotCount() const;
/**
	Sets the current slot index, aka select the current slot on the slot
	button.
*/
	void setCurrentSlotIndex(int);
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
	Returns the button menu pointer, used to choose one of the original saved
	curve or one of other 3 temporary curve memory slots.
	At each selection it sets the saved slot to the device for use.
*/
	QToolButton *buttonSlot() const;
/**
	Returns the cancel button pointer.
	When pressed, cancels all changes, restoring the current curve settings.
*/
	QToolButton *buttonCancel() const;
/**
	Returns the Ok button pointer.
	When pressed, sets all changes in the current selected slot.
*/
	QToolButton *buttonOk() const;
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
    qsw::SpinBox *spinBox() const;
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
	void onSaveClicked();

	QHBoxLayout *layout;
	QToolButton *tbnSlot;
	QToolButton *tbnCancel;
	QToolButton *tbnOk;
	QToolButton *tbnSave;
    qsw::SpinBox  *sbx;
	QMenu	   *mnuDropDown;
};
