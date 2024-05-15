// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QBARSET_H
#define QBARSET_H

#include <QtCore/qobject.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtGui/qbrush.h>
#include <QtGui/qfont.h>
#include <QtGui/qpen.h>
#include <QtQml/qqmlengine.h>

QT_BEGIN_NAMESPACE

class QBarSetPrivate;

class Q_GRAPHS_EXPORT QBarSet : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString label READ label WRITE setLabel NOTIFY labelChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectedColor READ selectedColor WRITE setSelectedColor NOTIFY selectedColorChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY borderColorChanged)
    Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)
    Q_PROPERTY(QVariantList values READ values WRITE setValues NOTIFY valuesChanged)
    Q_PROPERTY(qreal borderWidth READ borderWidth WRITE setBorderWidth NOTIFY borderWidthChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QList<int> selectedBars READ selectedBars NOTIFY selectedBarsChanged)
    QML_NAMED_ELEMENT(BarSet)
    Q_DECLARE_PRIVATE(QBarSet)

public:
    explicit QBarSet(QObject *parent = nullptr);
    explicit QBarSet(const QString label, QObject *parent = nullptr);
    ~QBarSet() override;

    void setLabel(const QString label);
    QString label() const;

    Q_INVOKABLE void append(const qreal value);
    Q_INVOKABLE void append(const QList<qreal> &values);
    Q_INVOKABLE void insert(const int index, const qreal value);
    Q_INVOKABLE void remove(const int index, const int count = 1);
    Q_INVOKABLE void replace(const int index, const qreal value);
    Q_INVOKABLE qreal at(const int index) const;
    Q_INVOKABLE int count() const;
    Q_INVOKABLE qreal sum() const;
    Q_INVOKABLE void clear();

    qreal operator [](const int index) const;
    QBarSet &operator << (const qreal &value);

    QColor color();
    void setColor(QColor color);

    QColor borderColor();
    void setBorderColor(QColor color);

    QColor labelColor();
    void setLabelColor(QColor color);

    QColor selectedColor() const;
    void setSelectedColor(const QColor &color);

    QVariantList values();
    void setValues(QVariantList values);
    qreal borderWidth() const;
    void setBorderWidth(qreal borderWidth);

    Q_INVOKABLE bool isBarSelected(int index) const;
    Q_INVOKABLE void selectBar(int index);
    Q_INVOKABLE void deselectBar(int index);
    Q_INVOKABLE void setBarSelected(int index, bool selected);
    Q_INVOKABLE void selectAllBars();
    Q_INVOKABLE void deselectAllBars();
    Q_INVOKABLE void selectBars(const QList<int> &indexes);
    Q_INVOKABLE void deselectBars(const QList<int> &indexes);
    Q_INVOKABLE void toggleSelection(const QList<int> &indexes);
    QList<int> selectedBars() const;

Q_SIGNALS:
    void update();
    void labelChanged();
    void colorChanged(QColor color);
    void borderColorChanged(QColor color);
    void labelColorChanged(QColor color);
    void valuesChanged();
    void selectedColorChanged(const QColor &color);
    void countChanged();

    void borderWidthChanged(qreal width);

    void valuesAdded(int index, int count);
    void valuesRemoved(int index, int count);
    void valueChanged(int index);

    void updatedBars();
    void valueAdded(int index, int count);
    void valueRemoved(int index, int count);

    void selectedBarsChanged(const QList<int> &indexes);

private:
    Q_DISABLE_COPY(QBarSet)
    friend class QBarSeries;
    friend class QBarSeriesPrivate;
};

QT_END_NAMESPACE

#endif // QBARSET_H
