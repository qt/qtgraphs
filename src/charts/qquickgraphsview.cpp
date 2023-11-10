// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <private/qquickgraphsview_p.h>
#include <QtGraphs2D/qbarseries.h>
#include <QtGraphs2D/qlineseries.h>

QT_BEGIN_NAMESPACE

QQuickGraphs2DView::QQuickGraphs2DView(QQuickItem *parent) :
    QQuickItem(parent)
{
    setFlag(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::LeftButton);
    m_shape.setParentItem(this);
}

QQuickGraphs2DView::~QQuickGraphs2DView()
{
    for (auto&& line : std::as_const(m_linePaths))
        delete line;
}

void QQuickGraphs2DView::setBackgroundColor(QColor color)
{
    auto &b = m_backgroundBrush;
    if (b.style() != Qt::SolidPattern || color != b.color()) {
        b.setStyle(Qt::SolidPattern);
        b.setColor(color);
        emit backgroundColorChanged();
    }
}

QColor QQuickGraphs2DView::backgroundColor()
{
    return m_backgroundBrush.color();
}

void QQuickGraphs2DView::addSeries(QObject *series)
{
    insertSeries(m_seriesList.size(), series);
}

void QQuickGraphs2DView::insertSeries(int index, QObject *object)
{
    if (auto series = qobject_cast<QAbstractSeries *>(object)) {
        series->setChart(this);
        if (m_seriesList.contains(series)) {
            int oldIndex = m_seriesList.indexOf(series);
            if (index != oldIndex) {
                m_seriesList.removeOne(series);
                if (oldIndex < index)
                    index--;
                m_seriesList.insert(index, series);
            }
        } else {
            int oldSize = m_seriesList.size();
            m_seriesList.insert(index, series);
            QObject::connect(series, &QAbstractSeries::update, this, &QQuickItem::update);
            if (series->theme())
                QObject::connect(series->theme(), &SeriesTheme::update, this, &QQuickItem::update);
            //QObject::connect(series, &QAbstractSeries::visibilityChanged,
            //                 this, &Abstract3DController::handleSeriesVisibilityChanged);
            //series->resetToTheme(*m_themeManager->activeTheme(), oldSize, false);
        }
//        if (series->isVisible())
//            handleSeriesVisibilityChangedBySender(series);

    }
}

void QQuickGraphs2DView::removeSeries(QObject *object)
{
    if (auto series = reinterpret_cast<QAbstractSeries *>(object)) {
        m_seriesList.removeAll(series);
        for (auto a : series->attachedAxes())
            m_axis.removeAll(a);

        //QObject::disconnect(series, &QAbstractSeries::visibilityChanged,
        //                    this, &Abstract3DController::handleSeriesVisibilityChanged);
        //series->d_func()->setController(0);
        //m_isDataDirty = true;
        //m_isSeriesVisualsDirty = true;
        //emitNeedRender();
    }
}

bool QQuickGraphs2DView::hasSeries(QObject *series)
{
    return m_seriesList.contains(series);
}


void QQuickGraphs2DView::addAxis(QAbstractAxis *axis)
{
    if (!m_axis.contains(axis)) {
        m_axis << axis;
        update();
    }
}
void QQuickGraphs2DView::removeAxis(QAbstractAxis *axis)
{
    if (m_axis.contains(axis)) {
        m_axis.removeAll(axis);
        update();
    }
}

void QQuickGraphs2DView::updateComponentSizes()
{
    if (!m_axisRenderer || !m_barsRenderer)
        return;

    m_axisRenderer->setSize(size());
    m_barsRenderer->setSize(size());
}

void QQuickGraphs2DView::componentComplete()
{
    if (!m_theme) {
        qDebug() << "Using default theme!";
        m_theme = new GraphTheme(this);
        QObject::connect(m_theme, &GraphTheme::update, this, &QQuickItem::update);
        m_theme->resetColorTheme();
    }
    QQuickItem::componentComplete();
    ensurePolished();
}

void QQuickGraphs2DView::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChange(newGeometry, oldGeometry);

    // TODO: Take margins into account here, so render items
    // sizes already match to their content.

    if (!m_axisRenderer) {
        m_axisRenderer = new AxisRenderer(this);
        m_axisRenderer->setZ(-1);
    }

    if (!m_barsRenderer)
        m_barsRenderer = new BarsRenderer(this);

    updateComponentSizes();
}

