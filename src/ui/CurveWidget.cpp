#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QVector>

#include "CurveWidget.h"

CurveWidget::CurveWidget(QWidget *parent)
:
    QWidget(parent),
    rectGrid(QRectF()),
    knotSize(QSize(16, 16)),
    penWidth(2),
    currKnotIndex(0),
    isMovingKnot(false),
    curValues(QVector<int>(HHCurve::MaxValue + 1)),
    knotVals(HHCurve::KnotCount),
    knots(HHCurve::KnotCount)
{
    resetKnots();
    setStyleSheet(".CurveWidget {background-color:white;"
                  "border:1px solid palette(dark)}");
    setMouseTracking(true);
    updateRect();
}
void CurveWidget::resetKnots()
{
    for (int i = 0; i < HHCurve::KnotCount; ++i)
    {
        if ((i > 0) && (i < HHCurve::CellCount))
            knotVals[i] = HHCurve::Range * i;
        else if (i == 0)
            knotVals[i] = HHCurve::MinValue;
        else
            knotVals[i] = HHCurve::MaxValue;
    }
}
void CurveWidget::setCurveValues(QVector<int> values)
{
    curValues = values;

    for (int i = 0; i < HHCurve::KnotCount; ++i)
    {
        if (i == 0)
            knotVals[i] = curValues.at(HHCurve::MinValue);
        else if (i == HHCurve::CellCount)
            knotVals[i] = curValues.at(HHCurve::MaxValue);
        else
            knotVals[i] = curValues.at(HHCurve::Range * i);
    }
    update();
}
void CurveWidget::setCurveValueAt(int index, int value)
{
    curValues[index] = value;
}
int CurveWidget::curveValueAt(int index)
{
    if ((index < 0) || (index >= curValues.size()))
        return -1;

    return curValues.at(index);
}
bool CurveWidget::isInRange(int index, int value)
{
    int prevValue = -1,
        nextValue = 128;

    if (index > 0)
        prevValue = knotVals.at(index - 1);

    if (index < HHCurve::KnotCount - 1)
        nextValue = knotVals.at(index + 1);

    bool inRange = (prevValue <= value) && (value <= nextValue);
    return inRange;
}
void CurveWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() && rectGrid.contains(event->pos()))
    {
        // Set a rect with the circle area size
        QRectF rPos(event->pos().x() - knotSize.width(), rectGrid.top(),
                    knotSize.width() * 2, height());

        for (int i = 0; i < knots.size(); i++)
        {
            if (rPos.contains(knots.at(i)))
            {
                int y = event->pos().y();
                int value = yAsValue(y);

                if (isInRange(i, value))
                {
                    knots[i].setY(y);
                    knotVals[i] = value;
                    currKnotIndex = i;
                    emit knotValueChanged(i, value);
                }
                break;
            }
        }
        update();
    }
}
void CurveWidget::mouseReleaseEvent(QMouseEvent *event)
{
    QRectF rPos(event->pos().x() - knotSize.width(), rectGrid.top(),
                knotSize.width() * 2, height());

    for (int i = 0; i < knots.size(); i++)
    {
        if (rPos.contains(knots.at(i)))
        {
            emit curveChanged();
            return;
        }
    }
}
void CurveWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

//  Required by setStyleSheet to work on a subclassed widget
    QStyleOption option;
    option.initFrom(this);
    style()->drawPrimitive(QStyle::PE_Widget, &option, &painter, this);

//  Smooth drawing
    painter.setRenderHint(QPainter::Antialiasing);

    painter.drawRect(rectGrid);
    int x, y;
    QPen pen;
    pen.setWidth(1);
    painter.setPen(pen);
    int i;

//  Draw grid
    for (i = 1; i < HHCurve::CellCount; ++i)
    {
        x = rectGrid.width()  / HHCurve::CellCount * i + knotSize.width();
        y = rectGrid.height() / HHCurve::CellCount * i + knotSize.height();

    //  Draw horizontal lines
        QPointF p1(rectGrid.left(),  y);
        QPointF p2(rectGrid.right(), y);
        painter.drawLine(p1, p2);

    //  Draw vertical lines
        p1 = QPointF(x, rectGrid.bottom());
        p2 = QPointF(x, rectGrid.top());
        painter.drawLine(p1, p2);
    }
//  Draw points
    int indexMin = 0;
    int indexMax = HHCurve::Range;
    qreal ratio;
    qreal temp = HHCurve::Range;

    pen.setWidth(penWidth);
    for (i = 0; i < HHCurve::KnotCount; i++)
    {
        QColor color("#3C96DE");

        if (!isEnabled())
            color = QColor(Qt::black);

        pen.setColor(color);
        painter.setPen(pen);
        painter.setBrush(QBrush(color));

        int x = rectGrid.width() / HHCurve::CellCount * i + knotSize.width();
        int y = valueAsY(knotVals.at(i));

        knots[i].setX(x);
        knots[i].setY(y);

        painter.drawEllipse(knots.at(i), knotSize.width(), knotSize.height());

        if (this->isEnabled())
            pen.setColor(Qt::black);
        else
            pen.setColor(Qt::darkGray);

        painter.setPen(pen);

        if (i > 0)
        {
            QPainterPath path;
            path.moveTo(knots.at(i - 1));
            path.lineTo(knots.at(i));
            painter.drawPath(path);

            temp = 1;
            for (int index = indexMin; index < indexMax; ++index)
            {
                ratio = temp / HHCurve::Range;
                temp++;
                QPointF p = path.pointAtPercent(ratio);
//              painter.drawEllipse(p, 2, 2);
                curValues[index] = yAsValue(p.y());
            }
            indexMin = indexMin + HHCurve::Range;
            indexMax = indexMax + HHCurve::Range;
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
/*
int CurveWidget::knotValueAt(int index) const
{
    if (index >= 0 && index < HHCurve::KnotCount)
        return knotVals.at(index);

    return -1;
}
*/
void CurveWidget::setKnotValueAt(int index, int value)
{
    int y = valueAsY(value);

    knots[index].setY(y);
    knotVals[index] = value;
    update();
}
qreal CurveWidget::valueAsY(int value)
{
    int y = (rectGrid.height() + (knotSize.height())) -
            (rectGrid.height() * value / HHCurve::MaxValue);
    return y;
}
int CurveWidget::yAsValue(qreal y)
{
    int value = HHCurve::MaxValue -
                ((y - knotSize.height()) / rectGrid.height() * HHCurve::MaxValue);
    return value;
}
