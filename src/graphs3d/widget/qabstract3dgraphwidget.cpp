// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphsWidgets/qabstract3dgraphwidget.h>
#include <private/q3dscene_p.h>
#include <private/qquickgraphsitem_p.h>

#ifdef Q_OS_DARWIN
#include <QtQuick3D/qquick3d.h>
#endif

QT_BEGIN_NAMESPACE

/*!
 * \class QAbstract3DGraphWidget
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The QAbstract3DGraphWidget class provides a window and render loop for
 * graphs.
 *
 * This class subclasses a QWindow and provides render loop for graphs
 * inheriting it.
 *
 * You should not need to use this class directly, but one of its subclasses
 * instead.
 *
 * Anti-aliasing is turned on by default on C++, except in OpenGL ES2
 * environments, where anti-aliasing is not supported by Qt Graphs.
 * To specify non-default anti-aliasing for a graph, give a custom surface
 * format as a constructor parameter. You can use the convenience function \c
 * qDefaultSurfaceFormat() to create the surface format object.
 *
 * \note QAbstract3DGraphWidget sets window flag \c Qt::FramelessWindowHint on by
 * default. If you want to display graph windows as standalone windows with
 * regular window frame, clear this flag after constructing the graph. For
 * example:
 *
 * \code
 *  Q3DBarsWidget *graphWindow = new Q3DBarsWidget;
 *  graphWindow->setFlags(graphWindow->flags() ^ Qt::FramelessWindowHint);
 * \endcode
 *
 * \sa Q3DBarsWidget, Q3DScatterWidget, Q3DSurfaceWidget, {Qt Graphs C++ Classes for 3D}
 */

/*!
 * \internal
 */
QAbstract3DGraphWidget::QAbstract3DGraphWidget(const QString &graphType)
{
    setResizeMode(QQuickWidget::SizeRootObjectToView);

#ifdef Q_OS_DARWIN
    // Take care of widget users (or CI) wanting to use OpenGL backend on macOS
    if (QQuickWindow::graphicsApi() == QSGRendererInterface::OpenGL)
        QSurfaceFormat::setDefaultFormat(QQuick3D::idealSurfaceFormat(4));
#endif

    const QString qmlData = QLatin1StringView(R"QML(
        import QtQuick;
        import QtGraphs;

        %1
        {
            anchors.fill: parent;
        }
    )QML").arg(graphType);
    QQmlComponent *component = new QQmlComponent(engine(), this);
    component->setData(qmlData.toUtf8(), QUrl());
    m_graphsItem.reset(qobject_cast<QQuickGraphsItem *>(component->create()));
    setContent(component->url(), component, m_graphsItem.data());

    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::selectedElementChanged,
                     this,
                     &QAbstract3DGraphWidget::selectedElementChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::msaaSamplesChanged,
                     this,
                     &QAbstract3DGraphWidget::msaaSamplesChanged);

    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::tapped,
                     this,
                     &QAbstract3DGraphWidget::tapped);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::doubleTapped,
                     this,
                     &QAbstract3DGraphWidget::doubleTapped);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::longPressed,
                     this,
                     &QAbstract3DGraphWidget::longPressed);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::dragged,
                     this,
                     &QAbstract3DGraphWidget::dragged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::wheel,
                     this,
                     &QAbstract3DGraphWidget::onWheel);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::pinch,
                     this,
                     &QAbstract3DGraphWidget::pinch);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::mouseMove,
                     this,
                     &QAbstract3DGraphWidget::mouseMove);

    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::zoomEnabledChanged,
                     this,
                     &QAbstract3DGraphWidget::zoomEnabledChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::zoomAtTargetEnabledChanged,
                     this,
                     &QAbstract3DGraphWidget::zoomAtTargetEnabledChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::rotationEnabledChanged,
                     this,
                     &QAbstract3DGraphWidget::rotationEnabledChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::selectionEnabledChanged,
                     this,
                     &QAbstract3DGraphWidget::selectionEnabledChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::queriedGraphPositionChanged,
                     this,
                     &QAbstract3DGraphWidget::queriedGraphPositionChanged);

    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::ambientLightStrengthChanged,
                     this,
                     &QAbstract3DGraphWidget::ambientLightStrengthChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::lightStrengthChanged,
                     this,
                     &QAbstract3DGraphWidget::lightStrengthChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::shadowStrengthChanged,
                     this,
                     &QAbstract3DGraphWidget::shadowStrengthChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::lightColorChanged,
                     this,
                     &QAbstract3DGraphWidget::lightColorChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::shaderGridEnabledChanged,
                     this,
                     &QAbstract3DGraphWidget::shaderGridEnabledChanged);
    QObject::connect(m_graphsItem.data(),
                     &QQuickGraphsItem::labelMarginChanged,
                     this,
                     &QAbstract3DGraphWidget::labelMarginChanged);
}

/*!
 * Destroys QAbstract3DGraphWidget.
 */
QAbstract3DGraphWidget::~QAbstract3DGraphWidget() {}

