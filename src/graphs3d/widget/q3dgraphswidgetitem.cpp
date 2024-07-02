
// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include <QtGraphsWidgets/q3dgraphswidgetitem.h>
#include <private/q3dgraphswidgetitem_p.h>
#include <private/q3dscene_p.h>
#include <private/qquickgraphsitem_p.h>

#ifdef Q_OS_DARWIN
#include <QtQuick3D/qquick3d.h>
#endif

QT_BEGIN_NAMESPACE

/*!
 * \class Q3DGraphsWidgetItem
 * \inmodule QtGraphs
 * \ingroup graphs_3D
 * \brief The Q3DGraphsWidgetItem class provides a window and render loop for
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
 * \note Q3DGraphsWidgetItem sets window flag \c Qt::FramelessWindowHint on by
 * default. If you want to display graph windows as standalone windows with
 * regular window frame, clear this flag after constructing the graph. For
 * example:
 *
 * \code
 *  Q3DBarsWidgetItem *graphWindow = new Q3DBarsWidgetItem;
 *  graphWindow->setFlags(graphWindow->flags() ^ Qt::FramelessWindowHint);
 * \endcode
 *
 * \sa Q3DBarsWidgetItem, Q3DScatterWidgetItem, Q3DSurfaceWidgetItem, {Qt Graphs C++ Classes for 3D}
 */

/*!
 * \internal
 */
Q3DGraphsWidgetItem::Q3DGraphsWidgetItem(Q3DGraphsWidgetItemPrivate &dd, const QString &graphType)
    : QObject(dd, nullptr)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphType = graphType;

    // TODO: Remove these
    // d->m_widget = new QQuickWidget;
    // createGraph();
}

/*!
 * Destroys Q3DGraphsWidgetItem.
 */
Q3DGraphsWidgetItem::~Q3DGraphsWidgetItem()
{
}

/*!
 * Adds the given \a theme to the graph. The themes added via addTheme are not
 * taken in to use directly. Only the ownership of the theme is given to the
 * graph. The \a theme must not be null or already added to another graph.
 *
 * \sa releaseTheme(), setActiveTheme()
 */
void Q3DGraphsWidgetItem::addTheme(QGraphsTheme *theme)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->addTheme(theme);
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
void Q3DGraphsWidgetItem::releaseTheme(QGraphsTheme *theme)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->releaseTheme(theme);
}

/*!
 * \property Q3DGraphsWidgetItem::activeTheme
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
QGraphsTheme *Q3DGraphsWidgetItem::activeTheme() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->theme();
}

void Q3DGraphsWidgetItem::setActiveTheme(QGraphsTheme *activeTheme)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setTheme(activeTheme);
    emit activeThemeChanged(activeTheme);
}

/*!
 * Returns the list of all added themes.
 *
 * \sa addTheme()
 */
QList<QGraphsTheme *> Q3DGraphsWidgetItem::themes() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->themes();
}

/*!
 * \property Q3DGraphsWidgetItem::selectionMode
 *
 * \brief Item selection mode.
 *
 * A combination of SelectionFlags. By default, \c Item.
 * Different graph types support different selection modes.
 *
 * \sa QtGraphs3D::SelectionFlags
 */
QtGraphs3D::SelectionFlags Q3DGraphsWidgetItem::selectionMode() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectionMode();
}

void Q3DGraphsWidgetItem::setSelectionMode(const QtGraphs3D::SelectionFlags &selectionMode)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setSelectionMode(selectionMode);
    emit selectionModeChanged(selectionMode);
}

/*!
 * \property Q3DGraphsWidgetItem::shadowQuality
 *
 * \brief The quality of the shadow.
 *
 * One of the ShadowQuality enum values. By default, \c Medium.
 *
 * \note If setting the shadow quality to a certain level fails, the level is
 * lowered until it is successfully set. The \c shadowQualityChanged signal is
 * emitted each time a change is made.
 *
 * \sa QtGraphs3D::ShadowQuality
 */
QtGraphs3D::ShadowQuality Q3DGraphsWidgetItem::shadowQuality() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->shadowQuality();
}

void Q3DGraphsWidgetItem::setShadowQuality(const QtGraphs3D::ShadowQuality &shadowQuality)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setShadowQuality(shadowQuality);
    emit shadowQualityChanged(shadowQuality);
}

