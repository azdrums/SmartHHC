#include <QStackedWidget>
#include <QToolButton>
#include <QVBoxLayout>

#include "CurveEditor.h"
#include "CurvePage.h"
#include "CurveToolBar.h"
#include "CurveWidget.h"
#include "spinbox.h"

CurveEditor::CurveEditor(QWidget *parent)
:
    QWidget(parent),
    pages(QVector<CurvePage *>(pageCount)),
    saveFlags({false, false, false}),
    layout(new QVBoxLayout(this)),
    stkMain(new QStackedWidget(this)),
    tbrMain(new CurveToolBar(this))
{
    layout->setSpacing(0);
    layout->setSizeConstraint(QLayout::SetNoConstraint);
    layout->setContentsMargins(0, 0, 0, 0);

    tbrMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    tbrMain->setMinimumSize(QSize(0, 48));
    layout->addWidget(tbrMain);

    stkMain->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    for (int i = 0; i < tbrMain->slotCount(); ++i)
    {
        CurvePage *curvePage = new CurvePage(stkMain);
        curvePage->setEnabled(false);

        stkMain->addWidget(curvePage);
        pages[i] = curvePage;

        if (i == 0)
            continue;

        connect(curvePage->curveWidget(), &CurveWidget::knotValueChanged, this,
            &CurveEditor::onKnotValueChanged);
    }
    layout->addWidget(stkMain);

    connect(tbrMain, &CurveToolBar::sigSlotButtonMenuItemChanged,
            this, &CurveEditor::setCurrentIndex);

    connect(tbrMain->buttonCancel(), &QToolButton::clicked, this,
            &CurveEditor::onCurveCancel);

    connect(tbrMain->buttonOk(), &QToolButton::clicked, this,
            &CurveEditor::onCurveOk);

    connect(tbrMain->buttonSave(), &QToolButton::clicked, this,
            &CurveEditor::onCurveSave);

    connect(tbrMain->spinBox(), QOverload<int>::of(&SpinBox::valueChanged),
            this, &CurveEditor::onSpinBoxValueChanged);

    setCurrentIndex(0);
}
CurveEditor::~CurveEditor()
{
}
CurveToolBar *CurveEditor::toolBar() const
{
    return tbrMain;
}
int CurveEditor::count() const
{
    return stkMain->count();
}
CurvePage *CurveEditor::currentPage() const
{
    return pages.at(stkMain->currentIndex());
}
int CurveEditor::currentIndex() const
{
    return stkMain->currentIndex();
}
void CurveEditor::setCurrentIndex(int index)
{
    stkMain->setCurrentIndex(index);

    if (index > 0)
    {
        bool enableSave = saveFlags.at(index - 1);
        tbrMain->buttonSave()->setEnabled(enableSave);
    }
}
void CurveEditor::setValuesAt(int index, QVector<int> values)
{
    pages[index]->setValues(values);
}
void CurveEditor::setConnected(bool connected)
{
//  Don't enable/disable the stored curve
    foreach (CurvePage *page, pages)
    {
        if (page != pages.at(0))
            page->setEnabled(connected);
    }
    tbrMain->buttonSlot()->setEnabled(connected);

    if (connected)
    {
        stkMain->setCurrentIndex(1);
        tbrMain->setCurrentSlotIndex(1);
    }
    else
    {
        stkMain->setCurrentIndex(0);
        tbrMain->setCurrentSlotIndex(0);
    }
}
void CurveEditor::onKnotValueChanged(int index, int value)
{
//  Show fine tune spinbox.
    if (tbrMain->isSpinBoxHidden())
        tbrMain->showSpinBox();

    CurvePage   *curPage  = pages.at(stkMain->currentIndex());
    CurveWidget *wgtCurve = curPage->curveWidget();

//  Set min, max and value into spinbox.
    int minimum = HHCurve::MinValue,
        maximum = wgtCurve->knotValues().at(1);

    if (index > 0)
    {
        minimum = wgtCurve->knotValues().at(index - 1);

        if (index < HHCurve::KnotCount - 1)
            maximum = wgtCurve->knotValues().at(index + 1);
        else
            maximum = HHCurve::MaxValue;
    }
    tbrMain->spinBox()->blockSignals(true);
    tbrMain->spinBox()->setMaximum(maximum);
    tbrMain->spinBox()->setMinimum(minimum);
    tbrMain->spinBox()->setValue(value);
    tbrMain->spinBox()->blockSignals(false);

    if (!tbrMain->isSetupChanged())
        tbrMain->setSetupChanged(true);
}
void CurveEditor::onSpinBoxValueChanged(int value)
{
    CurvePage   *curPage  = pages.at(stkMain->currentIndex());
    CurveWidget *wgtCurve = curPage->curveWidget();

    wgtCurve->setKnotValueAt(wgtCurve->currentKnotIndex(), value);
}
void CurveEditor::onCurveCancel()
{
    tbrMain->buttonSlot()->setEnabled(true);
}
void CurveEditor::onCurveOk()
{
    tbrMain->buttonSlot()->setEnabled(true);

    int index = stkMain->currentIndex();
    if (index > 0)
        saveFlags[index - 1] = true;
}
void CurveEditor::onCurveSave()
{
    saveFlags[stkMain->currentIndex() - 1] = false;
    tbrMain->buttonSlot()->setEnabled(true);
}
