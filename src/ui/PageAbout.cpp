/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "PageAbout.h"
#include "ui_PageAbout.h"

PageAbout::PageAbout(QWidget *parent)
:
	QWidget(parent),
	ui(new Ui::PageAbout)
{
	ui->setupUi(this);

	setLayout(ui->layout);
	ui->tabInfo->setLayout(ui->layTabInfo);
	ui->tabTnx->setLayout(ui->layTabTnx);
	ui->tabLicense->setLayout(ui->layTabLicense);
}
PageAbout::~PageAbout()
{
	delete ui;
}
