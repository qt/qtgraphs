// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "private/qquick3drepeater_p.h"
#include "qquickgraphssurface_p.h"
#include <QtCore/QMutexLocker>

#include "declarativescene_p.h"
#include "surface3dcontroller_p.h"
#include "surfaceselectioninstancing_p.h"
#include "qvalue3daxis_p.h"
#include "qcategory3daxis_p.h"
#include "quickgraphstexturedata_p.h"

#include <QtQuick3D/private/qquick3dprincipledmaterial_p.h>
#include <QtQuick3D/private/qquick3ddefaultmaterial_p.h>
#include <QtQuick3D/private/qquick3dcustommaterial_p.h>

QT_BEGIN_NAMESPACE

QQuickGraphsSurface::QQuickGraphsSurface(QQuickItem *parent)
    : QQuickGraphsItem(parent),
      m_surfaceController(0)
{
    setAcceptedMouseButtons(Qt::AllButtons);

    // Create the shared component on the main GUI thread.
    m_surfaceController = new Surface3DController(boundingRect().toRect(), new Declarative3DScene);
    setSharedController(m_surfaceController);

    QObject::connect(m_surfaceController, &Surface3DController::selectedSeriesChanged,
                     this, &QQuickGraphsSurface::selectedSeriesChanged);
    QObject::connect(m_surfaceController, &Surface3DController::flipHorizontalGridChanged,
                     this, &QQuickGraphsSurface::handleFlipHorizontalGridChanged);
}

QQuickGraphsSurface::~QQuickGraphsSurface()
{
    QMutexLocker locker(m_nodeMutex.data());
    const QMutexLocker locker2(mutex());
    delete m_surfaceController;
    for (auto model : m_model)
        delete model;
    delete m_instancing;
    delete m_sliceInstancing;
}

QValue3DAxis *QQuickGraphsSurface::axisX() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisX());
}

void QQuickGraphsSurface::setAxisX(QValue3DAxis *axis)
{
    m_surfaceController->setAxisX(axis);
}

QValue3DAxis *QQuickGraphsSurface::axisY() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisY());
}

void QQuickGraphsSurface::setAxisY(QValue3DAxis *axis)
{
    m_surfaceController->setAxisY(axis);
}

QValue3DAxis *QQuickGraphsSurface::axisZ() const
{
    return static_cast<QValue3DAxis *>(m_surfaceController->axisZ());
}

void QQuickGraphsSurface::setAxisZ(QValue3DAxis *axis)
{
    m_surfaceController->setAxisZ(axis);
}

void QQuickGraphsSurface::handleFlatShadingEnabledChanged()
{
    auto series = static_cast<QSurface3DSeries *>(sender());
    for (auto model : m_model) {
        if (model->series == series) {
            updateModel(model);
            break;
        }
    }
}

void QQuickGraphsSurface::handleWireframeColorChanged()
{
    for (auto model : m_model) {
        QQmlListReference gridMaterialRef(model->gridModel, "materials");
        auto gridMaterial = static_cast<QQuick3DPrincipledMaterial *>(gridMaterialRef.at(0));
        QColor gridColor = model->series->wireframeColor();
        gridMaterial->setBaseColor(gridColor);

        if (sliceView()) {
            QQmlListReference gridMaterialRef(model->sliceGridModel, "materials");
            auto gridMaterial = static_cast<QQuick3DPrincipledMaterial *>(gridMaterialRef.at(0));
            gridMaterial->setBaseColor(gridColor);
        }
    }
}

void QQuickGraphsSurface::handleFlipHorizontalGridChanged(bool flip)
{
    if (!segmentLineRepeaterX())
        return;
    if (!segmentLineRepeaterZ())
        return;
    int gridLineCountX = segmentLineRepeaterX()->count();
    int subGridLineCountX = subsegmentLineRepeaterX()->count();
    int gridLineCountZ = segmentLineRepeaterZ()->count();
    int subGridLineCountZ = subsegmentLineRepeaterZ()->count();

    float factor = -1.0f;
    if (isGridUpdated())
        factor = flip ? -1.0f : 1.0f;

    for (int i = 0; i < subGridLineCountZ; i++) {
        QQuick3DNode *lineNode = static_cast<QQuick3DNode *>(subsegmentLineRepeaterZ()->objectAt(i));
        QVector3D pos = lineNode->position();
        pos.setY(pos.y() * factor);
        lineNode->setPosition(pos);
    }
    for (int i = 0; i < gridLineCountZ; i++) {
        QQuick3DNode *lineNode = static_cast<QQuick3DNode *>(segmentLineRepeaterZ()->objectAt(i));
        QVector3D pos = lineNode->position();
        pos.setY(pos.y() * factor);
        lineNode->setPosition(pos);
    }
    for (int i = 0; i < subGridLineCountX; i++) {
        QQuick3DNode *lineNode = static_cast<QQuick3DNode *>(subsegmentLineRepeaterX()->objectAt(i));
        QVector3D pos = lineNode->position();
        pos.setY(pos.y() * factor);
        lineNode->setPosition(pos);
    }
    for (int i = 0; i < gridLineCountX; i++) {
        QQuick3DNode *lineNode = static_cast<QQuick3DNode *>(segmentLineRepeaterX()->objectAt(i));
        QVector3D pos = lineNode->position();
        pos.setY(pos.y() * factor);
        lineNode->setPosition(pos);
    }

    for (int i = 0; i < repeaterX()->count(); i++) {
        auto obj = static_cast<QQuick3DNode *>(repeaterX()->objectAt(i));
        QVector3D pos = obj->position();
        pos.setY(pos.y() * factor);
        obj->setPosition(pos);
    }

    for (int i = 0; i < repeaterZ()->count(); i++) {
        auto obj = static_cast<QQuick3DNode *>(repeaterZ()->objectAt(i));
        QVector3D pos = obj->position();
        pos.setY(pos.y() * factor);
        obj->setPosition(pos);
    }

    setGridUpdated(false);
    emit flipHorizontalGridChanged(flip);
    m_surfaceController->setFlipHorizontalGridChanged(false);
}

QSurface3DSeries *QQuickGraphsSurface::selectedSeries() const
{
    return m_surfaceController->selectedSeries();
}

void QQuickGraphsSurface::setFlipHorizontalGrid(bool flip)
{
    m_surfaceController->setFlipHorizontalGrid(flip);
}

bool QQuickGraphsSurface::flipHorizontalGrid() const
{
    return m_surfaceController->flipHorizontalGrid();
}

QQmlListProperty<QSurface3DSeries> QQuickGraphsSurface::seriesList()
{
    return QQmlListProperty<QSurface3DSeries>(this, this,
                                              &QQuickGraphsSurface::appendSeriesFunc,
                                              &QQuickGraphsSurface::countSeriesFunc,
                                              &QQuickGraphsSurface::atSeriesFunc,
                                              &QQuickGraphsSurface::clearSeriesFunc);
}

void QQuickGraphsSurface::appendSeriesFunc(QQmlListProperty<QSurface3DSeries> *list,
                                           QSurface3DSeries *series)
{
    reinterpret_cast<QQuickGraphsSurface *>(list->data)->addSeries(series);
}

