/*
    SmartHHC - Electronic Drums HiHat Controller Manager
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