/*!
 * \property Q3DGraphsWidgetItem::scene
 *
 * \brief The Q3DScene pointer that can be used to manipulate the scene and
 * access the scene elements.
 *
 * This property is read-only.
 */
Q3DScene *Q3DGraphsWidgetItem::scene() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return (Q3DScene *) d->m_graphsItem->scene();
}

/*!
 * Clears selection from all attached series.
 */
void Q3DGraphsWidgetItem::clearSelection()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->clearSelection();
}

/*!
 * Returns whether the \a series has already been added to the graph.
 */
bool Q3DGraphsWidgetItem::hasSeries(QAbstract3DSeries *series) const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->hasSeries(series);
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
qsizetype Q3DGraphsWidgetItem::addCustomItem(QCustom3DItem *item)
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->addCustomItem(item);
}

/*!
 * Removes all custom items. Deletes the resources allocated to them.
 */
void Q3DGraphsWidgetItem::removeCustomItems()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->removeCustomItems();
}

/*!
 * Removes the custom \a {item}. Deletes the resources allocated to it.
 */
void Q3DGraphsWidgetItem::removeCustomItem(QCustom3DItem *item)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->removeCustomItem(item);
}

/*!
 * Removes all custom items at \a {position}. Deletes the resources allocated to
 * them.
 */
void Q3DGraphsWidgetItem::removeCustomItemAt(const QVector3D &position)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->removeCustomItemAt(position);
}

/*!
 * Gets ownership of given \a item back and removes the \a item from the graph.
 *
 * \note If the same item is added back to the graph, the texture or the texture
 * file needs to be re-set.
 *
 * \sa QCustom3DItem::setTextureImage(), QCustom3DItem::setTextureFile()
 */
void Q3DGraphsWidgetItem::releaseCustomItem(QCustom3DItem *item)
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->releaseCustomItem(item);
}

/*!
 * Returns the list of all added custom items.
 * \sa addCustomItem()
 */
QList<QCustom3DItem *> Q3DGraphsWidgetItem::customItems() const
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
int Q3DGraphsWidgetItem::selectedLabelIndex() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectedLabelIndex();
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
QAbstract3DAxis *Q3DGraphsWidgetItem::selectedAxis() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectedAxis();
}

/*!
 * Can be used to query the index of the selected custom item after receiving \c
 * selectedElementChanged signal with Q3DGraphsWidgetItem::ElementType::CustomItem
 * type. Selection is valid until the next \c selectedElementChanged signal.
 *
 * Returns the index of the selected custom item, or -1.
 *
 * \sa selectedElement
 */
qsizetype Q3DGraphsWidgetItem::selectedCustomItemIndex() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectedCustomItemIndex();
}

/*!
 * Can be used to get the selected custom item after receiving \c
 * selectedElementChanged signal with Q3DGraphsWidgetItem::ElementType::CustomItem
 * type. Ownership of the item remains with the graph. Selection is valid until
 * the next \c selectedElementChanged signal.
 *
 * Returns the pointer to the selected custom item, or null.
 *
 * \sa selectedElement
 */
QCustom3DItem *Q3DGraphsWidgetItem::selectedCustomItem() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectedCustomItem();
}

/*!
 * \property Q3DGraphsWidgetItem::selectedElement
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
 * selectedCustomItem(), Q3DBarsWidgetItem::selectedSeries(),
 * Q3DScatterWidgetItem::selectedSeries(), Q3DSurfaceWidgetItem::selectedSeries(),
 * Q3DScene::setSelectionQueryPosition()
 */
QtGraphs3D::ElementType Q3DGraphsWidgetItem::selectedElement() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectedElement();
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
QSharedPointer<QQuickItemGrabResult> Q3DGraphsWidgetItem::renderToImage(const QSize &imageSize)
{
    QSize renderSize = imageSize;

    Q_D(Q3DGraphsWidgetItem);
    if (renderSize.isEmpty())
        renderSize = d->m_widget->size();

    return d->m_graphsItem->grabToImage(renderSize);
}

QtGraphs3D::CameraPreset Q3DGraphsWidgetItem::cameraPreset() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->cameraPreset();
}