qsizetype QQuickGraphsSurface::countSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    return reinterpret_cast<QQuickGraphsSurface *>(list->data)->m_surfaceController->surfaceSeriesList().size();
}

QSurface3DSeries *QQuickGraphsSurface::atSeriesFunc(QQmlListProperty<QSurface3DSeries> *list,
                                                    qsizetype index)
{
    return reinterpret_cast<QQuickGraphsSurface *>(list->data)->m_surfaceController->surfaceSeriesList().at(index);
}

void QQuickGraphsSurface::clearSeriesFunc(QQmlListProperty<QSurface3DSeries> *list)
{
    QQuickGraphsSurface *declSurface = reinterpret_cast<QQuickGraphsSurface *>(list->data);
    QList<QSurface3DSeries *> realList = declSurface->m_surfaceController->surfaceSeriesList();
    int count = realList.size();
    for (int i = 0; i < count; i++)
        declSurface->removeSeries(realList.at(i));
}

void QQuickGraphsSurface::addSeries(QSurface3DSeries *series)
{
    m_surfaceController->addSeries(series);
    if (isReady())
        addModel(series);
}

void QQuickGraphsSurface::removeSeries(QSurface3DSeries *series)
{
    m_surfaceController->removeSeries(series);
    series->setParent(this); // Reparent as removing will leave series parentless
    for (int i = 0; i < m_model.size();) {
        if (m_model[i]->series == series) {
            m_model[i]->model->deleteLater();
            m_model[i]->gridModel->deleteLater();
            if (sliceView()) {
                m_model[i]->sliceModel->deleteLater();
                m_model[i]->sliceGridModel->deleteLater();
            }
            m_model.removeAt(i);
        } else {
            ++i;
        }
    }
}

void QQuickGraphsSurface::handleAxisXChanged(QAbstract3DAxis *axis)
{
    emit axisXChanged(static_cast<QValue3DAxis *>(axis));
}

void QQuickGraphsSurface::handleAxisYChanged(QAbstract3DAxis *axis)
{
    emit axisYChanged(static_cast<QValue3DAxis *>(axis));
}

void QQuickGraphsSurface::handleAxisZChanged(QAbstract3DAxis *axis)
{
    emit axisZChanged(static_cast<QValue3DAxis *>(axis));
}

void QQuickGraphsSurface::componentComplete()
{
    QQuickGraphsItem::componentComplete();

    for (auto series : m_surfaceController->surfaceSeriesList())
        addModel(series);

    QQuick3DNode *parent = rootNode();

    m_selectionPointer = new QQuick3DModel();
    m_selectionPointer->setParent(parent);
    m_selectionPointer->setParentItem(parent);
    m_selectionPointer->setSource(QUrl(QStringLiteral("#Sphere")));
    auto pointerMaterial = new QQuick3DPrincipledMaterial();
    pointerMaterial->setParent(this);
    pointerMaterial->setBaseColor(m_surfaceController->activeTheme()->singleHighlightColor());
    QQmlListReference materialRef(m_selectionPointer, "materials");
    materialRef.append(pointerMaterial);
    m_instancing = new SurfaceSelectionInstancing();
    m_instancing->setScale(QVector3D(0.001f, 0.001f, 0.001f));
    m_selectionPointer->setInstancing(m_instancing);
}

void QQuickGraphsSurface::synchData()
{
    QQuickGraphsItem::synchData();

    if (m_surfaceController->isSelectedPointChanged()) {
        if (m_surfaceController->selectionMode().testFlag(QAbstract3DGraph::SelectionItem))
            updateSelectedPoint();
        m_surfaceController->setSelectedPointChanged(false);
    }

    if (isGridUpdated() || m_surfaceController->isFlipHorizontalGridChanged())
        handleFlipHorizontalGridChanged(m_surfaceController->flipHorizontalGrid());

    if (m_surfaceController->isSurfaceTextureChanged()) {
        if (!m_surfaceController->isChangedTexturesEmpty()) {
            for (auto model : m_model) {
                if (m_surfaceController->hasSeriesToChangeTexture(model->series))
                    updateMaterial(model, true);
            }
        }
        m_surfaceController->setSurfaceTextureChanged(false);
    }
}

inline static float getDataValue(const QSurfaceDataArray &array, bool searchRow, int index)
{
    if (searchRow)
        return array.at(0)->at(index).x();
    else
        return array.at(index)->at(0).z();
}

inline static int binarySearchArray(const QSurfaceDataArray &array, int maxIndex, float limitValue, bool searchRow, bool lowBound, bool ascending)
{
    int min = 0;
    int max = maxIndex;
    int mid = 0;
    int retVal;

    while (max >= min) {
        mid = (min + max) / 2;
        float arrayValue = getDataValue(array, searchRow, mid);
        if (arrayValue == limitValue)
            return mid;
        if (ascending) {
            if (arrayValue < limitValue)
                min = mid + 1;
            else
                max = mid -1;
        } else {
            if (arrayValue > limitValue)
                min = mid + 1;
            else
                max = mid - 1;
        }
    }

    if (lowBound == ascending) {
        if (mid > max)
            retVal = mid;
        else
            retVal = min;
    } else {
        if (mid > max)
            retVal = max;
        else
            retVal = mid;
    }

    if (retVal < 0 || retVal > maxIndex) {
        retVal = -1;
    } else if (lowBound) {
        if (getDataValue(array, searchRow, retVal) < limitValue)
            retVal = -1;
    } else {
        if (getDataValue(array, searchRow, retVal) > limitValue)
            retVal = -1;
    }
    return retVal;
}

void QQuickGraphsSurface::updateGraph()
{
    for (auto model : m_model) {
        bool seriesVisible = model->series->isVisible();
        if (m_surfaceController->isSeriesVisibilityDirty()) {
            bool graphVisible = (model->model->visible() || model->gridModel->visible());

            if (seriesVisible != graphVisible && m_surfaceController->isSlicingActive()) {
                setSliceActivatedChanged(true);
            }
            if (!seriesVisible) {
                model->model->setVisible(seriesVisible);
                model->gridModel->setVisible(seriesVisible);
                if (sliceView()) {
                    model->sliceModel->setVisible(seriesVisible);
                    model->sliceGridModel->setVisible(seriesVisible);
                }
                continue;
            }
        }

        if (model->gridModel->visible() != seriesVisible)
            model->gridModel->setVisible(seriesVisible);
        if (model->model->visible() != seriesVisible)
            model->model->setVisible(seriesVisible);
        model->gridModel->setVisible(model->series->drawMode().testFlag(QSurface3DSeries::DrawWireframe));
        if (model->series->drawMode().testFlag(QSurface3DSeries::DrawSurface))
            model->model->setLocalOpacity(1.f);
        else
            model->model->setLocalOpacity(.0f);

        if (sliceView() && sliceView()->isVisible()) {
            model->sliceGridModel->setVisible(model->series->drawMode().testFlag(QSurface3DSeries::DrawWireframe));
            model->sliceModel->setVisible(model->series->drawMode().testFlag(QSurface3DSeries::DrawSurface));
        }

        updateMaterial(model);
    }

    if (m_surfaceController->selectionMode().testFlag(QAbstract3DGraph::SelectionItem))
        updateSelectedPoint();

    m_surfaceController->setSeriesVisibilityDirty(false);

    if (m_surfaceController->isDataDirty() ||
            m_surfaceController->isSeriesVisualsDirty()) {
        if (sliceView() && sliceView()->isVisible())
            updateSliceGraph();

        m_surfaceController->clearSelection();

        if (m_surfaceController->hasChangedSeriesList()) {
            handleChangedSeries();
        } else {
            for (auto model : m_model) {
                bool visible = model->series->isVisible();
                if (visible)
                    updateModel(model);
            }
        }

        m_surfaceController->setDataDirty(false);
        m_surfaceController->setSeriesVisualsDirty(false);
    }
}