/*!
 * Adds the given \a theme to the graph. The themes added via addTheme are not
 * taken in to use directly. Only the ownership of the theme is given to the
 * graph. The \a theme must not be null or already added to another graph.
 *
 * \sa releaseTheme(), setActiveTheme()
 */
void QAbstract3DGraphWidget::addTheme(QGraphsTheme *theme)
{
    m_graphsItem->addTheme(theme);
}

/*!
 * Releases the ownership of the \a theme back to the caller, if it was added to
 * this graph. If the released \a theme is in use, a new default theme will be
 * created and set active.
 *
 * If the default theme is released and added back later, it behaves as any
 * other theme would.
 *
 * \sa addTheme(), setActiveTheme()
 */
void QAbstract3DGraphWidget::releaseTheme(QGraphsTheme *theme)
{
    m_graphsItem->releaseTheme(theme);
}

/*!
 * \property QAbstract3DGraphWidget::activeTheme
 *
 * \brief The active theme of the graph.
 *
 * Sets \a activeTheme as the active theme to be used for the graph. Implicitly
 * calls addTheme() to transfer the ownership of the theme to this graph.
 *
 * If \a activeTheme is null, a temporary default theme is created. This
 * temporary theme is destroyed if any theme is explicitly set later. Properties
 * of the theme can be modified even after setting it, and the modifications
 * take effect immediately.
 */
QGraphsTheme *QAbstract3DGraphWidget::activeTheme() const
{
    return m_graphsItem->theme();
}

void QAbstract3DGraphWidget::setActiveTheme(QGraphsTheme *activeTheme)
{
    m_graphsItem->setTheme(activeTheme);
    emit activeThemeChanged(activeTheme);
}

/*!
 * Returns the list of all added themes.
 *
 * \sa addTheme()
 */
QList<QGraphsTheme *> QAbstract3DGraphWidget::themes() const
{
    return m_graphsItem->themes();
}

/*!
 * \property QAbstract3DGraphWidget::selectionMode
 *
 * \brief Item selection mode.
 *
 * A combination of SelectionFlags. By default, \c Item.
 * Different graph types support different selection modes.
 *
 * \sa QGraphs3D::SelectionFlags
 */
QGraphs3D::SelectionFlags QAbstract3DGraphWidget::selectionMode() const
{
    return m_graphsItem->selectionMode();
}

void QAbstract3DGraphWidget::setSelectionMode(const QGraphs3D::SelectionFlags &selectionMode)
{
    m_graphsItem->setSelectionMode(selectionMode);
    emit selectionModeChanged(selectionMode);
}

/*!
 * \property QAbstract3DGraphWidget::shadowQuality
 *
 * \brief The quality of the shadow.
 *
 * One of the ShadowQuality enum values. By default, \c Medium.
 *
 * \note If setting the shadow quality to a certain level fails, the level is
 * lowered until it is successfully set. The \c shadowQualityChanged signal is
 * emitted each time a change is made.
 *
 * \sa QGraphs3D::ShadowQuality
 */
QGraphs3D::ShadowQuality QAbstract3DGraphWidget::shadowQuality() const
{
    return m_graphsItem->shadowQuality();
}

void QAbstract3DGraphWidget::setShadowQuality(const QGraphs3D::ShadowQuality &shadowQuality)
{
    m_graphsItem->setShadowQuality(shadowQuality);
    emit shadowQualityChanged(shadowQuality);
}

/*!
 * \property QAbstract3DGraphWidget::scene
 *
 * \brief The Q3DScene pointer that can be used to manipulate the scene and
 * access the scene elements.
 *
 * This property is read-only.
 */
Q3DScene *QAbstract3DGraphWidget::scene() const
{
    return (Q3DScene *) m_graphsItem->scene();
}

/*!
 * Clears selection from all attached series.
 */
void QAbstract3DGraphWidget::clearSelection()
{
    m_graphsItem->clearSelection();
}

/*!
 * Returns whether the \a series has already been added to the graph.
 */
bool QAbstract3DGraphWidget::hasSeries(QAbstract3DSeries *series) const
{
    return m_graphsItem->hasSeries(series);
}

/*!
 * Adds a QCustom3DItem \a item to the graph. Graph takes ownership of the added
 * item.
 *
 * Returns the index to the added item if the add operation was successful, -1
 * if trying to add a null item, and the index of the item if trying to add an
 * already added item.
 *
 * Items are rendered in the order they have been inserted. The rendering order
 * needs to be taken into account when having solid and transparent items.
 *
 * \sa removeCustomItems(), removeCustomItem(), removeCustomItemAt(),
 * customItems()
 */
qsizetype QAbstract3DGraphWidget::addCustomItem(QCustom3DItem *item)
{
    return m_graphsItem->addCustomItem(item);
}

/*!
 * Removes all custom items. Deletes the resources allocated to them.
 */
void QAbstract3DGraphWidget::removeCustomItems()
{
    m_graphsItem->removeCustomItems();
}

/*!
 * Removes the custom \a {item}. Deletes the resources allocated to it.
 */