void Q3DGraphsWidgetItem::setCameraPreset(QtGraphs3D::CameraPreset preset)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraPreset(preset);
}
/*!
 * \property Q3DGraphsWidgetItem::cameraXRotation
 *
 * \brief The X-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::cameraXRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->cameraXRotation();
}

void Q3DGraphsWidgetItem::setCameraXRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraXRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::cameraYRotation
 *
 * \brief The Y-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::cameraYRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->cameraYRotation();
}

void Q3DGraphsWidgetItem::setCameraYRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraYRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::minCameraXRotation
 *
 * \brief The minimum X-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::minCameraXRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->minCameraXRotation();
}

void Q3DGraphsWidgetItem::setMinCameraXRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMinCameraXRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::maxCameraXRotation
 *
 * \brief The maximum X-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::maxCameraXRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->maxCameraXRotation();
}

void Q3DGraphsWidgetItem::setMaxCameraXRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMaxCameraXRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::minCameraYRotation
 *
 * \brief The minimum Y-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::minCameraYRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->minCameraYRotation();
}

void Q3DGraphsWidgetItem::setMinCameraYRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMinCameraYRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::maxCameraYRotation
 *
 * \brief The maximum Y-rotation angle of the camera around the target point in degrees.
 */
float Q3DGraphsWidgetItem::maxCameraYRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->maxCameraYRotation();
}

void Q3DGraphsWidgetItem::setMaxCameraYRotation(float rotation)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMaxCameraYRotation(rotation);
}

/*!
 * \property Q3DGraphsWidgetItem::zoomAtTargetEnabled
 *
 * \brief Whether zooming should change the camera target so that the zoomed point
 * of the graph stays at the same location after the zoom.
 *
 * Defaults to \c{true}.
 */
bool Q3DGraphsWidgetItem::zoomAtTargetEnabled()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->zoomAtTargetEnabled();
}

void Q3DGraphsWidgetItem::setZoomAtTargetEnabled(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setZoomAtTargetEnabled(enable);
}

/*!
 * \property Q3DGraphsWidgetItem::zoomEnabled
 *
 * \brief Whether this input handler allows graph zooming.
 *
 * Defaults to \c{true}.
 */
bool Q3DGraphsWidgetItem::zoomEnabled()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->zoomEnabled();
}

void Q3DGraphsWidgetItem::setZoomEnabled(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setZoomEnabled(enable);
}

/*!
 * \property Q3DGraphsWidgetItem::ambientLightStrength
 *
 * \brief The ambient light strength for the whole graph.
 *
 * This value determines how evenly and brightly the colors are shown throughout
 * the graph regardless of the light position.
 *
 * The value must be between \c 0.0f and \c 1.0f.
 */
float Q3DGraphsWidgetItem::ambientLightStrength() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->ambientLightStrength();
}

void Q3DGraphsWidgetItem::setAmbientLightStrength(float newAmbientLightStrength)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setAmbientLightStrength(newAmbientLightStrength);
}

/*!
 * \property Q3DGraphsWidgetItem::lightStrength
 *
 * \brief The specular light strength for the whole graph.
 *
 * The value must be between \c 0.0f and \c 10.0f.
 *
 * This value affects the light specified in Q3DScene.
 */
float Q3DGraphsWidgetItem::lightStrength() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->lightStrength();
}

void Q3DGraphsWidgetItem::setLightStrength(float newLightStrength)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setLightStrength(newLightStrength);
}

/*!
 * \property Q3DGraphsWidgetItem::shadowStrength
 *
 * \brief The shadow strength for the whole graph.
 *
 * The higher the number, the darker the shadows will be. The value must be
 * between \c 0.0 and \c 100.0.
 *
 * This value affects the light specified in Q3DScene.
 */
float Q3DGraphsWidgetItem::shadowStrength() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->shadowStrength();
}

void Q3DGraphsWidgetItem::setShadowStrength(float newShadowStrength)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setShadowStrength(newShadowStrength);
}

/*!
 * \property Q3DGraphsWidgetItem::lightColor
 *
 * \brief The color for the ambient and specular light.
 *
 * This value affects the light specified in Q3DScene.
 */
QColor Q3DGraphsWidgetItem::lightColor() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->lightColor();
}

