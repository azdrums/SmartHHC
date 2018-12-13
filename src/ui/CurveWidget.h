#ifndef WIDGETCURVE_H
#define WIDGETCURVE_H

#include <QWidget>

namespace HHCurve
{
    const short KnotCount = 9;
    const short MinValue  = 0;
    const short MaxValue  = 127;
    const short CellCount = KnotCount - 1;
    const short Range     = (MaxValue + 1) / CellCount;
}
class CurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWidget(QWidget *parent = nullptr);
/**
    Sets the values of the curve.
*/
    void setCurveValues(QVector<int>);
    void setCurveValueAt(int, int);
    int curveValueAt(int);
    QVector<int> curveValues() const {return curValues;}

    void setKnotValueAt(int, int);
//  int  knotValueAt(int) const;
    QVector<int> knotValues()  const {return knotVals;}
    int currentKnotIndex() const {return currKnotIndex;}

signals:
/**
    Signal emitted while moving a knot by keeping the LMB pressed over it,
    passing its index and value arguments as output.
*/
    void knotValueChanged(int /*index*/, int /*value*/);
/**
    Signal emitted while releasing the LMB after a knot move,
    so it is called less often than knotValueChanged().
*/
    void curveChanged();

private:
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent (QResizeEvent *);

    void updateRect();
    void resetKnots(); //  Sets a linear curve.

    qreal valueAsY(int);
    int   yAsValue(qreal);

    bool isInRange(int, int);

    QRectF rectGrid;
    QSize knotSize;
    int   penWidth;
    int   currKnotIndex;
    bool  isMovingKnot; // We are going to release the LMB after moving the knot
                        // or after cancel by clicking on the free area.

    QVector<int>     curValues;
    QVector<int>     knotVals;
    QVector<QPointF> knots;
};
#endif // WIDGETCURVE_H