void QAbstract3DGraphWidget::removeCustomItem(QCustom3DItem *item)
{
    m_graphsItem->removeCustomItem(item);
}

/*!
 * Removes all custom items at \a {position}. Deletes the resources allocated to
 * them.
 */
void QAbstract3DGraphWidget::removeCustomItemAt(const QVector3D &position)
{
    m_graphsItem->removeCustomItemAt(position);
}

/*!
 * Gets ownership of given \a item back and removes the \a item from the graph.
 *
 * \note If the same item is added back to the graph, the texture or the texture
 * file needs to be re-set.
 *
 * \sa QCustom3DItem::setTextureImage(), QCustom3DItem::setTextureFile()
 */
void QAbstract3DGraphWidget::releaseCustomItem(QCustom3DItem *item)
{
    return m_graphsItem->releaseCustomItem(item);
}

/*!
 * Returns the list of all added custom items.
 * \sa addCustomItem()
 */
QList<QCustom3DItem *> QAbstract3DGraphWidget::customItems() const
{
    // TODO: API missing in QQuickGraphsItem (QTBUG-99844)
    return {};
//    return m_graphsItem->customItems();
}

/*!
 * Can be used to query the index of the selected label after receiving \c
 * selectedElementChanged signal with any label type. Selection is valid until
 * the next \c selectedElementChanged signal.
 *
 * Returns the index of the selected label, or -1.
 *
 * \sa selectedElement
 */
int QAbstract3DGraphWidget::selectedLabelIndex() const
{
    return m_graphsItem->selectedLabelIndex();
}

/*!
 * Can be used to get the selected axis after receiving \c
 * selectedElementChanged signal with any label type. Selection is valid until
 * the next \c selectedElementChanged signal.
 *
 * Returns the pointer to the selected axis, or null.
 *
 * \sa selectedElement
 */
QAbstract3DAxis *QAbstract3DGraphWidget::selectedAxis() const
{
    return m_graphsItem->selectedAxis();
}

/*!
 * Can be used to query the index of the selected custom item after receiving \c
 * selectedElementChanged signal with QAbstract3DGraphWidget::ElementType::CustomItem
 * type. Selection is valid until the next \c selectedElementChanged signal.
 *
 * Returns the index of the selected custom item, or -1.
 *
 * \sa selectedElement
 */
qsizetype QAbstract3DGraphWidget::selectedCustomItemIndex() const
{
    return m_graphsItem->selectedCustomItemIndex();
}

/*!
 * Can be used to get the selected custom item after receiving \c
 * selectedElementChanged signal with QAbstract3DGraphWidget::ElementType::CustomItem
 * type. Ownership of the item remains with the graph. Selection is valid until
 * the next \c selectedElementChanged signal.
 *
 * Returns the pointer to the selected custom item, or null.
 *
 * \sa selectedElement
 */
QCustom3DItem *QAbstract3DGraphWidget::selectedCustomItem() const
{
    return m_graphsItem->selectedCustomItem();
}

/*!
 * \property QAbstract3DGraphWidget::selectedElement
 *
 * \brief The element selected in the graph.
 *
 * This property can be used to query the selected element type. The type is
 * valid until a new selection is made in the graph and the
 * \c selectedElementChanged signal is emitted.
 *
 * The signal can be used for example for implementing custom input handlers, as
 * demonstrated in the \l {Graph Gallery} example under \uicontrol {Scatter
 * Graph} tab.
 *
 * \sa selectedLabelIndex(), selectedAxis(), selectedCustomItemIndex(),
 * selectedCustomItem(), Q3DBarsWidget::selectedSeries(),
 * Q3DScatterWidget::selectedSeries(), Q3DSurfaceWidget::selectedSeries(),
 * Q3DScene::setSelectionQueryPosition()
 */
QGraphs3D::ElementType QAbstract3DGraphWidget::selectedElement() const
{
    return m_graphsItem->selectedElement();
}

/*!
 * Renders current frame to an image of \a imageSize.
 * Returns a shared pointer to grab resut which can be used to access the
 * rendered image when it's ready. Image is rendered with the current
 * antialiasing settings.
 *
 * \note OpenGL ES2 does not support anitialiasing.
 * \sa QQuickItem::grabToImage
 */
QSharedPointer<QQuickItemGrabResult> QAbstract3DGraphWidget::renderToImage(const QSize &imageSize)
{
    QSize renderSize = imageSize;
    if (renderSize.isEmpty())
        renderSize = size();

    return m_graphsItem->grabToImage(renderSize);
}

QGraphs3D::CameraPreset QAbstract3DGraphWidget::cameraPreset() const
{
    return m_graphsItem->cameraPreset();
}