void QQuickGraphs2DView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_pointPressed && m_pressedLine->series->isPointSelected(m_pressedPointIndex)) {
        float w = width() - m_marginLeft - m_marginRight - m_axisRenderer->m_axisWidth;
        float h = height() - m_marginTop - m_marginBottom - m_axisRenderer->m_axisHeight;
        double maxVertical = m_axisRenderer->m_axisVerticalMaxValue > 0 ? 1.0 / m_axisRenderer->m_axisVerticalMaxValue : 100.0;
        double maxHorizontal = m_axisRenderer->m_axisHorizontalMaxValue > 0 ? 1.0 / m_axisRenderer->m_axisHorizontalMaxValue : 100.0;

        QPoint delta = m_pressStart - event->pos();

        qreal deltaX = -delta.x() / w / maxHorizontal;
        qreal deltaY = delta.y() / h / maxVertical;

        for (auto &&line : m_linePaths) {
            auto &&selectedPoints = line->series->selectedPoints();
            for (int index : selectedPoints) {
                QPointF point = line->series->at(index) + QPointF(deltaX, deltaY);
                line->series->replace(index, point);
            }
        }

        m_pressStart = event->pos();
        m_pointDragging = true;
    }
}

void QQuickGraphs2DView::mousePressEvent(QMouseEvent *event)
{

    m_barsRenderer->handleMousePress(event);

    const int selectionSize = 20;
    for (auto &&line : m_linePaths) {
        QRect startRect(line->shapePath->startX() - selectionSize / 2.0,
                        line->shapePath->startY() - selectionSize / 2.0,
                        selectionSize, selectionSize);
        if (startRect.contains(event->pos())) {
            if (line->series->isPointSelected(0)) {
                line->series->deselectPoint(0);
            } else {
                line->series->selectPoint(0);
            }

            m_pointPressed = true;
            m_pressStart = event->pos();
            m_pressedLine = line;
            m_pressedPointIndex = 0;
        }

        int index = 1;
        for (auto &&path : line->paths) {
            QRect rect(path->x() - selectionSize / 2.0,
                       path->y() - selectionSize / 2.0,
                       selectionSize, selectionSize);
            if (rect.contains(event->pos())) {
                m_pointPressed = true;
                m_pressStart = event->pos();
                m_pressedLine = line;
                m_pressedPointIndex = index;
            }
            index++;
        }
    }
}

void QQuickGraphs2DView::mouseReleaseEvent(QMouseEvent *event)
{
    if (!m_pointDragging && m_pointPressed && m_pressedLine) {
        const int selectionSize = 20;
        QRect rect(m_pressedLine->shapePath->startX() - selectionSize / 2.0,
                   m_pressedLine->shapePath->startY() - selectionSize / 2.0,
                   selectionSize, selectionSize);

        if (m_pressedPointIndex > 0) {
            rect = QRect(m_pressedLine->paths[m_pressedPointIndex - 1]->x() - selectionSize / 2.0,
                         m_pressedLine->paths[m_pressedPointIndex - 1]->y() - selectionSize / 2.0,
                         selectionSize, selectionSize);
        }

        if (rect.contains(event->pos())) {
            if (m_pressedLine->series->isPointSelected(m_pressedPointIndex)) {
                m_pressedLine->series->deselectPoint(m_pressedPointIndex);
            } else {
                m_pressedLine->series->selectPoint(m_pressedPointIndex);
            }
        }
    }
    m_pointPressed = false;
    m_pointDragging = false;
}

QSGNode *QQuickGraphs2DView::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *updatePaintNodeData)
{
    if (!m_backgroundNode)
        m_backgroundNode = new QSGClipNode();

    // Background node, used for clipping
    QRectF clipRect = boundingRect();
    clipRect.adjust(m_marginLeft + m_axisRenderer->m_axisWidth, m_marginTop, -m_marginRight, -m_marginBottom - m_axisRenderer->m_axisHeight);
    m_backgroundNode->setClipRect(clipRect);
    m_backgroundNode->setIsRectangular(true);
    oldNode = m_backgroundNode;

    for (auto series : std::as_const(m_seriesList)) {
        if (auto barSeries = qobject_cast<QBarSeries*>(series))
            m_barsRenderer->updateBarSeries(barSeries);

        if (auto lineSeries = qobject_cast<QLineSeries*>(series))
            updateLineSeries(lineSeries);
    }

    // Now possibly dirty theme has been taken into use
    m_theme->resetThemeDirty();

    polish();

    return oldNode;
}

