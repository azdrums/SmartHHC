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

	QString htmlInfo = ui->txtInfo->toHtml();
	htmlInfo.replace("@TITLE@", tr("SmartHHC Digital HiHat Controller"));
	htmlInfo.replace("@VERSION@", "0.1.2alpha");
	htmlInfo.replace("@RESERVED@", tr("All Rights Reserved."));
	ui->txtInfo->setHtml(htmlInfo);

	QString htmlTnx = ui->txtTnx->toHtml();
	htmlTnx.replace("@TITLE@", tr("Hardware Developers"));
	htmlTnx.replace("@AUTHOR@", tr("Project Creator, Firmware and Hardware Developer"));
	htmlTnx.replace("@AUTHOR2@", tr("Hardware Developer"));
	htmlTnx.replace("@SOFTWARE_TITLE@", tr("Software Developers"));
	ui->txtTnx->setHtml(htmlTnx);
}
PageAbout::~PageAbout()
{
	delete ui;
}
