#ifndef CURVEEDITOR_H
#define CURVEEDITOR_H

#include <QWidget>

class QStackedWidget;
class QVBoxLayout;
class CurvePage;
class CurveToolBar;
class CurveEditor : public QWidget
{
    Q_OBJECT

public:
    explicit CurveEditor(QWidget *parent = nullptr);
    ~CurveEditor();
/**
    Returns the editor toolbar pointer.
*/
    CurveToolBar *toolBar()     const;
/**
    Returns the page pointer at the specified index.
    Returns the amount of pages in this editor.
*/
    int count() const;
/**
    Returns the current page pointer.
*/
    CurvePage *currentPage() const;
/**
    Returns the current page index.
*/
    int currentIndex() const;
/**
    Sets the index position of the current page.
*/
    void setCurrentIndex(int);
/**
    Set curve values at specified page index.
*/
    void setValuesAt(int, QVector<int>);
/**
    Enables the needed controls.
*/
    void setConnected(bool);

private:
    void onKnotValueChanged(int, int);
    void onSpinBoxValueChanged(int);

    void onCurveOk();
    void onCurveCancel();
    void onCurveRestore();
    void onCurveSave();

    const int pageCount = 4;
    QVector<CurvePage *> pages;
    QVector<bool> saveFlags;

    QVBoxLayout    *layout;
    QStackedWidget *stkMain;
    CurveToolBar   *tbrMain;
};
#endif // CURVEEDITOR_H