void Q3DGraphsWidgetItem::setLightColor(const QColor &newLightColor)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setLightColor(newLightColor);
}

/*!
 * \property Q3DGraphsWidgetItem::shaderGridEnabled
 *
 * \brief Whether the grid lines are drawn inside a shader instead of lines.
 *
 * This value affects all grid lines.
 */
bool Q3DGraphsWidgetItem::isShaderGridEnabled()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->isShaderGridEnabled();
}

void Q3DGraphsWidgetItem::setShaderGridEnabled(bool enabled)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setShaderGridEnabled(enabled);
}

void Q3DGraphsWidgetItem::setWidget(QQuickWidget *widget)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_widget = widget;
    if (d->m_widget != nullptr) {
        d->m_widget->installEventFilter(this);
        d->createGraph();
    }
}

QQuickWidget *Q3DGraphsWidgetItem::widget() const
{
    Q_D(const Q3DGraphsWidgetItem);
    return d->m_widget;
}

/*!
 * \property Q3DGraphsWidgetItem::selectionEnabled
 *
 * \brief Whether this input handler allows selection from the graph.
 *
 * Defaults to \c{true}.
 */
bool Q3DGraphsWidgetItem::selectionEnabled()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->selectionEnabled();
}

void Q3DGraphsWidgetItem::setSelectionEnabled(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setSelectionEnabled(enable);
}

/*!
 * \property Q3DGraphsWidgetItem::rotationEnabled
 *
 * \brief Whether this input handler allows graph rotation.
 *
 * Defaults to \c{true}.
 */
bool Q3DGraphsWidgetItem::rotationEnabled()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->rotationEnabled();
}

void Q3DGraphsWidgetItem::setRotationEnabled(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setRotationEnabled(enable);
}

void Q3DGraphsWidgetItem::setDefaultInputHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setDefaultInputHandler();
}

void Q3DGraphsWidgetItem::unsetDefaultInputHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->unsetDefaultInputHandler();
}

void Q3DGraphsWidgetItem::unsetDefaultTapHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->unsetDefaultTapHandler();
}

void Q3DGraphsWidgetItem::unsetDefaultDragHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->unsetDefaultDragHandler();
}

void Q3DGraphsWidgetItem::unsetDefaultWheelHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->unsetDefaultWheelHandler();
}

void Q3DGraphsWidgetItem::unsetDefaultPinchHandler()
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->unsetDefaultPinchHandler();
}

void Q3DGraphsWidgetItem::setDragButton(Qt::MouseButtons button)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setDragButton(button);
}

/*!
 * \property Q3DGraphsWidgetItem::cameraZoomLevel
 *
 * \brief The camera zoom level in percentage.
 *
 * The default value of \c{100.0f} means there is no zoom in or out set in the
 * camera. The value is limited by the minCameraZoomLevel and maxCameraZoomLevel
 * properties.
 *
 * \sa minCameraZoomLevel, maxCameraZoomLevel
 */
float Q3DGraphsWidgetItem::cameraZoomLevel()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->cameraZoomLevel();
}

void Q3DGraphsWidgetItem::setCameraZoomLevel(float level)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraZoomLevel(level);
    d->m_graphsItem->update();
}

/*!
 * \property Q3DGraphsWidgetItem::minCameraZoomLevel
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
float Q3DGraphsWidgetItem::minCameraZoomLevel()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->minCameraZoomLevel();
}

void Q3DGraphsWidgetItem::setMinCameraZoomLevel(float level)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMinCameraZoomLevel(level);
}

/*!
 * \property Q3DGraphsWidgetItem::maxCameraZoomLevel
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
float Q3DGraphsWidgetItem::maxCameraZoomLevel()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->maxCameraZoomLevel();
}

void Q3DGraphsWidgetItem::setMaxCameraZoomLevel(float level)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMaxCameraZoomLevel(level);
}

/*!
 * \property Q3DGraphsWidgetItem::cameraTargetPosition
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
QVector3D Q3DGraphsWidgetItem::cameraTargetPosition()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->cameraTargetPosition();
}

void Q3DGraphsWidgetItem::setCameraTargetPosition(const QVector3D &target)
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

    if (Q_D(Q3DGraphsWidgetItem); d->m_graphsItem->cameraTargetPosition() != newTarget) {
        if (d->m_graphsItem->cameraPreset() != QtGraphs3D::CameraPreset::NoPreset)
            d->m_graphsItem->setCameraPreset(QtGraphs3D::CameraPreset::NoPreset);
        d->m_graphsItem->setCameraTargetPosition(newTarget);
    }
}

/*!
 * \property Q3DGraphsWidgetItem::wrapCameraXRotation
 *
 * \brief The behavior of the minimum and maximum limits in the X-rotation.
 *
 * If set to \c true, the X-rotation of the camera is wrapped from minimum to
 * maximum and from maximum to minimum. If set to \c false, the X-rotation of
 * the camera is limited to the sector determined by the minimum and maximum
 * values. Set to \c true by default.
 */