void QQuickGraphsSurface::handleChangedSeries()
{
    auto changedSeries = m_surfaceController->changedSeriesList();

    for (auto series : changedSeries) {
        for (auto model : m_model) {
            if (model->series == series) {
                updateModel(model);
            }
        }
    }
}

void QQuickGraphsSurface::updateModel(SurfaceModel *model)
{
    const QSurfaceDataArray &array = *(model->series->dataProxy())->array();

    // calculateSampleRect
    QRect sampleSpace;
    if (array.size() > 0) {
        if (array.size() >= 2 && array.at(0)->size() >= 2) {
            const int maxRow = array.size() - 1;
            const int maxColumn = array.at(0)->size() - 1;

            const bool ascendingX = array.at(0)->at(0).x() < array.at(0)->at(maxColumn).x();
            const bool ascendingZ = array.at(0)->at(0).z() < array.at(maxRow)->at(0).z();

            int idx = binarySearchArray(array, maxColumn, m_surfaceController->axisX()->min(), true, true, ascendingX);
            if (idx != -1) {
                if (ascendingX)
                    sampleSpace.setLeft(idx);
                else
                    sampleSpace.setRight(idx);
            } else {
                sampleSpace.setWidth(-1);
            }

            idx = binarySearchArray(array, maxColumn, m_surfaceController->axisX()->max(), true, false, ascendingX);
            if (idx != -1) {
                if (ascendingX)
                    sampleSpace.setRight(idx);
                else
                    sampleSpace.setLeft(idx);
            } else {
                sampleSpace.setWidth(-1); // to indicate nothing needs to be shown
            }

            idx = binarySearchArray(array, maxRow, m_surfaceController->axisZ()->min(), false, true, ascendingZ);
            if (idx != -1) {
                if (ascendingZ)
                    sampleSpace.setTop(idx);
                else
                    sampleSpace.setBottom(idx);
            } else {
                sampleSpace.setWidth(-1); // to indicate nothing needs to be shown
            }

            idx = binarySearchArray(array, maxRow, m_surfaceController->axisZ()->max(), false, false, ascendingZ);
            if (idx != -1) {
                if (ascendingZ)
                    sampleSpace.setBottom(idx);
                else
                    sampleSpace.setTop(idx);
            } else {
                sampleSpace.setWidth(-1); // to indicate nothing needs to be shown
            }
        }

        int rowCount = sampleSpace.height();
        int columnCount = sampleSpace.width();
        model->rowCount = rowCount;
        model->columnCount = columnCount;

        int totalSize = rowCount * columnCount * 2;
        float uvX = 1.0f / float(columnCount - 1);
        float uvY = 1.0f / float(rowCount - 1);

        // checkDirection
        int dataDimensions = Surface3DController::BothAscending;
        if (array.at(0)->at(0).x() > array.at(0)->at(array.at(0)->size() - 1).x())
            dataDimensions |= Surface3DController::XDescending;
        if (static_cast<QValue3DAxis *>(m_surfaceController->axisX())->reversed())
            dataDimensions ^= Surface3DController::XDescending;

        if (array.at(0)->at(0).z() > array.at(array.size() - 1)-> at(0).z())
            dataDimensions |= Surface3DController::ZDescending;
        if (static_cast<QValue3DAxis *>(m_surfaceController->axisZ())->reversed())
            dataDimensions ^= Surface3DController::ZDescending;

        m_surfaceController->setDataDimensions(static_cast<Surface3DController::DataDimensions>(dataDimensions));

        model->vertices.clear();
        model->vertices.reserve(totalSize);

        bool isFlatShadingEnabled = model->series->isFlatShadingEnabled();

        QVector3D boundsMin(0.0f, 0.0f, 0.0f);
        QVector3D boundsMax(0.0f, 0.0f, 0.0f);

        int startColumn = 0;
        int startRow = 0;
        int rowLimit = rowCount - 1;
        int colLimit = columnCount - 1;

        QAbstract3DAxis *axis = m_surfaceController->axisZ();
        float z = (*array.at(startRow)).at(0).z();
        while (z < axis->min())
            z = (*array.at(++startRow)).at(0).z();

        z = (*array.at(rowLimit)).at(0).z();
        while (z > axis->max())
            z = (*array.at(--rowLimit)).at(0).z();

        axis = m_surfaceController->axisX();
        float x = (*array.at(0)).at(startColumn).x();
        while (x < axis->min())
            x = (*array.at(0)).at(++startColumn).x();

        x = (*array.at(0)).at(colLimit).x();
        while (x > axis->max())
            x = (*array.at(0)).at(--colLimit).x();

        bool isPolar = m_surfaceController->isPolar();
        for (int i = startRow ; i < rowLimit + startRow + 1 ; i++) {
            const QSurfaceDataRow &row = *array.at(i);
            for (int j = startColumn ; j < colLimit + startColumn + 1 ; j++) {
                // getNormalizedVertex
                QVector3D pos = getNormalizedVertex(row.at(j), isPolar, false);
                SurfaceVertex vertex;
                vertex.position = pos;
                vertex.normal = QVector3D(0, 0, 0);
                vertex.uv = QVector2D(j * uvX, i * uvY);
                vertex.coord = QPoint(i, j);
                model->vertices.push_back(vertex);
                if (boundsMin.isNull())
                    boundsMin = pos;
                else
                    boundsMin = QVector3D(qMin(boundsMin.x(), pos.x()), qMin(boundsMin.y(), pos.y()), qMin(boundsMin.z(), pos.z()));
                if (boundsMax.isNull())
                    boundsMax = pos;
                else
                    boundsMax = QVector3D(qMax(boundsMax.x(), pos.x()), qMax(boundsMax.y(), pos.y()), qMax(boundsMax.z(), pos.z()));
            }
        }

        model->boundsMin = boundsMin;
        model->boundsMax = boundsMax;

        int totalIndex = 0;

        model->indices.clear();

        if (isFlatShadingEnabled) {
            createCoarseVertices(model, 0, 0, colLimit, rowLimit);
        } else {
            if (dataDimensions == Surface3DController::BothAscending || dataDimensions == Surface3DController::XDescending) {
                for (int row = 0 ; row < rowLimit ; row++)
                    createSmoothNormalBodyLine(model, totalIndex, row * columnCount);
                createSmoothNormalUpperLine(model, totalIndex);
            }
            else {
                createSmoothNormalUpperLine(model, totalIndex);
                for (int row = 1 ; row < rowCount ; row++)
                    createSmoothNormalBodyLine(model, totalIndex, row * columnCount);
            }

            createSmoothIndices(model, 0, 0, colLimit, rowLimit);
        }

        auto geometry = model->model->geometry();
        geometry->vertexData().clear();
        if (isFlatShadingEnabled) {
            QByteArray vertexBuffer(reinterpret_cast<char *>(model->coarceVertices.data()),
                                    model->coarceVertices.size() * sizeof(SurfaceVertex));
            geometry->setVertexData(vertexBuffer);
        } else {
            QByteArray vertexBuffer(reinterpret_cast<char *>(model->vertices.data()),
                                    model->vertices.size() * sizeof(SurfaceVertex));
            geometry->setVertexData(vertexBuffer);
        }
        QByteArray indexBuffer(reinterpret_cast<char *>(model->indices.data()),
                               model->indices.size() * sizeof(quint32));
        geometry->setIndexData(indexBuffer);
        geometry->setBounds(boundsMin, boundsMax);
        geometry->update();

        updateMaterial(model);

        createGridlineIndices(model, 0, 0, colLimit, rowLimit);

        auto gridGeometry = model->gridModel->geometry();
        gridGeometry->vertexData().clear();
        QByteArray vertexBuffer(reinterpret_cast<char *>(model->vertices.data()),
                                model->vertices.size() * sizeof(SurfaceVertex));
        gridGeometry->setVertexData(vertexBuffer);
        QByteArray gridIndexBuffer(reinterpret_cast<char *>(model->gridIndices.data()),
                                   model->gridIndices.size() * sizeof(quint32));
        gridGeometry->setIndexData(gridIndexBuffer);
        gridGeometry->setBounds(boundsMin, boundsMax);
        gridGeometry->update();

        QQmlListReference gridMaterialRef(model->gridModel, "materials");
        auto gridMaterial = static_cast<QQuick3DPrincipledMaterial *>(gridMaterialRef.at(0));
        QColor gridColor = model->series->wireframeColor();
        gridMaterial->setBaseColor(gridColor);
    }

    updateSelectedPoint();
}

