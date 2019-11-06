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

class QStackedWidget;
class QVBoxLayout;
class CurvePage;
class CurveToolBar;
class CurveEditor : public QWidget
{
	Q_OBJECT

public:
	explicit CurveEditor(QWidget *parent = nullptr);
	~CurveEditor();
/**
	Returns the editor toolbar pointer.
*/
	CurveToolBar *toolBar()	 const;
/**
	Returns the page pointer at the specified index.
	Returns the amount of pages in this editor.
*/
	int count() const;
/**
	Returns the current page pointer.
*/
	CurvePage *currentPage() const;
/**
	Returns the current page index.
*/
	int currentIndex() const;
/**
	Sets the index position of the current page.
*/
	void setCurrentIndex(int);
/**
	Set curve values at specified page index.
*/
	void setValuesAt(int, QVector<int>);
/**
	Enables the needed controls.
*/
	void setConnected(bool);

private:
	void onKnotValueChanged(int, int);
	void onSpinBoxValueChanged(int);

	void onCurveOk();
	void onCurveCancel();
	void onCurveRestore();
	void onCurveSave();

	const int pageCount = 4;
	QVector<CurvePage *> pages;
	QVector<bool> saveFlags;

	QVBoxLayout	*layout;
	QStackedWidget *stkMain;
	CurveToolBar   *tbrMain;
};