bool Q3DGraphsWidgetItem::wrapCameraXRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->wrapCameraXRotation();
}

void Q3DGraphsWidgetItem::setWrapCameraXRotation(bool wrap)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraXRotation(wrap);
}

/*!
 * \property Q3DGraphsWidgetItem::wrapCameraYRotation
 *
 * \brief The behavior of the minimum and maximum limits in the Y-rotation.
 *
 * If \c true, the Y-rotation of the camera is wrapped from minimum to maximum
 * and from maximum to minimum. If \c false, the Y-rotation of the camera is
 * limited to the sector determined by the minimum and maximum values.
 * Set to \c true by default.
 */
bool Q3DGraphsWidgetItem::wrapCameraYRotation()
{
    Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->wrapCameraYRotation();
}

void Q3DGraphsWidgetItem::setWrapCameraYRotation(bool wrap)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setWrapCameraYRotation(wrap);
}

/*!
 * Utility function that sets the camera rotations and distance.\a horizontal
 * and \a vertical define the camera rotations to be used. Optional \a zoom
 * parameter can be given to set the zoom percentage of the camera within the
 * bounds defined by minCameraZoomLevel and maxCameraZoomLevel properties.
 */
void Q3DGraphsWidgetItem::setCameraPosition(float horizontal, float vertical, float zoom)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setCameraPosition(horizontal, vertical, zoom);
}

/*!
 * \property Q3DGraphsWidgetItem::msaaSamples
 *
 * \brief The number of used samples in MSAA.
 *
 * Sets the number of used MSAA samples to \a samples. The number of samples can
 * be either 0, 2, 4, or 8.
 *
 */
int Q3DGraphsWidgetItem::msaaSamples() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->msaaSamples();
}

void Q3DGraphsWidgetItem::setMsaaSamples(int samples)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMsaaSamples(samples);
}

void Q3DGraphsWidgetItem::doPicking(QPoint point)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->doPicking(point);
}

/*!
 * \property Q3DGraphsWidgetItem::measureFps
 *
 * \brief Whether rendering is done continuously instead of on demand.
 *
 * If \c {true}, rendering is continuous and the value of the currentFps
 * property is updated. Defaults to \c{false}.
 *
 * \sa currentFps
 */
void Q3DGraphsWidgetItem::setMeasureFps(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMeasureFps(enable);
    if (enable) {
        QObject::connect(d->m_graphsItem.get(),
                         &QQuickGraphsItem::currentFpsChanged,
                         this,
                         &Q3DGraphsWidgetItem::currentFpsChanged);
    } else {
        QObject::disconnect(d->m_graphsItem.get(),
                            &QQuickGraphsItem::currentFpsChanged,
                            this,
                            &Q3DGraphsWidgetItem::currentFpsChanged);
    }
}

bool Q3DGraphsWidgetItem::measureFps() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->measureFps();
}

/*!
 * \property Q3DGraphsWidgetItem::currentFps
 *
 * \brief The rendering results for the last second.
 *
 * The results are stored in this read-only property when FPS measuring is
 * enabled. It takes at least a second before this value is updated after
 * measuring is activated.
 *
 * \sa measureFps
 */
int Q3DGraphsWidgetItem::currentFps() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->currentFps();
}

/*!
 * \property Q3DGraphsWidgetItem::orthoProjection
 *
 * \brief Whether orthographic projection is used for displaying the graph.
 *
 * Defaults to \c{false}.
 * \note Shadows will be disabled when set to \c{true}.
 *
 * \sa QAbstract3DAxis::labelAutoRotation,
 */
