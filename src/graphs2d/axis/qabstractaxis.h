// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QABSTRACTAXIS_H
#define QABSTRACTAXIS_H

#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtCore/QVariant>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

QT_BEGIN_NAMESPACE

class QAbstractAxisPrivate;

class QAbstractAxis : public QObject
{
    Q_OBJECT
    //visibility
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    //arrow
    Q_PROPERTY(bool lineVisible READ isLineVisible WRITE setLineVisible NOTIFY lineVisibleChanged)
    //labels
    Q_PROPERTY(bool labelsVisible READ labelsVisible WRITE setLabelsVisible NOTIFY labelsVisibleChanged)
    Q_PROPERTY(int labelsAngle READ labelsAngle WRITE setLabelsAngle NOTIFY labelsAngleChanged)
    Q_PROPERTY(QFont labelsFont READ labelsFont WRITE setLabelsFont NOTIFY labelsFontChanged)
    Q_PROPERTY(QColor labelsColor READ labelsColor WRITE setLabelsColor NOTIFY labelsColorChanged)
    Q_PROPERTY(bool labelsTruncated READ labelsTruncated NOTIFY labelsTruncatedChanged)
    Q_PROPERTY(bool truncateLabels READ truncateLabels WRITE setTruncateLabels NOTIFY truncateLabelsChanged)
    //grid
    Q_PROPERTY(bool gridVisible READ isGridLineVisible WRITE setGridLineVisible NOTIFY gridVisibleChanged)
    Q_PROPERTY(bool minorGridVisible READ isMinorGridLineVisible WRITE setMinorGridLineVisible NOTIFY minorGridVisibleChanged)
    Q_PROPERTY(QColor gridLineColor READ gridLineColor WRITE setGridLineColor NOTIFY gridLineColorChanged)
    Q_PROPERTY(QColor minorGridLineColor READ minorGridLineColor WRITE setMinorGridLineColor NOTIFY minorGridLineColorChanged)
    //title
    Q_PROPERTY(QString titleText READ titleText WRITE setTitleText NOTIFY titleTextChanged)
    Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor NOTIFY titleColorChanged)
    Q_PROPERTY(bool titleVisible READ isTitleVisible WRITE setTitleVisible NOTIFY titleVisibleChanged)
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont NOTIFY titleFontChanged)
    //orientation
    Q_PROPERTY(Qt::Orientation orientation READ orientation)
    //alignment
    Q_PROPERTY(Qt::Alignment alignment READ alignment)
    Q_PROPERTY(bool reverse READ isReverse WRITE setReverse NOTIFY reverseChanged)
    QML_FOREIGN(QAbstractAxis)
    QML_NAMED_ELEMENT(AbstractAxis)

public:

    enum AxisType {
        AxisTypeNoAxis = 0x0,
        AxisTypeValue = 0x1,
        AxisTypeBarCategory = 0x2,
        AxisTypeCategory = 0x4,
        AxisTypeDateTime = 0x8,
        AxisTypeLogValue = 0x10,
        AxisTypeColor = 0x20
    };

    Q_DECLARE_FLAGS(AxisTypes, AxisType)

protected:
    explicit QAbstractAxis(QAbstractAxisPrivate &d, QObject *parent = nullptr);

public:
    ~QAbstractAxis();

    virtual AxisType type() const = 0;

    //visibility handling
    bool isVisible() const;
    void setVisible(bool visible = true);
    void show();
    void hide();

    //arrow handling
    bool isLineVisible() const;
    void setLineVisible(bool visible = true);

    //grid handling
    bool isGridLineVisible() const;
    void setGridLineVisible(bool visible = true);
    bool isMinorGridLineVisible() const;
    void setMinorGridLineVisible(bool visible = true);
    void setGridLineColor(const QColor &color);
    QColor gridLineColor();
    void setMinorGridLineColor(const QColor &color);
    QColor minorGridLineColor();

    //labels handling
    bool labelsVisible() const;
    void setLabelsVisible(bool visible = true);
    void setLabelsFont(const QFont &font);
    QFont labelsFont() const;
    void setLabelsAngle(int angle);
    int labelsAngle() const;
    void setLabelsColor(QColor color);
    QColor labelsColor() const;

    //title handling
    bool isTitleVisible() const;
    void setTitleVisible(bool visible = true);
    void setTitleColor(const QColor &color);
    QColor titleColor() const;
    void setTitleFont(const QFont &font);
    QFont titleFont() const;
    void setTitleText(const QString &title);
    QString titleText() const;

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    Qt::Alignment alignment() const;

    //range handling
    void setMin(const QVariant &min);
    void setMax(const QVariant &max);
    void setRange(const QVariant &min, const QVariant &max);

    //reverse handling
    void setReverse(bool reverse = true);
    bool isReverse() const;

    //label editable handling
    void setLabelsEditable(bool editable = true);
    bool labelsEditable() const;

    bool labelsTruncated() const;

    void setTruncateLabels(bool truncateLabels = true);
    bool truncateLabels() const;

Q_SIGNALS:
    void visibleChanged(bool visible);
    void lineVisibleChanged(bool visible);
    void labelsVisibleChanged(bool visible);
    void labelsFontChanged(const QFont &font);
    void labelsAngleChanged(int angle);
    void gridVisibleChanged(bool visible);
    void minorGridVisibleChanged(bool visible);
    void gridLineColorChanged(const QColor &color);
    void minorGridLineColorChanged(const QColor &color);
    void colorChanged(QColor color);
    void labelsColorChanged(QColor color);
    void titleTextChanged(const QString &title);
    void titleColorChanged(const QColor &color);
    void titleVisibleChanged(bool visible);
    void titleFontChanged(const QFont &font);
    void reverseChanged(bool reverse);
    void labelsEditableChanged(bool editable);
    void labelsTruncatedChanged(bool labelsTruncated);
    void truncateLabelsChanged(bool truncateLabels);
    void update();

protected:
    QScopedPointer<QAbstractAxisPrivate> d_ptr;
    friend class ChartDataSet;
    friend class ChartPresenter;
    friend class ChartThemeManager;
    friend class AbstractDomain;
    friend class ChartAxisElement;
    friend class HorizontalAxis;
    friend class VerticalAxis;
    friend class XYChart;

private:
    Q_DISABLE_COPY(QAbstractAxis)
};

QT_END_NAMESPACE

#endif // QABSTRACTAXIS_H