void QAbstract3DGraphWidget::setCameraPreset(QGraphs3D::CameraPreset preset)
{
    m_graphsItem->setCameraPreset(preset);
}
/*!
 * \property QAbstract3DGraphWidget::cameraXRotation
 *
 * \brief The X-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::cameraXRotation()
{
    return m_graphsItem->cameraXRotation();
}

void QAbstract3DGraphWidget::setCameraXRotation(float rotation)
{
    m_graphsItem->setCameraXRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::cameraYRotation
 *
 * \brief The Y-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::cameraYRotation()
{
    return m_graphsItem->cameraYRotation();
}

void QAbstract3DGraphWidget::setCameraYRotation(float rotation)
{
    m_graphsItem->setCameraYRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::minCameraXRotation
 *
 * \brief The minimum X-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::minCameraXRotation()
{
    return m_graphsItem->minCameraXRotation();
}

void QAbstract3DGraphWidget::setMinCameraXRotation(float rotation)
{
    m_graphsItem->setMinCameraXRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::maxCameraXRotation
 *
 * \brief The maximum X-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::maxCameraXRotation()
{
    return m_graphsItem->maxCameraXRotation();
}

void QAbstract3DGraphWidget::setMaxCameraXRotation(float rotation)
{
    m_graphsItem->setMaxCameraXRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::minCameraYRotation
 *
 * \brief The minimum Y-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::minCameraYRotation()
{
    return m_graphsItem->minCameraYRotation();
}

void QAbstract3DGraphWidget::setMinCameraYRotation(float rotation)
{
    m_graphsItem->setMinCameraYRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::maxCameraYRotation
 *
 * \brief The maximum Y-rotation angle of the camera around the target point in degrees.
 */
float QAbstract3DGraphWidget::maxCameraYRotation()
{
    return m_graphsItem->maxCameraYRotation();
}

void QAbstract3DGraphWidget::setMaxCameraYRotation(float rotation)
{
    m_graphsItem->setMaxCameraYRotation(rotation);
}

/*!
 * \property QAbstract3DGraphWidget::zoomAtTargetEnabled
 *
 * \brief Whether zooming should change the camera target so that the zoomed point
 * of the graph stays at the same location after the zoom.
 *
 * Defaults to \c{true}.
 */
bool QAbstract3DGraphWidget::zoomAtTargetEnabled()
{
    return m_graphsItem->zoomAtTargetEnabled();
}

void QAbstract3DGraphWidget::setZoomAtTargetEnabled(bool enable)
{
    m_graphsItem->setZoomAtTargetEnabled(enable);
}

/*!
 * \property QAbstract3DGraphWidget::zoomEnabled
 *
 * \brief Whether this input handler allows graph zooming.
 *
 * Defaults to \c{true}.
 */
bool QAbstract3DGraphWidget::zoomEnabled()
{
    return m_graphsItem->zoomEnabled();
}

void QAbstract3DGraphWidget::setZoomEnabled(bool enable)
{
    m_graphsItem->setZoomEnabled(enable);
}

/*!
 * \property QAbstract3DGraphWidget::ambientLightStrength
 *
 * \brief The ambient light strength for the whole graph.
 *
 * This value determines how evenly and brightly the colors are shown throughout
 * the graph regardless of the light position.
 *
 * The value must be between \c 0.0f and \c 1.0f.
 */
float QAbstract3DGraphWidget::ambientLightStrength() const
{
    return m_graphsItem->ambientLightStrength();
}

void QAbstract3DGraphWidget::setAmbientLightStrength(float newAmbientLightStrength)
{
    m_graphsItem->setAmbientLightStrength(newAmbientLightStrength);
}

/*!
 * \property QAbstract3DGraphWidget::lightStrength
 *
 * \brief The specular light strength for the whole graph.
 *
 * The value must be between \c 0.0f and \c 10.0f.
 *
 * This value affects the light specified in Q3DScene.
 */
float QAbstract3DGraphWidget::lightStrength() const
{
    return m_graphsItem->lightStrength();
}

void QAbstract3DGraphWidget::setLightStrength(float newLightStrength)
{
    m_graphsItem->setLightStrength(newLightStrength);
}

/*!
 * \property QAbstract3DGraphWidget::shadowStrength
 *
 * \brief The shadow strength for the whole graph.
 *
 * The higher the number, the darker the shadows will be. The value must be
 * between \c 0.0 and \c 100.0.
 *
 * This value affects the light specified in Q3DScene.
 */
float QAbstract3DGraphWidget::shadowStrength() const
{
    return m_graphsItem->shadowStrength();
}

void QAbstract3DGraphWidget::setShadowStrength(float newShadowStrength)
{
    m_graphsItem->setShadowStrength(newShadowStrength);
}

/*!
 * \property QAbstract3DGraphWidget::lightColor
 *
 * \brief The color for the ambient and specular light.
 *
 * This value affects the light specified in Q3DScene.
 */
QColor QAbstract3DGraphWidget::lightColor() const
{
    return m_graphsItem->lightColor();
}

void QAbstract3DGraphWidget::setLightColor(const QColor &newLightColor)
{
    m_graphsItem->setLightColor(newLightColor);
}

/*!
 * \property QAbstract3DGraphWidget::shaderGridEnabled
 *
 * \brief Whether the grid lines are drawn inside a shader instead of lines.
 *
 * This value affects all grid lines.
 */
