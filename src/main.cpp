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
#include <QApplication>
#include <QTranslator>

//#include "actionbar.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
/*
//  TODO: Fix library issue on Android
	ActionBar::initResources();
*/
	// Set application data also for settings
	app.setOrganizationName("AZDrums");
	app.setOrganizationDomain("azdrums.org");
	app.setApplicationName("SmartHHC");

	// Load locale translation file if any
	QTranslator translator;
	if (translator.load(QLocale(), QLatin1String("smarthhc"), QLatin1String("_"),
						QLatin1String(":/translations")))
		app.installTranslator(&translator);

	MainWindow window;
	window.show();

	return app.exec();
}