void QQuickGraphsSurface::updateMaterial(SurfaceModel *model, bool texturedModel)
{
    QQmlListReference materialRef(model->model, "materials");
    QQuick3DMaterial *material = static_cast<QQuick3DMaterial *>(materialRef.at(0));
    if (!texturedModel) {
        if (m_surfaceController->isSeriesVisualsDirty()) {
            if (model->series->colorStyle() == Q3DTheme::ColorStyleUniform) {
                materialRef.clear();
                QQuick3DPrincipledMaterial *material = new QQuick3DPrincipledMaterial();
                material->setParent(model->model);
                material->setParentItem(model->model);
                material->setCullMode(QQuick3DMaterial::NoCulling);
                material->setBaseColor(model->series->baseColor());
                materialRef.append(material);
            } else {
                QQuick3DCustomMaterial *material = nullptr;
                if (model->series->colorStyle() == Q3DTheme::ColorStyleObjectGradient) {
                    material = createQmlCustomMaterial(QStringLiteral(":/materials/ObjectGradientSurfaceMaterial"));
                    float minY = model->boundsMin.y();
                    float maxY = model->boundsMax.y();
                    float range = maxY - minY;
                    material->setProperty("gradientMin", -(minY / range));
                    material->setProperty("gradientHeight", 1.0f / range);
                } else {
                    material = createQmlCustomMaterial(QStringLiteral(":/materials/RangeGradientMaterial"));
                }
                material->setParent(model->model);
                material->setParentItem(model->model);
                material->setCullMode(QQuick3DMaterial::NoCulling);
                QVariant textureInputAsVariant = material->property("custex");
                QQuick3DShaderUtilsTextureInput *textureInput = textureInputAsVariant.value<QQuick3DShaderUtilsTextureInput *>();
                auto textureData = static_cast<QuickGraphsTextureData *>(model->texture->textureData());
                textureData->createGradient(model->series->baseGradient());
                textureInput->setTexture(model->texture);
                material->update();
                delete model->customMaterial;
                model->customMaterial = material;
                materialRef.clear();
                materialRef.append(model->customMaterial);
            }
        }
    } else {
        if (!model->principledMaterial) {
            QQuick3DPrincipledMaterial *principledMaterial = new QQuick3DPrincipledMaterial();
            principledMaterial->setParent(model->model);
            principledMaterial->setParentItem(model->model);
            principledMaterial->setCullMode(QQuick3DMaterial::NoCulling);

            model->principledMaterial = principledMaterial;
        }
        if (material != model->principledMaterial) {
            materialRef.clear();
            materialRef.append(model->principledMaterial);
        }
        QQuick3DTexture *texture = model->principledMaterial->baseColorMap();
        if (!texture) {
            texture = new QQuick3DTexture();
            texture->setParent(model->model);
            texture->setParentItem(model->model);
            texture->setRotationUV(180.0f);
            texture->setFlipU(true);
            model->principledMaterial->setBaseColorMap(texture);
        }
        if (!model->series->textureFile().isEmpty()) {
            texture->setSource(QUrl::fromLocalFile(model->series->textureFile()));
        } else if (!model->series->texture().isNull()) {
            QImage image = model->series->texture();
            auto textureData = static_cast<QuickGraphsTextureData *>(model->texture->textureData());
            textureData->setTextureData(QByteArray(reinterpret_cast<const char*>(image.bits()),
                                                   image.sizeInBytes()));
        } else {
            texture->setSource(QUrl());
        }
    }
}

QVector3D QQuickGraphsSurface::getNormalizedVertex(const QSurfaceDataItem &data, bool polar, bool flipXZ)
{
    Q_UNUSED(flipXZ);

    QValue3DAxis* axisX = static_cast<QValue3DAxis *>(m_surfaceController->axisX());
    QValue3DAxis* axisY = static_cast<QValue3DAxis *>(m_surfaceController->axisY());
    QValue3DAxis* axisZ = static_cast<QValue3DAxis *>(m_surfaceController->axisZ());

    float normalizedX = axisX->positionAt(data.x());
    float normalizedY;
    float normalizedZ = axisZ->positionAt(data.z());
    // TODO : Need to handle, flipXZ

    float scale, translate;
    if (polar) {
        float angle = normalizedX * M_PI * 2.0f;
        float radius = normalizedZ;
        normalizedX = radius * qSin(angle) * 1.0f;
        normalizedZ = -(radius * qCos(angle)) * 1.0f;
    } else {
        scale = translate = this->scaleWithBackground().x();
        normalizedX = normalizedX * scale * 2.0f - translate;
        scale = translate = this->scaleWithBackground().z();
        normalizedZ = normalizedZ * -scale * 2.0f + translate;
    }
    scale = translate = this->scale().y();
    normalizedY = axisY->positionAt(data.y()) * scale * 2.0f - translate;
    return QVector3D(normalizedX, normalizedY, normalizedZ);
}