bool QAbstract3DGraphWidget::isShaderGridEnabled()
{
    return m_graphsItem->isShaderGridEnabled();
}

void QAbstract3DGraphWidget::setShaderGridEnabled(bool enabled)
{
    m_graphsItem->setShaderGridEnabled(enabled);
}

/*!
 * \property QAbstract3DGraphWidget::selectionEnabled
 *
 * \brief Whether this input handler allows selection from the graph.
 *
 * Defaults to \c{true}.
 */
bool QAbstract3DGraphWidget::selectionEnabled()
{
    return m_graphsItem->selectionEnabled();
}

void QAbstract3DGraphWidget::setSelectionEnabled(bool enable)
{
    m_graphsItem->setSelectionEnabled(enable);
}

/*!
 * \property QAbstract3DGraphWidget::rotationEnabled
 *
 * \brief Whether this input handler allows graph rotation.
 *
 * Defaults to \c{true}.
 */
bool QAbstract3DGraphWidget::rotationEnabled()
{
    return m_graphsItem->rotationEnabled();
}

void QAbstract3DGraphWidget::setRotationEnabled(bool enable)
{
    m_graphsItem->setRotationEnabled(enable);
}

void QAbstract3DGraphWidget::setDefaultInputHandler()
{
    m_graphsItem->setDefaultInputHandler();
}

void QAbstract3DGraphWidget::unsetDefaultInputHandler()
{
    m_graphsItem->unsetDefaultInputHandler();
}

void QAbstract3DGraphWidget::unsetDefaultTapHandler()
{
    m_graphsItem->unsetDefaultTapHandler();
}

void QAbstract3DGraphWidget::unsetDefaultDragHandler()
{
    m_graphsItem->unsetDefaultDragHandler();
}

void QAbstract3DGraphWidget::unsetDefaultWheelHandler()
{
    m_graphsItem->unsetDefaultWheelHandler();
}

void QAbstract3DGraphWidget::unsetDefaultPinchHandler()
{
    m_graphsItem->unsetDefaultPinchHandler();
}

void QAbstract3DGraphWidget::setDragButton(Qt::MouseButtons button)
{
    m_graphsItem->setDragButton(button);
}

/*!
 * \property QAbstract3DGraphWidget::cameraZoomLevel
 *
 * \brief The camera zoom level in percentage.
 *
 * The default value of \c{100.0f} means there is no zoom in or out set in the
 * camera. The value is limited by the minCameraZoomLevel and maxCameraZoomLevel
 * properties.
 *
 * \sa minCameraZoomLevel, maxCameraZoomLevel
 */
float QAbstract3DGraphWidget::cameraZoomLevel()
{
    return m_graphsItem->cameraZoomLevel();
}

void QAbstract3DGraphWidget::setCameraZoomLevel(float level)
{
    m_graphsItem->setCameraZoomLevel(level);
    m_graphsItem->update();
}

/*!
 * \property QAbstract3DGraphWidget::minCameraZoomLevel
 *
 * \brief The minimum allowed camera zoom level.
 *
 * If the minimum level is set to a new value that is higher than the existing
 * maximum level, the maximum level is adjusted to the new minimum as well.
 * If the current zoomLevel is outside the new bounds, it is adjusted as well.
 * The minCameraZoomLevel cannot be set below \c{1.0f}.
 * Defaults to \c{10.0f}.
 *
 * \sa cameraZoomLevel, maxCameraZoomLevel
 */
float QAbstract3DGraphWidget::minCameraZoomLevel()
{
    return m_graphsItem->minCameraZoomLevel();
}

void QAbstract3DGraphWidget::setMinCameraZoomLevel(float level)
{
    m_graphsItem->setMinCameraZoomLevel(level);
}

/*!
 * \property QAbstract3DGraphWidget::maxCameraZoomLevel
 *
 * \brief The maximum allowed camera zoom level.
 *
 * If the maximum level is set to a new value that is lower than the existing
 * minimum level, the minimum level is adjusted to the new maximum as well.
 * If the current cameraZoomLevel is outside the new bounds, it is adjusted as
 * well. Defaults to \c{500.0f}.
 *
 * \sa cameraZoomLevel, minCameraZoomLevel
 */
float QAbstract3DGraphWidget::maxCameraZoomLevel()
{
    return m_graphsItem->maxCameraZoomLevel();
}

void QAbstract3DGraphWidget::setMaxCameraZoomLevel(float level)
{
    m_graphsItem->setMaxCameraZoomLevel(level);
}

/*!
 * \property QAbstract3DGraphWidget::cameraTargetPosition
 *
 * \brief The camera target position as a vector or vertex in the 3D space.
 *
 * Defaults to \c {QVector3D(0.0, 0.0, 0.0)}.
 *
 * Valid coordinate values are between \c{-1.0...1.0}, where the edge values
 * indicate the edges of the corresponding axis range. Any values outside this
 * range are clamped to the edge.
 *
 * \note For bar graphs, the Y-coordinate is ignored and camera always targets a
 * point on the horizontal background.
 */