void QQuickGraphs2DView::updatePolish()
{
    m_axisRenderer->handlePolish();

    // Polish for all series
    for (auto series : std::as_const(m_seriesList)) {
        if (auto barSeries = qobject_cast<QBarSeries*>(series))
            m_barsRenderer->handlePolish(barSeries);
    }

    // TODO: Move these into line renderer handlePolish()
    int lineSeriesIndex = 0;
    for (auto series : std::as_const(m_seriesList)) {
        if (auto lineSeries = qobject_cast<QLineSeries*>(series)) {
            if (!m_linePaths.contains(lineSeries)) {
                LinePath *linePath = new LinePath();
                linePath->series = lineSeries;
                linePath->shapePath = new QQuickShapePath(&m_shape);
                m_linePaths.insert(lineSeries, linePath);

                auto data = m_shape.data();
                data.append(&data, m_linePaths.value(lineSeries)->shapePath);
            }

            auto line = m_linePaths.value(lineSeries);

            int pointCount = lineSeries->points().size();
            int currentSize = line->paths.size();
            if (currentSize < pointCount - 1) {
                auto pathElements = line->shapePath->pathElements();
                for (int i = currentSize; i < pointCount - 1; ++i) {
                    auto path = new QQuickPathLine(line->shapePath);
                    pathElements.append(&pathElements, path);
                    line->paths << path;
                }
            }

            if (lineSeries->pointMarker()) {
                int markerCount = line->markers.size();
                if (markerCount < pointCount) {
                    for (int i = markerCount; i < pointCount; ++i) {
                        QQuickItem *item = qobject_cast<QQuickItem *>(lineSeries->pointMarker()->create());
                        item->setParentItem(this);
                        line->markers << item;
                    }
                }
            } else if (line->markers.size() > 0) {
                for (int i = 0; i < line->markers.size(); i++) {
                    line->markers[i]->deleteLater();
                }
                line->markers.clear();
            }

            if (!lineSeries->pointMarker()) {
                // Create markers for selected points for a line
                // without a user defined point marker

                int selectionCount = line->selections.size();
                if (selectionCount < lineSeries->selectedPoints().size()) {
                    for (int i = selectionCount; i < lineSeries->selectedPoints().size(); ++i) {
                        auto selection = new QSGDefaultInternalRectangleNode();
                        m_backgroundNode->appendChildNode(selection);
                        line->selections << selection;
                    }
                }

                auto &&selectedPoints = lineSeries->selectedPoints();
                for (int i = 0; i < line->selections.size();) {
                    if (i < selectedPoints.size()) {
                        int index = selectedPoints[i];
                        auto &pointItem = line->selections[i];

                        qreal x = line->shapePath->startX();
                        qreal y = line->shapePath->startY();

                        if (index > 0) {
                            x = line->paths[index - 1]->x();
                            y = line->paths[index - 1]->y();
                        }

                        qreal markerSize = lineSeries->markerSize();
                        pointItem->setRect(QRectF(x - markerSize / 2.0, y - markerSize / 2.0,
                                                  markerSize, markerSize));
                        QColor c = lineSeries->color();
                        if (lineSeries->selectedColor().isValid())
                            c = lineSeries->selectedColor();
                        c.setAlpha(c.alpha() * lineSeries->opacity());
                        pointItem->setColor(QColorConstants::Transparent);
                        pointItem->setPenColor(c);
                        pointItem->setPenWidth(2.0);
                        // TODO: Required because of QTBUG-117892
                        pointItem->setTopLeftRadius(-1);
                        pointItem->setTopRightRadius(-1);
                        pointItem->setBottomLeftRadius(-1);
                        pointItem->setBottomRightRadius(-1);
                        pointItem->setRadius(180.0);
                        pointItem->setAntialiasing(true);
                        pointItem->update();

                        i++;
                    } else {
                        m_backgroundNode->removeChildNode(line->selections[i]);
                        delete line->selections[i];
                        line->selections.removeAt(i);
                    }
                }
            }

            auto seriesTheme = lineSeries->theme();
            if (seriesTheme) {
                auto &&colors = seriesTheme->colors();
                if (colors.size() > 0)
                    lineSeries->setColor(colors[lineSeriesIndex % colors.size()]);
            }
            lineSeriesIndex++;
        }
    }
}