inline static QVector3D normal(const QVector3D &a, const QVector3D &b, const QVector3D &c)
{
    QVector3D v1 = b - a;
    QVector3D v2 = c - a;
    QVector3D normal = QVector3D::crossProduct(v1, v2);

    return normal;
}

void QQuickGraphsSurface::updateSliceGraph()
{
    QQuickGraphsItem::updateSliceGraph();

    m_surfaceController->setSelectedPointChanged(true);

    if (!sliceView()->isVisible())
        return;

    auto selectionMode = m_surfaceController->selectionMode();

    for (auto model : m_model) {
        if (!model->series->isVisible())
            continue;

        QVector<SurfaceVertex> selectedSeries;

        int rowCount = model->rowCount;
        int columnCount = model->columnCount;

        int indexCount = 0;
        if (selectionMode.testFlag(QAbstract3DGraph::SelectionRow)) {
            int selectedRow = model->selectedVertex.coord.x() * columnCount;
            selectedSeries.reserve(columnCount * 2);
            QVector<SurfaceVertex> list;
            for (int i = 0; i < columnCount; i++) {
                SurfaceVertex vertex = model->vertices.at(selectedRow + i);
                vertex.normal = QVector3D(.0f, .0f, 1.f);
                vertex.position.setY(vertex.position.y() - .025f);
                vertex.position.setZ(.0f);
                selectedSeries.append(vertex);
                vertex.position.setY(vertex.position.y() + .05f);
                list.append(vertex);
            }
            selectedSeries.append(list);
            indexCount = columnCount - 1;
        }

        if (selectionMode.testFlag(QAbstract3DGraph::SelectionColumn)) {
            int selectedColumn = model->selectedVertex.coord.y();
            selectedSeries.reserve(rowCount * 2);
            QVector<SurfaceVertex> list;
            for (int i = 0; i < rowCount; i++) {
                SurfaceVertex vertex = model->vertices.at((i * columnCount) + selectedColumn);
                vertex.normal = QVector3D(.0f, .0f, 1.0f);
                vertex.position.setX(-vertex.position.z());
                vertex.position.setY(vertex.position.y() - .025f);
                vertex.position.setZ(0);
                selectedSeries.append(vertex);
                vertex.position.setY(vertex.position.y() + .05f);
                list.append(vertex);
            }
            selectedSeries.append(list);
            indexCount = rowCount - 1;
        }

        QVector<quint32> indices;
        indices.reserve(indexCount * 6);
        for (int i = 0; i < indexCount; i++) {
            indices.push_back(i + 1);
            indices.push_back(i + indexCount + 1);
            indices.push_back(i);

            indices.push_back(i + indexCount + 2);
            indices.push_back(i + indexCount + 1);
            indices.push_back(i + 1);
        }

        auto geometry = model->sliceModel->geometry();
        geometry->vertexData().clear();
        geometry->indexData().clear();
        QByteArray vertexBuffer(reinterpret_cast<char *>(selectedSeries.data()),
                                selectedSeries.size() * sizeof(SurfaceVertex));
        geometry->setVertexData(vertexBuffer);
        QByteArray indexBuffer(reinterpret_cast<char *>(indices.data()),
                               indices.size() * sizeof(quint32));
        geometry->setIndexData(indexBuffer);
        geometry->update();

        geometry = model->sliceGridModel->geometry();
        geometry->vertexData().clear();
        geometry->indexData().clear();
        geometry->setVertexData(vertexBuffer);

        QVector<quint32> gridIndices;
        gridIndices.reserve(indexCount * 4);
        for (int i = 0; i < indexCount; i++) {
            gridIndices.push_back(i);
            gridIndices.push_back(i + indexCount + 1);

            gridIndices.push_back(i);
            gridIndices.push_back(i + 1);
        }
        geometry->indexData().clear();
        QByteArray gridIndexBuffer(reinterpret_cast<char *>(gridIndices.data()),
                                   gridIndices.size() * sizeof(quint32));
        geometry->setIndexData(gridIndexBuffer);
        geometry->update();

        QQmlListReference gridMaterialRef(model->sliceGridModel, "materials");
        auto gridMaterial = static_cast<QQuick3DPrincipledMaterial *>(gridMaterialRef.at(0));
        QColor gridColor = model->series->wireframeColor();
        gridMaterial->setBaseColor(gridColor);

        updateSelectedPoint();
    }
}

void QQuickGraphsSurface::createSmoothNormalBodyLine(SurfaceModel *model, int &totalIndex, int column)
{
    int columnCount = model->columnCount;
    int colLimit = columnCount - 1;
    Surface3DController::DataDimensions dataDimensions = m_surfaceController->dataDimensions();
    if (dataDimensions == Surface3DController::BothAscending) {
        int end = colLimit + column;
        for (int j = column ; j < end ; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j + 1).position,
                                   model->vertices.at(j + columnCount).position);
            model->vertices.replace(totalIndex++, vertex);
        }
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(end).position,
                               model->vertices.at(end + columnCount).position,
                               model->vertices.at(end - 1).position);
        model->vertices.replace(totalIndex++, vertex);
    } else if (dataDimensions == Surface3DController::XDescending) {
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(column).position,
                               model->vertices.at(column + columnCount).position,
                               model->vertices.at(column + 1).position);
        model->vertices.replace(totalIndex++, vertex);
        int end = column + columnCount;
        for (int j = column + 1 ; j < end ; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j - 1).position,
                                   model->vertices.at(j + columnCount).position);
            model->vertices.replace(totalIndex++, vertex);
        }
    } else if (dataDimensions == Surface3DController::ZDescending) {
        int end = colLimit + column;
        for (int j = column; j < end ; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j + 1).position,
                                   model->vertices.at(j - columnCount).position);
            model->vertices.replace(totalIndex++, vertex);
        }
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(end).position,
                               model->vertices.at(end - columnCount).position,
                               model->vertices.at(end - 1).position);
        model->vertices.replace(totalIndex++, vertex);
    } else {
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(column).position,
                               model->vertices.at(column - columnCount).position,
                               model->vertices.at(column + 1).position);
        model->vertices.replace(totalIndex++, vertex);
        int end = column + columnCount;
        for (int j = 0 ; j < end ; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j-1).position,
                                   model->vertices.at(j - columnCount).position);
            model->vertices.replace(totalIndex++, vertex);
        }
    }
}