QVector3D QAbstract3DGraphWidget::cameraTargetPosition()
{
    return m_graphsItem->cameraTargetPosition();
}

void QAbstract3DGraphWidget::setCameraTargetPosition(const QVector3D &target)
{
    QVector3D newTarget = target;

    if (newTarget.x() < -1.0f)
        newTarget.setX(-1.0f);
    else if (newTarget.x() > 1.0f)
        newTarget.setX(1.0f);

    if (newTarget.y() < -1.0f)
        newTarget.setY(-1.0f);
    else if (newTarget.y() > 1.0f)
        newTarget.setY(1.0f);

    if (newTarget.z() < -1.0f)
        newTarget.setZ(-1.0f);
    else if (newTarget.z() > 1.0f)
        newTarget.setZ(1.0f);

    if (m_graphsItem->cameraTargetPosition() != newTarget) {
        if (m_graphsItem->cameraPreset() != QGraphs3D::CameraPreset::NoPreset)
            m_graphsItem->setCameraPreset(QGraphs3D::CameraPreset::NoPreset);
        m_graphsItem->setCameraTargetPosition(newTarget);
    }
}

/*!
 * \property QAbstract3DGraphWidget::wrapCameraXRotation
 *
 * \brief The behavior of the minimum and maximum limits in the X-rotation.
 *
 * If set to \c true, the X-rotation of the camera is wrapped from minimum to
 * maximum and from maximum to minimum. If set to \c false, the X-rotation of
 * the camera is limited to the sector determined by the minimum and maximum
 * values. Set to \c true by default.
 */
bool QAbstract3DGraphWidget::wrapCameraXRotation()
{
    return m_graphsItem->wrapCameraXRotation();
}

void QAbstract3DGraphWidget::setWrapCameraXRotation(bool wrap)
{
    m_graphsItem->setCameraXRotation(wrap);
}

/*!
 * \property QAbstract3DGraphWidget::wrapCameraYRotation
 *
 * \brief The behavior of the minimum and maximum limits in the Y-rotation.
 *
 * If \c true, the Y-rotation of the camera is wrapped from minimum to maximum
 * and from maximum to minimum. If \c false, the Y-rotation of the camera is
 * limited to the sector determined by the minimum and maximum values.
 * Set to \c true by default.
 */
bool QAbstract3DGraphWidget::wrapCameraYRotation()
{
    return m_graphsItem->wrapCameraYRotation();
}

void QAbstract3DGraphWidget::setWrapCameraYRotation(bool wrap)
{
    m_graphsItem->setWrapCameraYRotation(wrap);
}

/*!
 * Utility function that sets the camera rotations and distance.\a horizontal
 * and \a vertical define the camera rotations to be used. Optional \a zoom
 * parameter can be given to set the zoom percentage of the camera within the
 * bounds defined by minCameraZoomLevel and maxCameraZoomLevel properties.
 */
void QAbstract3DGraphWidget::setCameraPosition(float horizontal, float vertical, float zoom)
{
    m_graphsItem->setCameraPosition(horizontal, vertical, zoom);
}

/*!
 * \property QAbstract3DGraphWidget::msaaSamples
 *
 * \brief The number of used samples in MSAA.
 *
 * Sets the number of used MSAA samples to \a samples. The number of samples can
 * be either 0, 2, 4, or 8.
 *
 */
int QAbstract3DGraphWidget::msaaSamples() const
{
    return m_graphsItem->msaaSamples();
}

void QAbstract3DGraphWidget::setMsaaSamples(int samples)
{
    m_graphsItem->setMsaaSamples(samples);
}

void QAbstract3DGraphWidget::doPicking(QPoint point)
{
    m_graphsItem->doPicking(point);
}

/*!
 * \property QAbstract3DGraphWidget::measureFps
 *
 * \brief Whether rendering is done continuously instead of on demand.
 *
 * If \c {true}, rendering is continuous and the value of the currentFps
 * property is updated. Defaults to \c{false}.
 *
 * \sa currentFps
 */
void QAbstract3DGraphWidget::setMeasureFps(bool enable)
{
    m_graphsItem->setMeasureFps(enable);
    if (enable) {
        QObject::connect(m_graphsItem.data(),
                         &QQuickGraphsItem::currentFpsChanged,
                         this,
                         &QAbstract3DGraphWidget::currentFpsChanged);
    } else {
        QObject::disconnect(m_graphsItem.data(),
                            &QQuickGraphsItem::currentFpsChanged,
                            this,
                            &QAbstract3DGraphWidget::currentFpsChanged);
    }
}

bool QAbstract3DGraphWidget::measureFps() const
{
    return m_graphsItem->measureFps();
}

/*!
 * \property QAbstract3DGraphWidget::currentFps
 *
 * \brief The rendering results for the last second.
 *
 * The results are stored in this read-only property when FPS measuring is
 * enabled. It takes at least a second before this value is updated after
 * measuring is activated.
 *
 * \sa measureFps
 */
int QAbstract3DGraphWidget::currentFps() const
{
    return m_graphsItem->currentFps();
}

