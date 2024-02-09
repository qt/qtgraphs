// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "barcomponent_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype BarComponent
    \inqmlmodule QtGraphs
    \ingroup graphs_qml_2D
    \inherits QQuickItem
    \brief A base element for custom bar components.

    In bar graphs, bars are by default rendered as rectangles with specified fill color,
    border color etc. By using this element, it is possible to render the bars using custom
    QML elements.

    Here is an example of how to create bars that show \c backgroundImage, overlay the bar
    with \c selectionImage once selected and render also the value of the bar into the center
    of each bar.

    \badcode
    BarSeries {
        barComponent: BarComponent {
            id: comp
            BarImage {
                id: backgroundImage
                anchors.fill: parent
                source: "images/bar_background.png"
            }
            BarImage {
                id: selectionImage
                anchors.fill: parent
                source: "images/bar_selection.png"
                visible: comp.selected
            }
            Text {
                anchors.centerIn: parent
                text: comp.barLabel
            }
        }
    }
    \endcode
*/

BarComponent::BarComponent(QQuickItem *parent) :
      QQuickItem(parent)
{
}

/*!
    \qmlproperty real BarComponent::barValue
    The value of the bar. This value comes either from the \l{BarSet::values}.
*/
qreal BarComponent::barValue() const
{
    return m_barValue;
}

void BarComponent::setBarValue(qreal value)
{
    if (qFuzzyCompare(m_barValue, value))
        return;
    m_barValue = value;
    Q_EMIT barValueChanged();
}

/*!
    \qmlproperty color BarComponent::barColor
    The fill color of the bar. This value comes either from the \l SeriesTheme
    or from \l{BarSet::color} if the \l BarSet overrides the color.
*/
QColor BarComponent::barColor() const
{
    return m_barColor;
}

void BarComponent::setBarColor(const QColor &color)
{
    if (m_barColor == color)
        return;
    m_barColor = color;
    Q_EMIT barColorChanged();
}

/*!
    \qmlproperty color BarComponent::barBorderColor
    The border color of the bar. This value comes either from the \l SeriesTheme
    or from \l{BarSet::borderColor} if the \l BarSet overrides the color.
*/
QColor BarComponent::barBorderColor() const
{
    return m_barBorderColor;
}

void BarComponent::setBarBorderColor(const QColor &color)
{
    if (m_barBorderColor == color)
        return;
    m_barBorderColor = color;
    Q_EMIT barBorderColorChanged();
}

/*!
    \qmlproperty real BarComponent::barBorderWidth
    The width of the bar border. This value comes either from the \l SeriesTheme
    or from \l{BarSet::borderWidth} if the \l BarSet overrides the width.
*/
qreal BarComponent::barBorderWidth() const
{
    return m_barBorderWidth;
}

void BarComponent::setBarBorderWidth(qreal width)
{
    if (qFuzzyCompare(m_barBorderWidth, width))
        return;
    m_barBorderWidth = width;
    Q_EMIT barBorderWidthChanged();
}

/*!
    \qmlproperty string BarComponent::barLabel
    The label of the bar. This value comes either from the \l{BarSet::label}.
*/
QString BarComponent::barLabel() const
{
    return m_barLabel;
}

void BarComponent::setBarLabel(const QString &label)
{
    if (m_barLabel == label)
        return;
    m_barLabel = label;
    Q_EMIT barLabelChanged();
}

/*!
    \qmlproperty bool BarComponent::barSelected
    This value is true when the bar is selected, meaning that the bar index
    is in \l{BarSet::selectedBars}.
*/
bool BarComponent::barSelected() const
{
    return m_barSelected;
}

void BarComponent::setBarSelected(bool selected)
{
    if (m_barSelected == selected)
        return;
    m_barSelected = selected;
    Q_EMIT barSelectedChanged();
}

QT_END_NAMESPACE