void QQuickGraphsSurface::createSmoothNormalUpperLine(SurfaceModel *model, int &totalIndex)
{
    int columnCount = model->columnCount;
    int rowCount = model->rowCount;
    Surface3DController::DataDimensions dataDimensions = m_surfaceController->dataDimensions();

    if (dataDimensions == Surface3DController::BothAscending) {
        int lineEnd = rowCount * columnCount - 1;
        for (int j = (rowCount - 1) * columnCount; j < lineEnd; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j - columnCount).position,
                                   model->vertices.at(j + 1).position);
            model->vertices.replace(totalIndex++, vertex);
        }
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(lineEnd).position,
                               model->vertices.at(lineEnd - 1).position,
                               model->vertices.at(lineEnd - columnCount).position);
        model->vertices.replace(totalIndex++, vertex);
    } else if (dataDimensions == Surface3DController::XDescending) {
        int lineStart = (rowCount - 1) * columnCount;
        int lineEnd = rowCount * columnCount;
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(lineStart).position,
                               model->vertices.at(lineStart + 1).position,
                               model->vertices.at(lineStart - columnCount).position);
        model->vertices.replace(totalIndex++, vertex);
        for (int j = lineStart + 1; j < lineEnd; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j - columnCount).position,
                                   model->vertices.at(j - 1).position);
            model->vertices.replace(totalIndex++, vertex);
        }
    } else if (dataDimensions == Surface3DController::ZDescending) {
        int colLimit = columnCount - 1;
        for (int j = 0; j < colLimit; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j + columnCount).position,
                                   model->vertices.at(j + 1).position);
            model->vertices.replace(totalIndex++, vertex);
        }
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(colLimit).position,
                               model->vertices.at(colLimit - 1).position,
                               model->vertices.at(colLimit + columnCount).position);
        model->vertices.replace(totalIndex++, vertex);
    } else { // BothDescending
        SurfaceVertex vertex = model->vertices.at(totalIndex);
        vertex.normal = normal(model->vertices.at(0).position,
                               model->vertices.at(1).position,
                               model->vertices.at(columnCount).position);
        model->vertices.replace(totalIndex++, vertex);
        for (int j = 1; j < columnCount; j++) {
            SurfaceVertex vertex = model->vertices.at(totalIndex);
            vertex.normal = normal(model->vertices.at(j).position,
                                   model->vertices.at(j + columnCount).position,
                                   model->vertices.at(j - 1).position);
            model->vertices.replace(totalIndex++, vertex);
        }
    }
}

void QQuickGraphsSurface::createSmoothIndices(SurfaceModel *model, int x, int y, int endX, int endY)
{
    int columnCount = model->columnCount;
    int rowCount = model->rowCount;
    Surface3DController::DataDimensions dataDimensions = m_surfaceController->dataDimensions();

    if (endX >= columnCount)
        endX = columnCount - 1;
    if (endY >= rowCount)
        endY = rowCount - 1;
    if (x > endX)
        x = endX - 1;
    if (y > endY)
        y = endY - 1;

    int indexCount = 6 * (endX - x) * (endY - y);

    QVector<quint32> *indices = &model->indices;

    indices->clear();
    indices->resize(indexCount);

    int rowEnd = endY * columnCount;
    for (int row = y * columnCount ; row < rowEnd ; row += columnCount) {
        for (int j = x ; j < endX ; j++) {
            if (dataDimensions == Surface3DController::BothAscending
                    || dataDimensions == Surface3DController::BothDescending) {
                indices->push_back(row + j + 1);
                indices->push_back(row + columnCount + j);
                indices->push_back(row + j);

                indices->push_back(row + columnCount + j + 1);
                indices->push_back(row + columnCount + j);
                indices->push_back(row + j + 1);
            } else if (dataDimensions == Surface3DController::XDescending) {
                indices->push_back(row + columnCount + j);
                indices->push_back(row + columnCount + j + 1);
                indices->push_back(row + j);

                indices->push_back(row + j);
                indices->push_back(row + columnCount + j + 1);
                indices->push_back(row + j + 1);
            } else {
                indices->push_back(row + columnCount + j);
                indices->push_back(row + columnCount + j + 1);
                indices->push_back(row + j + 1);

                indices->push_back(row + j);
                indices->push_back(row + columnCount + j + 1);
                indices->push_back(row + j + 1);
            }
        }
    }
}

void QQuickGraphsSurface::createCoarseVertices(SurfaceModel *model, int x, int y, int endX, int endY)
{
    int columnCount = model->columnCount;
    int rowCount = model->rowCount;
    Surface3DController::DataDimensions dataDimensions = m_surfaceController->dataDimensions();

    if (endX >= columnCount)
        endX = columnCount - 1;
    if (endY >= rowCount)
        endY = rowCount - 1;
    if (x > endX)
        x = endX - 1;
    if (y > endY)
        y = endY - 1;

    int indexCount = 6 * (endX - x) * (endY - y);
    model->indices.clear();
    model->indices.resize(indexCount);

    model->coarceVertices.clear();

    int index = 0;
    int rowEnd = endY * columnCount;

    int i1, i2, i3;
    SurfaceVertex v1, v2, v3;
    QVector3D normalVector;

    for (int row = y * columnCount; row < rowEnd; row += columnCount) {
        for (int j = x; j < endX; j++) {
            if (dataDimensions == Surface3DController::BothAscending
                    || dataDimensions == Surface3DController::BothDescending) {
                i1 = row + j + 1, i2 = row + columnCount + j, i3 = row + j;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);

                i1 = row + columnCount + j + 1, i2 = row + columnCount + j, i3 = row + j + 1;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
            } else if (dataDimensions == Surface3DController::XDescending) {
                i1 = row + columnCount + j, i2 = row + columnCount + j + 1, i3 = row + j;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);

                i1 = row + j, i2 = row + columnCount + j + 1, i3 = row + j + 1;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
            } else {
                i1 = row + j, i2 = row + j + 1, i3 = row + j + columnCount;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);

                i1 = row + j + 1, i2 = row + columnCount + j + 1, i3 = row + j + columnCount;
                v1 = model->vertices.at(i1);
                v2 = model->vertices.at(i2);
                v3 = model->vertices.at(i3);
                normalVector = normal(v1.position, v2.position, v3.position);
                v1.normal = normalVector;
                v2.normal = normalVector;
                v3.normal = normalVector;
                model->coarceVertices.push_back(v1);
                model->coarceVertices.push_back(v2);
                model->coarceVertices.push_back(v3);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
                model->indices.push_back(index++);
            }
        }
    }
}

void QQuickGraphsSurface::createGridlineIndices(SurfaceModel *model, int x, int y, int endX, int endY)
{
    int columnCount = model->columnCount;
    int rowCount = model->rowCount;

    if (endX >= columnCount)
        endX = columnCount - 1;
    if (endY >= rowCount)
        endY = rowCount - 1;
    if (x > endX)
        x = endX - 1;
    if (y > endY)
        y = endY - 1;

    int nColumns = endX - x + 1;
    int nRows = endY - y + 1;

    int gridIndexCount = 2 * nColumns * (nRows - 1) + 2 * nRows * (nColumns - 1);
    model->gridIndices.clear();
    model->gridIndices.resize(gridIndexCount);

    for (int i = y, row = columnCount * y ; i <= endY ; i++, row += columnCount) {
        for (int j = x ; j < endX ; j++) {
            model->gridIndices.push_back(row + j);
            model->gridIndices.push_back(row + j + 1);
        }
    }
    for (int i = y, row = columnCount * y ; i < endY ; i++, row += columnCount) {
        for (int j = x ; j <= endX ; j++) {
            model->gridIndices.push_back(row + j);
            model->gridIndices.push_back(row + j + columnCount);
        }
    }
}