/*!
 * \property QAbstract3DGraphWidget::orthoProjection
 *
 * \brief Whether orthographic projection is used for displaying the graph.
 *
 * Defaults to \c{false}.
 * \note Shadows will be disabled when set to \c{true}.
 *
 * \sa QAbstract3DAxis::labelAutoRotation,
 */
void QAbstract3DGraphWidget::setOrthoProjection(bool enable)
{
    m_graphsItem->setOrthoProjection(enable);
}

bool QAbstract3DGraphWidget::isOrthoProjection() const
{
    return m_graphsItem->isOrthoProjection();
}

/*!
 * \property QAbstract3DGraphWidget::aspectRatio
 *
 * \brief The ratio of the graph scaling between the longest axis on the
 * horizontal plane and the y-axis.
 *
 * Defaults to \c{2.0}.
 *
 * \note Has no effect on Q3DBarsWidget.
 *
 * \sa horizontalAspectRatio
 */
void QAbstract3DGraphWidget::setAspectRatio(qreal ratio)
{
    m_graphsItem->setAspectRatio(ratio);
}

qreal QAbstract3DGraphWidget::aspectRatio() const
{
    return m_graphsItem->aspectRatio();
}

/*!
 * \property QAbstract3DGraphWidget::optimizationHint
 *
 * \brief Whether the default, static, or legacy mode is used for rendering
 * optimization.
 *
 * The default mode uses instanced rendering, and provides the full feature set
 * at the best level of performance on most systems. The static mode optimizes
 * graph rendering and is ideal for large non-changing data sets. It is slower
 * with dynamic data changes and item rotations. Selection is not optimized, so
 * using the static mode with massive data sets is not advisable. Static
 * optimization works only on scatter graphs. Legacy mode renders all items in
 * th graph individually, without instancing. It should be used only if default
 * mode does not work, i.e. if the target system does not support instancing.
 * Defaults to \l{QGraphs3D::OptimizationHint::Default}.
 *
 * \note On some environments, large graphs using static optimization may not
 * render, because all of the items are rendered using a single draw call, and
 * different graphics drivers support different maximum vertice counts per call.
 * This is mostly an issue on 32bit and OpenGL ES2 platforms.
 * To work around this issue, choose an item mesh with a low vertex count or use
 * the point mesh.
 *
 * \sa QAbstract3DSeries::mesh
 */
void QAbstract3DGraphWidget::setOptimizationHint(QGraphs3D::OptimizationHint hint)
{
    m_graphsItem->setOptimizationHint(hint);
}

QGraphs3D::OptimizationHint QAbstract3DGraphWidget::optimizationHint() const
{
    return m_graphsItem->optimizationHint();
}

/*!
 * \property QAbstract3DGraphWidget::polar
 *
 * \brief Whether horizontal axes are changed into polar axes.
 *
 * If \c {true}, the x-axis becomes the angular axis and the z-axis becomes the
 * radial axis.
 * Polar mode is not available for bar graphs.
 *
 * Defaults to \c{false}.
 *
 * \sa orthoProjection, radialLabelOffset
 */
void QAbstract3DGraphWidget::setPolar(bool enable)
{
    m_graphsItem->setPolar(enable);
}

bool QAbstract3DGraphWidget::isPolar() const
{
    return m_graphsItem->isPolar();
}

/*!
 * \property QAbstract3DGraphWidget::labelMargin
 *
 * \brief This property specifies the margin for the placement of the axis labels.
 *
 * Negative values place the labels inside the plot-area while positive values
 * place them outside the plot-area. Label automatic rotation is disabled when
 * the value is negative. Defaults to \c 0.1
 *
 * \sa QAbstract3DAxis::labelAutoRotation
 *
 */
void QAbstract3DGraphWidget::setLabelMargin(float margin)
{
    m_graphsItem->setLabelMargin(margin);
}

float QAbstract3DGraphWidget::labelMargin() const
{
    return m_graphsItem->labelMargin();
}

/*!
 * \property QAbstract3DGraphWidget::radialLabelOffset
 *
 * \brief The normalized horizontal offset for the axis labels of the radial
 * polar axis.
 *
 * The value \c 0.0 indicates that the labels should be drawn next to the
 * 0-angle angular axis grid line. The value \c 1.0 indicates that the labels
 * are drawn in their usual place at the edge of the graph background. Defaults
 * to \c 1.0.
 *
 * This property is ignored if the \l polar property value is \c{false}.
 *
 * \sa polar
 */
void QAbstract3DGraphWidget::setRadialLabelOffset(float offset)
{
    m_graphsItem->setRadialLabelOffset(offset);
}

float QAbstract3DGraphWidget::radialLabelOffset() const
{
    return m_graphsItem->radialLabelOffset();
}