void QQuickGraphs2DView::updateLineSeries(QLineSeries *series)
{
    if (series->points().isEmpty())
        return;

    if (!m_linePaths.contains(series))
        return;

    auto line = m_linePaths.value(series);

    line->shapePath->setStrokeColor(series->color());
    line->shapePath->setStrokeWidth(series->width());
    line->shapePath->setFillColor(QColorConstants::Transparent);

    Qt::PenCapStyle capStyle = series->capStyle();
    if (capStyle == Qt::PenCapStyle::SquareCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::SquareCap);
    } else if (capStyle == Qt::PenCapStyle::FlatCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::FlatCap);
    } else if (capStyle == Qt::PenCapStyle::RoundCap) {
        line->shapePath->setCapStyle(QQuickShapePath::CapStyle::RoundCap);
    }

    // Line area width & height
    float w = width() - m_marginLeft - m_marginRight - m_axisRenderer->m_axisWidth;
    float h = height() - m_marginTop - m_marginBottom - m_axisRenderer->m_axisHeight;

    auto &&points = series->points();
    if (points.count() > 0) {
        double maxVertical = m_axisRenderer->m_axisVerticalMaxValue > 0 ? 1.0 / m_axisRenderer->m_axisVerticalMaxValue : 100.0;
        double maxHorizontal = m_axisRenderer->m_axisHorizontalMaxValue > 0 ? 1.0 / m_axisRenderer->m_axisHorizontalMaxValue : 100.0;
        for (int i = 0; i < points.count(); ++i) {
            qreal x = m_marginLeft + m_axisRenderer->m_axisWidth + w * points[i].x() * maxHorizontal;
            qreal y = m_marginTop + h - h * points[i].y() * maxVertical;
            if (i == 0) {
                line->shapePath->setStartX(x);
                line->shapePath->setStartY(y);
            } else {
                line->paths[i - 1]->setX(x);
                line->paths[i - 1]->setY(y);
            }

            if (series->pointMarker()) {
                if (line->markers[i]->property("selected").isValid())
                    line->markers[i]->setProperty("selected", series->isPointSelected(i));
                line->markers[i]->setX(x - line->markers[i]->width() / 2.0);
                line->markers[i]->setY(y - line->markers[i]->height() / 2.0);
            }
        }
    }
}






// ***** Static QQmlListProperty methods *****

QQmlListProperty<QObject> QQuickGraphs2DView::seriesList()
{
    return QQmlListProperty<QObject>(this, this,
                                          &QQuickGraphs2DView::appendSeriesFunc,
                                          &QQuickGraphs2DView::countSeriesFunc,
                                          &QQuickGraphs2DView::atSeriesFunc,
                                          &QQuickGraphs2DView::clearSeriesFunc);
}

void QQuickGraphs2DView::appendSeriesFunc(QQmlListProperty<QObject> *list, QObject *series)
{
    reinterpret_cast<QQuickGraphs2DView *>(list->data)->addSeries(series);
}

qsizetype QQuickGraphs2DView::countSeriesFunc(QQmlListProperty<QObject> *list)
{
    return reinterpret_cast<QQuickGraphs2DView *>(list->data)->getSeriesList().size();
}

QObject *QQuickGraphs2DView::atSeriesFunc(QQmlListProperty<QObject> *list, qsizetype index)
{
    return reinterpret_cast<QQuickGraphs2DView *>(list->data)->getSeriesList().at(index);
}

void QQuickGraphs2DView::clearSeriesFunc(QQmlListProperty<QObject> *list)
{
    QQuickGraphs2DView *declItems = reinterpret_cast<QQuickGraphs2DView *>(list->data);
    QList<QObject *> realList = declItems->getSeriesList();
    int count = realList.size();
    for (int i = 0; i < count; i++)
        declItems->removeSeries(realList.at(i));
}

GraphTheme *QQuickGraphs2DView::theme() const
{
    return m_theme;
}

void QQuickGraphs2DView::setTheme(GraphTheme *newTheme)
{
    if (m_theme == newTheme)
        return;

    if (m_theme)
        QObject::disconnect(m_theme, nullptr, this, nullptr);

    m_theme = newTheme;

    if (m_theme)
        QObject::connect(m_theme, &GraphTheme::update, this, &QQuickItem::update);

    emit themeChanged();
}

qreal QQuickGraphs2DView::marginTop() const
{
    return m_marginTop;
}

void QQuickGraphs2DView::setMarginTop(qreal newMarginTop)
{
    if (qFuzzyCompare(m_marginTop, newMarginTop))
        return;
    m_marginTop = newMarginTop;
    updateComponentSizes();
    update();
    emit marginTopChanged();
}

qreal QQuickGraphs2DView::marginBottom() const
{
    return m_marginBottom;
}

void QQuickGraphs2DView::setMarginBottom(qreal newMarginBottom)
{
    if (qFuzzyCompare(m_marginBottom, newMarginBottom))
        return;
    m_marginBottom = newMarginBottom;
    updateComponentSizes();
    update();
    emit marginBottomChanged();
}

qreal QQuickGraphs2DView::marginLeft() const
{
    return m_marginLeft;
}

void QQuickGraphs2DView::setMarginLeft(qreal newMarginLeft)
{
    if (qFuzzyCompare(m_marginLeft, newMarginLeft))
        return;
    m_marginLeft = newMarginLeft;
    updateComponentSizes();
    update();
    emit marginLeftChanged();
}

qreal QQuickGraphs2DView::marginRight() const
{
    return m_marginRight;
}

void QQuickGraphs2DView::setMarginRight(qreal newMarginRight)
{
    if (qFuzzyCompare(m_marginRight, newMarginRight))
        return;
    m_marginRight = newMarginRight;
    updateComponentSizes();
    update();
    emit marginRightChanged();
}

QT_END_NAMESPACE