bool QQuickGraphsSurface::handleMousePressedEvent(QMouseEvent *event)
{
    if (!QQuickGraphsItem::handleMousePressedEvent(event))
        return true;

    if (!sliceView())
        createSliceView();

    if (Qt::LeftButton == event->button())
        doPicking(event->pos());

    return true;
}

bool QQuickGraphsSurface::handleTouchEvent(QTouchEvent *event)
{
    if (!QQuickGraphsItem::handleTouchEvent(event))
        return true;

    if (!sliceView())
        createSliceView();

    if (scene()->selectionQueryPosition() != scene()->invalidSelectionPoint()
        && !event->isUpdateEvent()) {
        doPicking(event->point(0).position());
        scene()->setSelectionQueryPosition(scene()->invalidSelectionPoint());
    }

    return true;
}

bool QQuickGraphsSurface::doPicking(const QPointF &position)
{
    if (!QQuickGraphsItem::doPicking(position))
        return false;

    auto pickResult = pickAll(position.x(), position.y());
    QVector3D pickedPos(0.0f, 0.0f, 0.0f);
    QQuick3DModel *pickedModel = nullptr;

    auto selectionMode = m_surfaceController->selectionMode();
    if (!selectionMode.testFlag(QAbstract3DGraph::SelectionNone)) {
        for (auto picked : pickResult) {
            if (picked.objectHit()
                    && picked.objectHit()->objectName().contains(QStringLiteral("SurfaceModel"))) {
                pickedPos = picked.position();
                pickedModel = picked.objectHit();
                if (!pickedPos.isNull())
                    break;
            }
        }

        if (!pickedPos.isNull()) {
            float min = -1.0f;

            for (auto model : m_model) {
                if (!model->series->isVisible())
                    continue;

                model->picked = (model->model == pickedModel);

                if (!selectionMode.testFlag(QAbstract3DGraph::SelectionMultiSeries)
                    && !model->picked) {
                    continue;
                }

                SurfaceVertex selectedVertex;
                for (auto vertex : model->vertices) {
                    QVector3D pos = vertex.position;
                    float dist = pickedPos.distanceToPoint(pos);
                    if (selectedVertex.position.isNull() || dist < min) {
                        min = dist;
                        selectedVertex = vertex;
                    }
                }
                model->selectedVertex = selectedVertex;
                if (!selectedVertex.position.isNull()
                    && model->picked) {
                    model->series->setSelectedPoint(selectedVertex.coord);
                    m_surfaceController->setSlicingActive(false);
                    if (isSliceEnabled())
                        setSliceActivatedChanged(true);
                }
            }
        }
    }
    return true;
}

void QQuickGraphsSurface::updateSelectedPoint()
{
    bool labelVisible = false;
    m_instancing->resetPositions();
    if (sliceView() && sliceView()->isVisible())
        m_sliceInstancing->resetPositions();
    for (auto model : m_model) {
        if (model->selectedVertex.position.isNull())
            continue;
        QPoint selectedCoord = model->selectedVertex.coord;
        int index = selectedCoord.x() * model->columnCount + selectedCoord.y();
        SurfaceVertex selectedVertex = model->vertices.at(index);
        if (model->series->isVisible() &&
                !selectedVertex.position.isNull()) {
            m_instancing->addPosition(selectedVertex.position);
            QVector3D slicePosition = selectedVertex.position;
            if (sliceView() && sliceView()->isVisible()) {
                if (m_surfaceController->selectionMode().testFlag(QAbstract3DGraph::SelectionColumn))
                    slicePosition.setX(-slicePosition.z());
                slicePosition.setZ(.0f);
                m_sliceInstancing->addPosition(slicePosition);
            }
            if (model->picked) {
                const QSurfaceDataArray &array = *(model->series->dataProxy())->array();
                const QSurfaceDataRow &rowArray = *array.at(selectedVertex.coord.x());
                QVector3D value = rowArray.at(selectedVertex.coord.y()).position();
                QVector3D labelPosition = selectedVertex.position;
                QString x = static_cast<QValue3DAxis *>(m_surfaceController->axisX())->stringForValue(value.x());
                QString y = static_cast<QValue3DAxis *>(m_surfaceController->axisY())->stringForValue(value.y());
                QString z = static_cast<QValue3DAxis *>(m_surfaceController->axisZ())->stringForValue(value.z());
                QString label = x + QStringLiteral(", ") +
                        y + QStringLiteral(", ") +
                        z;

                updateItemLabel(labelPosition);
                itemLabel()->setProperty("labelText", label);
                labelVisible = true;

                if (sliceView() && sliceView()->isVisible()) {
                    labelPosition = slicePosition;
                    labelPosition.setZ(.1f);
                    labelPosition.setY(labelPosition.y() + .05f);
                    sliceItemLabel()->setPosition(labelPosition);
                    sliceItemLabel()->setProperty("labelText", label);
                }
            }
        }
    }
    itemLabel()->setVisible(labelVisible);
    if (sliceView() && sliceView()->isVisible())
        sliceItemLabel()->setVisible(labelVisible);
}

