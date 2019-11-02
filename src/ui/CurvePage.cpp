/*
	SmartHHC - Electronic Drums HiHat Controller Manager
	Copyright (C) 2018 Andrea Zanellato

	This Source Code Form is subject to the terms of the Mozilla Public
	License, v. 2.0. If a copy of the MPL was not distributed with this
	file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include <QAction>
#include <QLabel>
#include <QMenu>
#include <QStackedWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QVector>

#include "CurvePage.h"
#include "CurveWidget.h"

CurvePage::CurvePage(QWidget *parent)
:
	QWidget(parent),
	labels(QVector<QLabel *>(HHCurve::KnotCount)),
	layout(new QVBoxLayout(this)),
	layCurveValues(new QHBoxLayout()),
	wgtCurve(new CurveWidget(this))
{
	layout->setSpacing(0);
	layout->setSizeConstraint(QLayout::SetNoConstraint);
	layout->setContentsMargins(0, 0, 0, 0);

	wgtCurve->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	layout->addWidget(wgtCurve);

	layCurveValues->setSpacing(0);
	layCurveValues->setSizeConstraint(QLayout::SetNoConstraint);
	layCurveValues->setContentsMargins(6, 6, 6, 6);
	layout->addLayout(layCurveValues);

	for (int i = 0; i < labels.size(); i++)
	{
		labels[i] = new QLabel(this);
		labels[i]->setText(QString::number(wgtCurve->knotValues().at(i)));

		if (i < labels.size() - 1)
			labels[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		else
			labels[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

		layCurveValues->addWidget(labels.at(i));
	}
	connect(wgtCurve, &CurveWidget::knotValueChanged, this,
			&CurvePage::onKnotValueChanged);
}
CurvePage::~CurvePage()
{
	delete layCurveValues;
}
CurveWidget *CurvePage::curveWidget() const
{
	return wgtCurve;
}
void CurvePage::onKnotValueChanged(int index, int value)
{
//  Update labels.
	labels[index]->setText(QString::number(value));
}
void CurvePage::setValue(int index, int value)
{
	Q_ASSERT(index < HHCurve::MaxValue + 1);

	wgtCurve->setCurveValueAt(index, value);
}
QVector<int> CurvePage::values() const
{
	return wgtCurve->curveValues();
}
void CurvePage::setValues(const QVector<int> values)
{
	wgtCurve->setCurveValues(values);

	for (int i = 0; i < labels.count(); ++i)
		labels[i]->setText(QString::number(wgtCurve->knotValues().at(i)));
}