/*!
 * \property QAbstract3DGraphWidget::horizontalAspectRatio
 *
 * \brief The ratio of the graph scaling between the x-axis and z-axis.
 *
 * The value of \c 0.0 indicates automatic scaling according to axis ranges.
 * Defaults to \c{0.0}.
 *
 * Has no effect on Q3DBarsWidget, which handles scaling on the horizontal plane via
 * the \l{Q3DBarsWidget::barThickness}{barThickness} and
 * \l{Q3DBarsWidget::barSpacing}{barSpacing} properties. Polar graphs also ignore this
 * property.
 *
 * \sa aspectRatio, polar, Q3DBarsWidget::barThickness, Q3DBarsWidget::barSpacing
 */
void QAbstract3DGraphWidget::setHorizontalAspectRatio(qreal ratio)
{
    m_graphsItem->setHorizontalAspectRatio(ratio);
}

qreal QAbstract3DGraphWidget::horizontalAspectRatio() const
{
    return m_graphsItem->horizontalAspectRatio();
}

/*!
 * \property QAbstract3DGraphWidget::locale
 *
 * \brief The locale used for formatting various numeric labels.
 *
 * Defaults to the \c{"C"} locale.
 *
 * \sa QValue3DAxis::labelFormat
 */
void QAbstract3DGraphWidget::setLocale(const QLocale &locale)
{
    m_graphsItem->setLocale(locale);
}

QLocale QAbstract3DGraphWidget::locale() const
{
    return m_graphsItem->locale();
}

/*!
 * \property QAbstract3DGraphWidget::queriedGraphPosition
 *
 * \brief The latest queried graph position values along each axis.
 *
 * This read-only property contains the results from
 * Q3DScene::graphPositionQuery. The values are normalized to the range \c{[-1,
 * 1]}. If the queried position was outside the graph bounds, the values will
 * not reflect the real position, but will instead indicate an undefined
 * position outside the range \c{[-1, 1]}. The value will be undefined until a
 * query is made.
 *
 * There is no single correct 3D coordinate to match a particular screen
 * position, so to be consistent, the queries are always done against the inner
 * sides of an invisible box surrounding the graph.
 *
 * \note Bar graphs only allow querying graph position at the graph floor level,
 * so the y-value is always zero for bar graphs and the valid queries can be
 * only made at screen positions that contain the floor of the graph.
 *
 * \sa Q3DScene::graphPositionQuery
 */
QVector3D QAbstract3DGraphWidget::queriedGraphPosition() const
{
    return m_graphsItem->queriedGraphPosition();
}

/*!
 * \property QAbstract3DGraphWidget::margin
 *
 * \brief The absolute value used for the space left between the edge of the
 * plottable graph area and the edge of the graph background.
 *
 * If the margin value is negative, the margins are determined automatically and
 * can vary according to the size of the items in the series and the type of the
 * graph. The value is interpreted as a fraction of the y-axis range if the
 * graph aspect ratios have not beed changed from the default values. Defaults
 * to \c{-1.0}.
 *
 * \note Setting a smaller margin for a scatter graph than the automatically
 * determined margin can cause the scatter items at the edges of the graph to
 * overlap with the graph background.
 *
 * \note On scatter and surface graphs, if the margin is small in comparison to
 * the axis label size, the positions of the edge labels of the axes are
 * adjusted to avoid overlap with the edge labels of the neighboring axes.
 */
void QAbstract3DGraphWidget::setMargin(qreal margin)
{
    m_graphsItem->setMargin(margin);
}

qreal QAbstract3DGraphWidget::margin() const
{
    return m_graphsItem->margin();
}

/*!
 * \internal
 */
bool QAbstract3DGraphWidget::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchCancel:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
        m_graphsItem->touchEvent(static_cast<QTouchEvent *>(event));
        return true;
    default:
        return QWidget::event(event);
    }
}

/*!
 * \internal
 */
void QAbstract3DGraphWidget::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    if (m_graphsItem) {
        QQuickWidget::resizeEvent(event);

        Q3DScene *scene = (Q3DScene *) m_graphsItem->scene();
        scene->d_func()->setWindowSize(QSize(width(), height()));
        scene->d_func()->setViewport(QRect(0, 0, width(), height()));
        if (m_graphsItem->sliceView() && m_graphsItem->sliceView()->isVisible())
            m_graphsItem->minimizeMainGraph();
        m_graphsItem->updateSubViews();
    }
}

void QAbstract3DGraphWidget::onWheel(QQuickWheelEvent *event)
{
    QWheelEvent *ev = new QWheelEvent(QPointF(event->x(), event->y()),
                                      QPointF(event->x(), event->y()),
                                      event->pixelDelta(),
                                      event->angleDelta(),
                                      static_cast<Qt::MouseButton>(event->buttons()),
                                      static_cast<Qt::KeyboardModifier>(event->modifiers()),
                                      event->phase(),
                                      event->inverted(),
                                      Qt::MouseEventSynthesizedBySystem,
                                      event->pointingDevice());
    emit wheel(ev);
}

/*!
 * \internal
 */
void QAbstract3DGraphWidget::mouseMoveEvent(QMouseEvent *event)
{
    QQuickWidget::mouseMoveEvent(event);
    m_graphsItem->mouseMoveEvent(event);
}

QT_END_NAMESPACE