void QQuickGraphsSurface::addModel(QSurface3DSeries *series)
{
    auto parent = graphNode();
    bool visible = series->isVisible();

    auto model = new QQuick3DModel();
    model->setParent(parent);
    model->setParentItem(parent);
    model->setObjectName(QStringLiteral("SurfaceModel"));
    model->setVisible(visible);
    if (m_surfaceController->selectionMode().testFlag(QAbstract3DGraph::SelectionNone))
        model->setPickable(false);
    else
        model->setPickable(true);

    auto geometry = new QQuick3DGeometry();
    geometry->setParent(model);
    geometry->setStride(sizeof(SurfaceVertex));
    geometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    geometry->addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                           0,
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::TexCoord0Semantic,
                           sizeof(QVector3D) * 2,
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::NormalSemantic,
                           sizeof(QVector3D),
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::IndexSemantic,
                           0,
                           QQuick3DGeometry::Attribute::U32Type);
    model->setGeometry(geometry);

    QQuick3DTexture *texture = new QQuick3DTexture();
    texture->setParent(this);
    texture->setRotationUV(-90.0f);
    texture->setHorizontalTiling(QQuick3DTexture::ClampToEdge);
    texture->setVerticalTiling(QQuick3DTexture::ClampToEdge);
    QuickGraphsTextureData *textureData = new QuickGraphsTextureData();
    textureData->setParent(texture);
    textureData->setParentItem(texture);
    texture->setTextureData(textureData);

    QQmlListReference materialRef(model, "materials");
    QQuick3DCustomMaterial *customMaterial = nullptr;
    if (series->colorStyle() == Q3DTheme::ColorStyleObjectGradient)
        customMaterial = createQmlCustomMaterial(QStringLiteral(":/materials/ObjectGradientSurfaceMaterial"));
    else
        customMaterial = createQmlCustomMaterial(QStringLiteral(":/materials/RangeGradientMaterial"));
    customMaterial->setParent(model);
    customMaterial->setParentItem(model);
    customMaterial->setCullMode(QQuick3DMaterial::NoCulling);
    QVariant textureInputAsVariant = customMaterial->property("custex");
    QQuick3DShaderUtilsTextureInput *textureInput = textureInputAsVariant.value<QQuick3DShaderUtilsTextureInput *>();
    textureInput->setTexture(texture);

    materialRef.append(customMaterial);

    auto gridModel = new QQuick3DModel();
    gridModel->setParent(parent);
    gridModel->setParentItem(parent);
    gridModel->setObjectName(QStringLiteral("SurfaceModel"));
    gridModel->setVisible(visible);
    gridModel->setDepthBias(1.0f);
    auto gridGeometry = new QQuick3DGeometry();
    gridGeometry->setParent(this);
    gridGeometry->setStride(sizeof(SurfaceVertex));
    gridGeometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Lines);
    gridGeometry->addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                               0,
                               QQuick3DGeometry::Attribute::F32Type);
    gridGeometry->addAttribute(QQuick3DGeometry::Attribute::IndexSemantic,
                               0,
                               QQuick3DGeometry::Attribute::U32Type);
    gridModel->setGeometry(gridGeometry);
    QQmlListReference gridMaterialRef(gridModel, "materials");
    auto gridMaterial = new QQuick3DPrincipledMaterial();
    gridMaterial->setParent(gridModel);
    gridMaterial->setLighting(QQuick3DPrincipledMaterial::NoLighting);
    gridMaterialRef.append(gridMaterial);

    SurfaceModel *surfaceModel = new SurfaceModel();
    surfaceModel->model = model;
    surfaceModel->gridModel = gridModel;
    surfaceModel->series = series;
    surfaceModel->texture = texture;
    surfaceModel->customMaterial = customMaterial;

    m_model.push_back(surfaceModel);

    connect(series,
            &QSurface3DSeries::flatShadingEnabledChanged,
            this,
            &QQuickGraphsSurface::handleFlatShadingEnabledChanged);
    connect(series,
            &QSurface3DSeries::wireframeColorChanged,
            this,
            &QQuickGraphsSurface::handleWireframeColorChanged);

    if (sliceView())
        addSliceModel(surfaceModel);
}

void QQuickGraphsSurface::createSliceView()
{
    QQuickGraphsItem::createSliceView();

    for (auto surfaceModel : m_model)
        addSliceModel(surfaceModel);

    QQuick3DViewport *sliceParent = sliceView();

    m_sliceSelectionPointer = new QQuick3DModel();
    m_sliceSelectionPointer->setParent(sliceParent->scene());
    m_sliceSelectionPointer->setParentItem(sliceParent->scene());
    m_sliceSelectionPointer->setSource(QUrl(QStringLiteral("#Sphere")));
    QQuick3DPrincipledMaterial *pointerMaterial = new QQuick3DPrincipledMaterial();
    pointerMaterial->setParent(m_sliceSelectionPointer);
    pointerMaterial->setBaseColor(m_surfaceController->activeTheme()->singleHighlightColor());
    QQmlListReference sliceMaterialRef(m_sliceSelectionPointer, "materials");
    sliceMaterialRef.append(pointerMaterial);
    m_sliceInstancing = new SurfaceSelectionInstancing();
    m_sliceInstancing->setScale(QVector3D(0.001f, 0.001f, 0.001f));
    m_sliceSelectionPointer->setInstancing(m_sliceInstancing);
    m_sliceInstancing->setColor(m_surfaceController->activeTheme()->singleHighlightColor());
}

void QQuickGraphsSurface::addSliceModel(SurfaceModel *model)
{
    QQuick3DViewport *sliceParent = sliceView();

    auto surfaceModel = new QQuick3DModel();
    surfaceModel->setParent(sliceParent->scene());
    surfaceModel->setParentItem(sliceParent->scene());
    surfaceModel->setVisible(model->series->isVisible());
    surfaceModel->setDepthBias(1.f);

    auto geometry = new QQuick3DGeometry();
    geometry->setParent(surfaceModel);
    geometry->setParentItem(surfaceModel);
    geometry->setStride(sizeof(SurfaceVertex));
    geometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    geometry->addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                           0,
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::TexCoord0Semantic,
                           sizeof(QVector3D) * 2,
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::NormalSemantic,
                           sizeof(QVector3D),
                           QQuick3DGeometry::Attribute::F32Type);
    geometry->addAttribute(QQuick3DGeometry::Attribute::IndexSemantic,
                           0,
                           QQuick3DGeometry::Attribute::U32Type);
    surfaceModel->setGeometry(geometry);

    QQmlListReference materialRef(surfaceModel, "materials");
    auto material = createQmlCustomMaterial(QStringLiteral(":/materials/RangeGradientMaterial"));
    material->setCullMode(QQuick3DMaterial::NoCulling);
    QVariant textureInputAsVariant = material->property("custex");
    QQuick3DShaderUtilsTextureInput *textureInput = textureInputAsVariant.value<QQuick3DShaderUtilsTextureInput *>();
    QQuick3DTexture *texture = model->texture;
    textureInput->setTexture(texture);
    materialRef.append(material);

    model->sliceModel = surfaceModel;

    QQuick3DModel *gridModel = new QQuick3DModel();
    gridModel->setParent(sliceParent->scene());
    gridModel->setParentItem(sliceParent->scene());
    gridModel->setVisible(model->series->isVisible());
    gridModel->setDepthBias(1.0f);
    QQuick3DGeometry *gridGeometry = new QQuick3DGeometry();
    gridGeometry->setParent(gridModel);
    gridGeometry->setStride(sizeof(SurfaceVertex));
    gridGeometry->setPrimitiveType(QQuick3DGeometry::PrimitiveType::Lines);
    gridGeometry->addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                               0,
                               QQuick3DGeometry::Attribute::F32Type);
    gridGeometry->addAttribute(QQuick3DGeometry::Attribute::IndexSemantic,
                               0,
                               QQuick3DGeometry::Attribute::U32Type);
    gridModel->setGeometry(gridGeometry);
    QQmlListReference gridMaterialRef(gridModel, "materials");
    QQuick3DPrincipledMaterial *gridMaterial = new QQuick3DPrincipledMaterial();
    gridMaterial->setParent(gridModel);
    gridMaterial->setLighting(QQuick3DPrincipledMaterial::NoLighting);
    gridMaterial->setParent(gridModel);
    gridMaterialRef.append(gridMaterial);

    model->sliceGridModel = gridModel;
}

void QQuickGraphsSurface::updateSingleHighlightColor()
{
    m_instancing->setColor(m_surfaceController->activeTheme()->singleHighlightColor());
    if (sliceView())
        m_sliceInstancing->setColor(m_surfaceController->activeTheme()->singleHighlightColor());
}

void QQuickGraphsSurface::handleThemeTypeChange()
{
    for (auto model : m_model)
        updateMaterial(model);
}

QT_END_NAMESPACE