void Q3DGraphsWidgetItem::setOrthoProjection(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setOrthoProjection(enable);
}

bool Q3DGraphsWidgetItem::isOrthoProjection() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->isOrthoProjection();
}

/*!
 * \property Q3DGraphsWidgetItem::aspectRatio
 *
 * \brief The ratio of the graph scaling between the longest axis on the
 * horizontal plane and the y-axis.
 *
 * Defaults to \c{2.0}.
 *
 * \note Has no effect on Q3DBarsWidgetItem.
 *
 * \sa horizontalAspectRatio
 */
void Q3DGraphsWidgetItem::setAspectRatio(qreal ratio)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setAspectRatio(ratio);
}

qreal Q3DGraphsWidgetItem::aspectRatio() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->aspectRatio();
}

/*!
 * \property Q3DGraphsWidgetItem::optimizationHint
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
 * Defaults to \l{QtGraphs3D::OptimizationHint::Default}.
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
void Q3DGraphsWidgetItem::setOptimizationHint(QtGraphs3D::OptimizationHint hint)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setOptimizationHint(hint);
}

QtGraphs3D::OptimizationHint Q3DGraphsWidgetItem::optimizationHint() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->optimizationHint();
}

/*!
 * \property Q3DGraphsWidgetItem::polar
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
void Q3DGraphsWidgetItem::setPolar(bool enable)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setPolar(enable);
}

bool Q3DGraphsWidgetItem::isPolar() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->isPolar();
}

/*!
 * \property Q3DGraphsWidgetItem::labelMargin
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
void Q3DGraphsWidgetItem::setLabelMargin(float margin)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setLabelMargin(margin);
}

float Q3DGraphsWidgetItem::labelMargin() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->labelMargin();
}
/*!
 * \property Q3DGraphsWidgetItem::radialLabelOffset
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
void Q3DGraphsWidgetItem::setRadialLabelOffset(float offset)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setRadialLabelOffset(offset);
}

float Q3DGraphsWidgetItem::radialLabelOffset() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->radialLabelOffset();
}

/*!
 * \property Q3DGraphsWidgetItem::horizontalAspectRatio
 *
 * \brief The ratio of the graph scaling between the x-axis and z-axis.
 *
 * The value of \c 0.0 indicates automatic scaling according to axis ranges.
 * Defaults to \c{0.0}.
 *
 * Has no effect on Q3DBarsWidgetItem, which handles scaling on the horizontal plane via
 * the \l{Q3DBarsWidgetItem::barThickness}{barThickness} and
 * \l{Q3DBarsWidgetItem::barSpacing}{barSpacing} properties. Polar graphs also ignore this
 * property.
 *
 * \sa aspectRatio, polar, Q3DBarsWidgetItem::barThickness, Q3DBarsWidgetItem::barSpacing
 */
void Q3DGraphsWidgetItem::setHorizontalAspectRatio(qreal ratio)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setHorizontalAspectRatio(ratio);
}

qreal Q3DGraphsWidgetItem::horizontalAspectRatio() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->horizontalAspectRatio();
}

/*!
 * \property Q3DGraphsWidgetItem::locale
 *
 * \brief The locale used for formatting various numeric labels.
 *
 * Defaults to the \c{"C"} locale.
 *
 * \sa QValue3DAxis::labelFormat
 */
void Q3DGraphsWidgetItem::setLocale(const QLocale &locale)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setLocale(locale);
}

QLocale Q3DGraphsWidgetItem::locale() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->locale();
}

/*!
 * \property Q3DGraphsWidgetItem::queriedGraphPosition
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
QVector3D Q3DGraphsWidgetItem::queriedGraphPosition() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->queriedGraphPosition();
}

/*!
 * \property Q3DGraphsWidgetItem::margin
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
void Q3DGraphsWidgetItem::setMargin(qreal margin)
{
    Q_D(Q3DGraphsWidgetItem);
    d->m_graphsItem->setMargin(margin);
}

qreal Q3DGraphsWidgetItem::margin() const
{
    const Q_D(Q3DGraphsWidgetItem);
    return d->m_graphsItem->margin();
}

/*!
 * \internal
 */
