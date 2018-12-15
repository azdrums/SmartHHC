/*
    QtAndroidWidgets - Custom Android like Qt based Widgets Library
    Copyright (C) 2018 Andrea Zanellato

    This Source Code Form is subject to the terms of the Mozilla Public
    License, v. 2.0. If a copy of the MPL was not distributed with this
    file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/
/**
    @file   spinbox.h
    @author Andrea Zanellato
*/
#ifndef SPINBOX_H
#define SPINBOX_H

#include <QWidget>
#include <QAbstractSpinBox>

class QHBoxLayout;
class QVBoxLayout;
class QIntValidator;
class QLineEdit;
class QToolButton;
/**
    Custom spinbox class replacement for
    <a href="http://doc.qt.io/qt-5/qspinbox.html#details">QSpinBox</a>,
    which ATM has too small buttons on Android and
    also working on stylesheet to change them get bad results.

    Some fake functions has been added to mimic the original QSpinBox and
    inherited classes:

    hasFrame() / setFrame(): a frame is always drawn.
    alignment() / setAlignment(): the text alignment is always centered.
    wrapping() / setWrapping(): the wrapping is not used.
    isReadOnly() / setReadOnly(): the control is always editable.
    specialValueText() / setSpecialValueText(): it has no special value text...
    suffix() / setSuffix(): ...nor suffix...
    prefix() / setPrefix(): ...or prefix.
*/
class SpinBox : public QWidget
{
    Q_OBJECT

public:
/**
    Constructs a spin box with 0 as minimum value and 99 as maximum value,
    a step value of 1.

    The value is initially set to 0. It is parented to parent.
*/
    explicit SpinBox(QWidget *parent = nullptr);
/**
    This property holds the maximum value of the spin box.

    When setting this property the @ref minimum is adjusted if necessary,
    to ensure that the range remains valid.

    The default maximum value is 99.
*/
    int  maximum() const {return mMax;}
/**
    @see maximum().
*/
    void setMaximum(int max);
/**
    This property holds the minimum value of the spin box.

    When setting this property the @ref maximum is adjusted if necessary
    to ensure that the range remains valid.

    The default minimum value is 0.
*/
    int  minimum() const {return mMin;}
/**
    @see minimum().
*/
    void setMinimum(int min);
/**
    This property holds the value of the spin box.

    @ref setValue() will emit valueChanged() if the new value is different from
    the old one.
    The value property has a second notifier signal which includes the
    spin box's prefix and suffix.
*/
    int value() const {return mVal;}
/**
    @see value()
*/
    void setValue(int val);
/**
    @brief hasFrame

    @return @true.
*/
    bool hasFrame() const {return true;}
/**
    @brief Ignored.
*/
    void setFrame(bool) {}
/**
    @brief alignment

    @return Qt::AlignCenter.
*/
    Qt::Alignment alignment() const {return Qt::AlignCenter;}
/**
    @brief Ignored.
*/
    void setAlignment(Qt::Alignment) {}
/**
    @brief wrapping
    @return @false
*/
    bool wrapping() const {return false;}
/**
    @brief Ignored.
*/
    void setWrapping(bool) {}
/**
    @brief isReadOnly
    @return @false.
*/
    bool isReadOnly() const {return false;}
/**
    @brief Ignored.
*/
    void setReadOnly(bool) {}
/**
    @brief specialValueText
    @return QString().
*/
    QString specialValueText() const {return QString();}
/**
    @brief Ignored.
*/
    void setSpecialValueText(const QString &) {}
/**
    @brief suffix
    @return QString().
*/
    QString suffix() const {return QString();}
/**
    @brief Ignored.
*/
    void setSuffix(const QString &) {}
/**
    @brief prefix
    @return QString().
*/
    QString prefix() const {return QString();}
/**
    @brief Ignored.
*/
    void setPrefix(const QString &) {}

    QAbstractSpinBox::ButtonSymbols buttonSymbols() const;
    void setButtonSymbols(QAbstractSpinBox::ButtonSymbols);

    QToolButton *getButtonMinus() const;
    QToolButton *getButtonPlus() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation);

signals:
/**

*/
    void valueChanged(int);

private:
    bool eventFilter(QObject *, QEvent *);
    void onButtonPressed();
    void onButtonReleased();
    void onTextChanged(const QString &);
    void onTimer();
    void checkButtonsEnabled();

    void setOrientationHorizontal();
    void setOrientationVertical();
/*
    // This could be used for compatibility for future use.
    void mousePressEvent(QMouseEvent *) override;
    void mouseReleaseEvent(QMouseEvent *) override;
*/
    QHBoxLayout   *hLayout   = nullptr; //!< Widget horizontal layout.
    QVBoxLayout   *vLayout   = nullptr; //!< Widget vertical layout.
    QIntValidator *validator = nullptr; //!< QLineEdit validator.
    QLineEdit     *txtSpin   = nullptr; //!< Single line text editor.
    QTimer        *timer     = nullptr;
    QToolButton   *tbnMinus  = nullptr, //!< Decreasing value button.
                  *tbnPlus   = nullptr; //!< Increasing value button.
    int           mMax       = 100,     //!< Maximum range value.
                  mMin       = 0,       //!< Minimum range value.
                  mVal       = 0;       //!< Current value.
    QAbstractSpinBox::ButtonSymbols _buttonSymbols = QAbstractSpinBox::UpDownArrows;
    Qt::Orientation _orientation = Qt::Horizontal;
};
#endif // SPINBOX_H
