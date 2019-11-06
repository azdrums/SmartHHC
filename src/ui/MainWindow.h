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
#include <QMainWindow>
#include <QSharedPointer>

namespace hhc {

class device;
#ifdef Q_OS_ANDROID
	class bluetooth;
#else
	class serial;
#endif
class settings;
}
class QStackedWidget;
class QVBoxLayout;
class ActionBar;
class TabLayout;
class CurveEditor;
class PageHome;
class PageConnection;
class PageTerminal;
class PageAbout;
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow();
	~MainWindow();

private:
	void closeEvent(QCloseEvent *);

	void loadAppSettings();
	void saveAppSettings();
	void loadProfileToUI();
	void setProfileFromUI();

	void setConnected(bool);

	void writeData(const QByteArray &);

	void scanDevices();
	void doToggleConnection();
	void doConnect();
	void doDisconnect();

	// Device Signal Slots

	void onConnected();
	void onCurveChanged(const QStringList &);
	void onCurvePointChanged(const QPoint &);
	void onDisconnected();
	void onError(uint8_t);
	void onFixed1ValueChanged(const uint8_t);
	void onFixed2ValueChanged(const uint8_t);
	void onFixed3ValueChanged(const uint8_t);
	void onIntervalValueChanged(const uint16_t);
	void onMinValueChanged(const uint16_t);
	void onMaxValueChanged(const uint16_t);
	void onPositionValueChanged(const uint16_t);
	void onReadLine(const QString &);
	void onReady();

	// Profile Settings Signal Slots

	void onOpenProfile();
	void onSaveProfile();
	void onSaveAsProfile();
	bool checkProfile(const QString &);

	// About Informations Signal Slots

	void onAbout();
	void onActionBarUp();

	// Device Settings Signal Slots

	void onSetFixed1();
	void onSetFixed2();
	void onSetFixed3();
	void onSetClosed();
	void onSetOpen();
	void onSetAsClosed();
	void onSetAsOpen();
	void onPedalPositionsRestore();
	void onPedalPositionsSet();
	void onPedalPositionsSave();
	void onSetInterval();

	void setCurrentIndex(int);
	void onCurveOk();
	void onCurveCancel();
	void onCurveRestore();
	void onCurveSave();

	void onSpinBoxValueChanged(int);

#ifdef Q_OS_ANDROID
	void onBluetoothDeviceDiscovered(const QStringList &);
	void onBluetoothDeviceScanFinished();
	void onBluetoothServiceDiscovered(const QStringList &);
	void onBluetoothServiceScanFinished();

	hhc::bluetooth *device;
#else
	hhc::serial *device;
#endif

	CurveEditor    *pageCurv;
	PageHome       *pageHome;
	PageConnection *pageConn;
	PageTerminal   *pageTerm;
	PageAbout      *pageInfo;

	QWidget        *wgtMain;
	QVBoxLayout    *layout;
	QStackedWidget *stkMain;
	TabLayout      *tloMain;
	ActionBar      *actBar;

	QAction *actConn;
	QAction *actScan;
	QAction *actLoad;
	QAction *actSave;
	QAction *actSvAs;
	QAction *actInfo;
	QAction *actQuit;

	bool hasDeviceSetValue; // Flag to avoid enabling buttons when
													// device changed values, not user.
	bool isStarted;         // onFirstDeviceDataSent() flag

	typedef QSharedPointer<hhc::settings> settingsPtr;
	settingsPtr settings;
};