bool Q3DGraphsWidgetItem::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchCancel:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd: {
        Q_D(Q3DGraphsWidgetItem);
        d->m_graphsItem->touchEvent(static_cast<QTouchEvent *>(event));
    }
        return true;
    default:
        return QObject::event(event);
    }
}

bool Q3DGraphsWidgetItem::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize) {
        Q_D(Q3DGraphsWidgetItem);
        auto ev = static_cast<QResizeEvent *>(event);
        if (d->m_graphsItem) {
            Q3DScene *scene = (Q3DScene *) d->m_graphsItem->scene();
            scene->d_func()->setWindowSize(QSize(ev->size().width(), ev->size().height()));
            d->m_graphsItem->resizeViewports(ev->size());
            if (d->m_graphsItem->sliceView() && d->m_graphsItem->sliceView()->isVisible())
                d->m_graphsItem->minimizeMainGraph();
            d->m_graphsItem->updateSubViews();
        }
        return false;
    }
    return QObject::eventFilter(obj, event);
}

/*!
 * \internal
 */

void Q3DGraphsWidgetItemPrivate::onWheel(QQuickWheelEvent *event)
{
    Q_Q(Q3DGraphsWidgetItem);

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
    emit q->wheel(ev);
}

void Q3DGraphsWidgetItemPrivate::createGraph()
{
    Q_Q(Q3DGraphsWidgetItem);
    if (m_widget == nullptr)
        return;
    m_widget->setResizeMode(QQuickWidget::SizeRootObjectToView);

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
    )QML")
                                .arg(m_graphType);
    QQmlComponent *component = new QQmlComponent(m_widget->engine(), q);
    component->setData(qmlData.toUtf8(), QUrl());
    m_graphsItem.reset(qobject_cast<QQuickGraphsItem *>(component->create()));
    m_widget->setContent(component->url(), component, m_graphsItem.get());

    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::selectedElementChanged,
                     q,
                     &Q3DGraphsWidgetItem::selectedElementChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::msaaSamplesChanged,
                     q,
                     &Q3DGraphsWidgetItem::msaaSamplesChanged);

    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::tapped,
                     q,
                     &Q3DGraphsWidgetItem::tapped);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::doubleTapped,
                     q,
                     &Q3DGraphsWidgetItem::doubleTapped);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::longPressed,
                     q,
                     &Q3DGraphsWidgetItem::longPressed);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::dragged,
                     q,
                     &Q3DGraphsWidgetItem::dragged);
    QObjectPrivate::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::wheel,
                     this,
                     &Q3DGraphsWidgetItemPrivate::onWheel);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::pinch,
                     q,
                     &Q3DGraphsWidgetItem::pinch);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::mouseMove,
                     q,
                     &Q3DGraphsWidgetItem::mouseMove);

    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::zoomEnabledChanged,
                     q,
                     &Q3DGraphsWidgetItem::zoomEnabledChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::zoomAtTargetEnabledChanged,
                     q,
                     &Q3DGraphsWidgetItem::zoomAtTargetEnabledChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::rotationEnabledChanged,
                     q,
                     &Q3DGraphsWidgetItem::rotationEnabledChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::selectionEnabledChanged,
                     q,
                     &Q3DGraphsWidgetItem::selectionEnabledChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::queriedGraphPositionChanged,
                     q,
                     &Q3DGraphsWidgetItem::queriedGraphPositionChanged);

    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::ambientLightStrengthChanged,
                     q,
                     &Q3DGraphsWidgetItem::ambientLightStrengthChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::lightStrengthChanged,
                     q,
                     &Q3DGraphsWidgetItem::lightStrengthChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::shadowStrengthChanged,
                     q,
                     &Q3DGraphsWidgetItem::shadowStrengthChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::lightColorChanged,
                     q,
                     &Q3DGraphsWidgetItem::lightColorChanged);
    QObject::connect(m_graphsItem.get(),
                     &QQuickGraphsItem::shaderGridEnabledChanged,
                     q,
                     &Q3DGraphsWidgetItem::shaderGridEnabledChanged);
    m_widget->installEventFilter(q);
}

QT_END_NAMESPACE