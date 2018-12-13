#ifndef UICURVE_H
#define UICURVE_H

#include <QWidget>

class SpinBox;
class CurveWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurveWidget(QWidget *parent = nullptr);
    int valueAt(int) const;
    void setValueAt(int, int);

    int minimumValue() const {return minValue;}
    int maximumValue() const {return maxValue;}

    QVector<int> yValues() const {return _yValues;}

signals:
    void valueChanged(int /*index*/, int /*value*/);

private:
    void mouseMoveEvent(QMouseEvent *);
//  void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void resizeEvent (QResizeEvent *);

    void updateRect();

    qreal valueAsY(int);
    int   yAsValue(qreal);

    bool isInRange(int, int);

    QRectF           rectGrid  = QRectF();
    int              knotCount = 5;
    QVector<QPointF> knots     = QVector<QPointF>(knotCount);
    QSize            knotSize  = QSize(8, 8);
    QVector<int>     _yValues  = {0, 17, 35, 53, 71, 89, 108, 127};
    int              penWidth  = 2;
    short            minValue  = 0;
    short            maxValue  = 127;
};
namespace Ui {
class UICurve;
}
class UICurve : public QWidget
{
    Q_OBJECT

public:
    explicit UICurve(QWidget *parent = nullptr);
    ~UICurve();

    void setValue(int, int);

    QVector<int> values() const;
    void setValues(const QVector<int>);

private:
    void onCurveValueChanged(int, int);
    void onSpinBoxCurveValueChanged(int);

    Ui::UICurve        *ui;
    int                knotCount = 5;
    QVector<SpinBox *> spinBoxes = QVector<SpinBox *>(knotCount);
    QVector<int>       curValues = QVector<int>(128);
};

#endif // UICURVE_H
