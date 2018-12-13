#include <QMouseEvent>
#include <QPainter>
#include <QVector>

#include <spinbox.h>
/*
#include <QSpinBox>
#define SpinBox QSpinBox
*/
#include "UICurve.h"
#include "ui_UICurve.h"

CurveWidget::CurveWidget(QWidget *parent) : QWidget(parent)
{
    setStyleSheet("background-color:white;border:1px solid palette(dark)");
    setMouseTracking(true);
    updateRect();
}
bool CurveWidget::isInRange(int index, int value)
{
    int prevValue = -1,
        nextValue = 128;

    if (index > 0)
        prevValue = _yValues.at(index - 1);

    if (index < knotCount - 1)
        nextValue = _yValues.at(index + 1);

    bool inRange = (prevValue <= value) && (value <= nextValue);
    return inRange;
}
void CurveWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && rectGrid.contains(event->pos()))
    {
        // Set a rect with the circle area size
        QRect rPos(event->pos().x() - knotSize.width(), rectGrid.top(),
                   knotSize.width() * 2, height());

        for (int i = 0; i < knots.size(); i++)
        {
            if (rPos.contains(knots.at(i).toPoint()))
            {
                int y = event->pos().y();
                int value = yAsValue(y);

                if (isInRange(i, value))
                {
                    knots[i].setY(y);
                    _yValues[i] = value;

                    emit valueChanged(i, value);
                }
                break;
            }
        }
        update();
    }
}
/*
void CurveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QRect rPos(event->pos().x() - knotSize.width(), rectGrid.top(),
               knotSize.width() * 2, height());

    for (int i = 0; i < knots.size(); i++)
    {
        if (rPos.contains(knots.at(i).toPoint()))
        {
            int y = event->pos().y();
            int value = yAsValue(y);
            emit valueChanged(i, value);
            break;
        }
    }
}
*/
void CurveWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    // Required by setStyleSheet to work on a subclassed widget
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

    // Smooth drawing
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(rectGrid);
    qreal height, width;
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    int i, nSquares = knotCount -1;

//  Draw grid

    for (i = 1; i < nSquares; i++)
    {
        height = rectGrid.height() / nSquares * i + 10;
        width  = rectGrid.width()  / nSquares * i + 10;
        QPointF p1(rectGrid.left(),  height);
        QPointF p2(rectGrid.right(), height);
        painter.drawLine(p1, p2);
        p1 = QPointF(width, rectGrid.bottom());
        p2 = QPointF(width, rectGrid.top());
        painter.drawLine(p1, p2);
    }

//  Draw points

    pen.setWidth(penWidth);
    for (i = 0; i < knotCount; i++)
    {
        pen.setColor(Qt::red);
        painter.setPen(pen);

        qreal x = rectGrid.width() / nSquares * i + knotSize.width();
        qreal y = valueAsY(_yValues.at(i));

        knots[i].setX(x);
        knots[i].setY(y);

        painter.drawEllipse(knots.at(i), knotSize.width(), knotSize.height());
        if (i > 0)
        {
            pen.setColor(Qt::black);
            painter.setPen(pen);
            painter.drawLine(knots.at(i - 1), knots.at(i));
        }
    }
}
void CurveWidget::resizeEvent (QResizeEvent *)
{
    updateRect();
}
void CurveWidget::updateRect()
{
    rectGrid = QRectF(this->rect().x()      + knotSize.width(),
                      this->rect().y()      + knotSize.height(),
                      this->rect().width()  - knotSize.width() * 2,
                      this->rect().height() - knotSize.height() * 2);
}
int CurveWidget::valueAt(int index) const
{
    if (index >= 0 && index < knotCount)
        return _yValues.at(index);

    return -1;
}
void CurveWidget::setValueAt(int index, int value)
{
    int y = valueAsY(value);

    knots[index].setY(y);
    _yValues[index] = value;
    update();
}
qreal CurveWidget::valueAsY(int value)
{
    int y = (rectGrid.height() + (knotSize.height())) -
            (rectGrid.height() * value / maxValue);
    return y;
}
int CurveWidget::yAsValue(qreal y)
{
    int value = maxValue - ((y - knotSize.height()) / rectGrid.height() * maxValue);
    return value;
}
UICurve::UICurve(QWidget *parent) : QWidget(parent),
    ui(new Ui::UICurve)
{
    ui->setupUi(this);

    setLayout(ui->layout);

    int minVal = ui->wgtCurve->minimumValue(),
        maxVal = ui->wgtCurve->yValues().at(1);

    for (int i = 0; i < spinBoxes.size(); i++)
    {
        if (i > 0)
        {
            minVal = ui->wgtCurve->yValues().at(i - 1);

            if (i < spinBoxes.size() - 1)
            {
                maxVal = ui->wgtCurve->yValues().at(i + 1);
            }
            else
            {
                maxVal = ui->wgtCurve->maximumValue();
            }
        }
        spinBoxes[i] = new SpinBox(this);
        spinBoxes[i]->setMinimum(minVal);
        spinBoxes[i]->setMaximum(maxVal);
        spinBoxes[i]->setOrientation(Qt::Vertical);
        spinBoxes[i]->setValue(ui->wgtCurve->valueAt(i));
        spinBoxes[i]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        spinBoxes[i]->setMaximumWidth(48);
#ifdef Q_OS_ANDROID
        QFont font = spinBoxes[i]->font();
        font.setPointSize(8);
        font.setBold(true);
        spinBoxes[i]->setFont(font);
#endif
        ui->layCurveValues->addWidget(spinBoxes.at(i));

        if (i < spinBoxes.size() - 1)
            ui->layCurveValues->addStretch();

        connect(spinBoxes.at(i), QOverload<int>::of(&SpinBox::valueChanged), this,
                &UICurve::onSpinBoxCurveValueChanged);
    }
    connect(ui->wgtCurve, &CurveWidget::valueChanged, this,
            &UICurve::onCurveValueChanged);
}
UICurve::~UICurve()
{
    delete ui;
}
void UICurve::onCurveValueChanged(int index, int value)
{
    int prevValue = ui->wgtCurve->minimumValue(),
        nextValue = ui->wgtCurve->yValues().at(1);

    if (index > 0)
    {
        prevValue = spinBoxes.at(index - 1)->value();

        if (index < knotCount - 1)
            nextValue = spinBoxes.at(index + 1)->value();
        else
            nextValue = ui->wgtCurve->maximumValue();
    }
    if ((prevValue > value) || (value > nextValue))
        return;

    spinBoxes[index]->blockSignals(true);
    spinBoxes[index]->setValue(value);
    spinBoxes[index]->blockSignals(false);
}
void UICurve::onSpinBoxCurveValueChanged(int value)
{
    SpinBox *spinBox = dynamic_cast<SpinBox *>(sender());
    if (spinBox)
    {
        int index = spinBoxes.indexOf(spinBox);
        ui->wgtCurve->setValueAt(index, value);

        if (spinBoxes.size() > 1)
        {
            if (spinBox == spinBoxes.first())
            {
                spinBoxes[1]->setMinimum(value);
            }
            else
            {
                spinBoxes[index - 1]->setMaximum(value);

                if (spinBox != spinBoxes.last())
                    spinBoxes[index + 1]->setMinimum(value);
            }
        }
    }
}
void UICurve::setValue(int index, int value)
{
    if (index < 128)
    {
        curValues[index] = value;
    }
}
QVector<int> UICurve::values() const
{
    return curValues;
}
void UICurve::setValues(const QVector<int> values)
{
    curValues = values;
}
