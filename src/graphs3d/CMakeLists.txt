# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

set(qml_files
    "qml/designer/Bars3DSpecifics.qml"
    "qml/designer/Scatter3DSpecifics.qml"
    "qml/designer/Surface3DSpecifics.qml"
    "qml/designer/GraphsCameraSection.qml"
    "qml/designer/GraphsSection.qml"
    "qml/designer/default/Bars3D.qml"
    "qml/designer/default/Scatter3D.qml"
    "qml/designer/default/Surface3D.qml"
)

set_source_files_properties(${qml_files} PROPERTIES
    QT_QML_SKIP_QMLDIR_ENTRY TRUE
)

qt_internal_add_qml_module(Graphs
    URI "QtGraphs3D"
    TARGET_PRODUCT "Qt Graphs (Qt $$QT_VERSION)"
    TARGET_DESCRIPTION "Qt Graphs 3D Visualization component for Qt."
    VERSION "${PROJECT_VERSION}"
    PAST_MAJOR_VERSIONS 1
    DEPENDENCIES
        QtQuick
        QtQuick3D
        QML_FILES
            ${qml_files}
    SOURCES
        axis/qabstract3daxis.cpp axis/qabstract3daxis.h axis/qabstract3daxis_p.h
        axis/qcategory3daxis.cpp axis/qcategory3daxis.h axis/qcategory3daxis_p.h
        axis/qlogvalue3daxisformatter.cpp axis/qlogvalue3daxisformatter.h axis/qlogvalue3daxisformatter_p.h
        axis/qvalue3daxis.cpp axis/qvalue3daxis.h axis/qvalue3daxis_p.h
        axis/qvalue3daxisformatter.cpp axis/qvalue3daxisformatter.h axis/qvalue3daxisformatter_p.h

        data/abstractitemmodelhandler.cpp data/abstractitemmodelhandler_p.h
        data/baritemmodelhandler.cpp data/baritemmodelhandler_p.h
        data/qabstract3dseries.cpp data/qabstract3dseries.h data/qabstract3dseries_p.h
        data/qabstractdataproxy.cpp data/qabstractdataproxy.h data/qabstractdataproxy_p.h
        data/qbar3dseries.cpp data/qbar3dseries.h data/qbar3dseries_p.h
        data/qbardataitem.cpp data/qbardataitem.h
        data/qbardataproxy.cpp data/qbardataproxy.h data/qbardataproxy_p.h
        data/qcustom3ditem.cpp data/qcustom3ditem.h data/qcustom3ditem_p.h
        data/qcustom3dlabel.cpp data/qcustom3dlabel.h data/qcustom3dlabel_p.h
        data/qcustom3dvolume.cpp data/qcustom3dvolume.h data/qcustom3dvolume_p.h
        data/qheightmapsurfacedataproxy.cpp data/qheightmapsurfacedataproxy.h data/qheightmapsurfacedataproxy_p.h
        data/qitemmodelbardataproxy.cpp data/qitemmodelbardataproxy.h data/qitemmodelbardataproxy_p.h
        data/qitemmodelscatterdataproxy.cpp data/qitemmodelscatterdataproxy.h data/qitemmodelscatterdataproxy_p.h
        data/qitemmodelsurfacedataproxy.cpp data/qitemmodelsurfacedataproxy.h data/qitemmodelsurfacedataproxy_p.h
        data/qscatter3dseries.cpp data/qscatter3dseries.h data/qscatter3dseries_p.h
        data/qscatterdataitem.cpp data/qscatterdataitem.h
        data/qscatterdataproxy.cpp data/qscatterdataproxy.h data/qscatterdataproxy_p.h
        data/qsurface3dseries.cpp data/qsurface3dseries.h data/qsurface3dseries_p.h
        data/qsurfacedataitem.cpp data/qsurfacedataitem.h
        data/qsurfacedataproxy.cpp data/qsurfacedataproxy.h data/qsurfacedataproxy_p.h
        data/scatteritemmodelhandler.cpp data/scatteritemmodelhandler_p.h
        data/surfaceitemmodelhandler.cpp data/surfaceitemmodelhandler_p.h

        engine/axishelper.cpp engine/axishelper_p.h
        engine/barinstancing.cpp engine/barinstancing_p.h
        engine/scatterinstancing.cpp engine/scatterinstancing_p.h
        engine/q3dbars.cpp engine/q3dbars.h
        engine/q3dscatter.cpp engine/q3dscatter.h
        engine/q3dscene.cpp engine/q3dscene.h engine/q3dscene_p.h
        engine/q3dsurface.cpp engine/q3dsurface.h
        engine/qabstract3dgraph.cpp engine/qabstract3dgraph.h
        engine/surfaceselectioninstancing.cpp engine/surfaceselectioninstancing_p.h

        global/graphsglobal_p.h
        global/qgraphsglobal.h

        input/q3dinputhandler.cpp input/q3dinputhandler.h input/q3dinputhandler_p.h
        input/qabstract3dinputhandler.cpp input/qabstract3dinputhandler.h input/qabstract3dinputhandler_p.h
        input/qtouch3dinputhandler.cpp input/qtouch3dinputhandler.h input/qtouch3dinputhandler_p.h

        theme/q3dtheme.cpp theme/q3dtheme.h theme/q3dtheme_p.h
        theme/thememanager.cpp theme/thememanager_p.h

        utils/qutils.h
        utils/utils.cpp utils/utils_p.h

        qml/foreigntypes_p.h
        qml/qquickgraphscolor.cpp qml/qquickgraphscolor_p.h
        qml/qquickgraphsseries.cpp qml/qquickgraphsseries_p.h
        qml/qquickgraphsitem.cpp qml/qquickgraphsitem_p.h
        qml/qquickgraphsbars.cpp qml/qquickgraphsbars_p.h
        qml/qquickgraphssurface.cpp qml/qquickgraphssurface_p.h
        qml/qquickgraphsscatter.cpp qml/qquickgraphsscatter_p.h
        qml/qquickgraphstexturedata.cpp qml/qquickgraphstexturedata_p.h
    NO_UNITY_BUILD_SOURCES
        # Symbol collision with, `qtouch3dinputhandler.cpp`
        # TODO: Resolve `rotationSpeed` symbol redefinition
        input/q3dinputhandler.cpp
        # Symbol collision with, `surfaceitemmodelhandler.cpp`
        # TODO: Resolve `noRoleIndex` symbol redefinition
        data/baritemmodelhandler.cpp
        data/scatteritemmodelhandler.cpp
        # QScatterDataItem::d_ptr' may be used uninitialized [-Werror=maybe-uninitialized]
        # TODO: Redo QScatterDataItem copy constructor, without using operator=()
        #       Similar issue exists in `qtdatavis3d`.
        data/qbardataitem.cpp
        data/qscatterdataitem.cpp
        data/qsurfacedataitem.cpp
    INCLUDE_DIRECTORIES
        axis
        data
        engine
        global
        input
        theme
        utils
        qml
    LIBRARIES
        Qt::Quick3DPrivate
        Qt::Quick3DRuntimeRenderPrivate
    PUBLIC_LIBRARIES
        Qt::Core
        Qt::Gui
        Qt::Quick
        Qt::QuickWidgets
        Qt::Quick3D
    PRIVATE_MODULE_INTERFACE
        Qt::Quick3DPrivate
        Qt::Quick3DRuntimeRenderPrivate
    GENERATE_CPP_EXPORTS
)
set_source_files_properties("qml/resources/DatapointSphere.qml"
    PROPERTIES QT_RESOURCE_ALIAS "DatapointSphere"
)

set_source_files_properties("qml/resources/DatapointCube.qml"
    PROPERTIES QT_RESOURCE_ALIAS "DatapointCube"
)

set_source_files_properties("qml/resources/AxisLabel.qml"
    PROPERTIES QT_RESOURCE_ALIAS "AxisLabel"
)

set_source_files_properties("qml/resources/GridLine.qml"
    PROPERTIES QT_RESOURCE_ALIAS "GridLine"
)

set_source_files_properties("qml/resources/ItemLabel.qml"
    PROPERTIES QT_RESOURCE_ALIAS "ItemLabel"
)

set_source_files_properties("qml/resources/TitleLabel.qml"
    PROPERTIES QT_RESOURCE_ALIAS "TitleLabel"
)
set_source_files_properties("qml/resources/GridSurfaceMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "GridSurfaceMaterial"
)
set_source_files_properties("qml/resources/SurfaceSliceMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "SurfaceSliceMaterial"
)
set_source_files_properties("qml/resources/BarsMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "BarsMaterial"
)
set_source_files_properties("qml/resources/BarsMaterialInstancing.qml"
    PROPERTIES QT_RESOURCE_ALIAS "BarsMaterialInstancing"
)
set_source_files_properties("qml/resources/SurfaceMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "SurfaceMaterial"
)
set_source_files_properties("qml/resources/ScatterMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "ScatterMaterial"
)
set_source_files_properties("qml/resources/ScatterMaterialInstancing.qml"
    PROPERTIES QT_RESOURCE_ALIAS "ScatterMaterialInstancing"
)
set_source_files_properties("qml/resources/ObjectGradientMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "ObjectGradientMaterial"
)
set_source_files_properties("qml/resources/ObjectGradientMaterialInstancing.qml"
    PROPERTIES QT_RESOURCE_ALIAS "ObjectGradientMaterialInstancing"
)

set_source_files_properties("qml/resources/VolumeMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "VolumeMaterial"
)

set_source_files_properties("qml/resources/VolumeLowDefMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "VolumeLowDefMaterial"
)

set_source_files_properties("qml/resources/VolumeSliceMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "VolumeSliceMaterial"
)

set_source_files_properties("qml/resources/VolumeFrameMaterial.qml"
    PROPERTIES QT_RESOURCE_ALIAS "VolumeFrameMaterial"
)

set(qml_component_resources
    "qml/resources/DatapointSphere.qml"
    "qml/resources/DatapointCube.qml"
)

set(qml_axis_resurces
    "qml/resources/AxisLabel.qml"
    "qml/resources/GridLine.qml"
    "qml/resources/ItemLabel.qml"
    "qml/resources/TitleLabel.qml"
)

set(qml_material_resources
    "qml/resources/ObjectGradientMaterial.qml"
    "qml/resources/ObjectGradientMaterialInstancing.qml"
    "qml/resources/BarsMaterial.qml"
    "qml/resources/BarsMaterialInstancing.qml"
    "qml/resources/SurfaceMaterial.qml"
    "qml/resources/ScatterMaterial.qml"
    "qml/resources/ScatterMaterialInstancing.qml"
    "qml/resources/GridSurfaceMaterial.qml"
    "qml/resources/SurfaceSliceMaterial.qml"
    "qml/resources/SurfaceShadowNoTex.qml"
    "qml/resources/VolumeMaterial.qml"
    "qml/resources/VolumeLowDefMaterial.qml"
    "qml/resources/VolumeSliceMaterial.qml"
    "qml/resources/VolumeFrameMaterial.qml"
)

set(resources
    "qml/designer/Graphs.metainfo"
    "qml/designer/images/bars3d-icon.png"
    "qml/designer/images/bars3d-icon16.png"
    "qml/designer/images/scatter3d-icon.png"
    "qml/designer/images/scatter3d-icon16.png"
    "qml/designer/images/surface3d-icon.png"
    "qml/designer/images/surface3d-icon16.png"
)

# .mesh file aliases
set_source_files_properties("engine/meshes/arrowFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "arrowMesh"
)
set_source_files_properties("engine/meshes/arrowSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "arrowMeshSmooth"
)
set_source_files_properties("engine/meshes/background.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "backgroundMesh"
)
set_source_files_properties("engine/meshes/barFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshFull"
)
set_source_files_properties("engine/meshes/barFilledSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshSmoothFull"
)
set_source_files_properties("engine/meshes/barFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "bevelBarMesh"
)
set_source_files_properties("engine/meshes/barSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshSmooth"
)
set_source_files_properties("engine/meshes/cubeFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "barMeshFull"
)
set_source_files_properties("engine/meshes/cubeFilledSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "barMeshSmoothFull"
)
set_source_files_properties("engine/meshes/cubeFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "barMesh"
)
set_source_files_properties("engine/meshes/cubeSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "barMeshSmooth"
)
set_source_files_properties("engine/meshes/coneFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "coneMeshFull"
)
set_source_files_properties("engine/meshes/coneFilledSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "coneMeshSmoothFull"
)
set_source_files_properties("engine/meshes/coneFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "coneMesh"
)
set_source_files_properties("engine/meshes/coneSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "coneMeshSmooth"
)
set_source_files_properties("engine/meshes/cylinderFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshFull"
)
set_source_files_properties("engine/meshes/cylinderFilledSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshSmoothFull"
)
set_source_files_properties("engine/meshes/cylinderFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "cylinderMesh"
)
set_source_files_properties("engine/meshes/cylinderSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshSmooth"
)
set_source_files_properties("engine/meshes/minimalFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "minimalMesh"
)
set_source_files_properties("engine/meshes/minimalSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "minimalMeshSmooth"
)
set_source_files_properties("engine/meshes/plane.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "planeMesh"
)
set_source_files_properties("engine/meshes/pyramidFilledFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshFull"
)
set_source_files_properties("engine/meshes/pyramidFilledSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshSmoothFull"
)
set_source_files_properties("engine/meshes/pyramidFlat.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "pyramidMesh"
)
set_source_files_properties("engine/meshes/pyramidSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshSmooth"
)
set_source_files_properties("engine/meshes/octagon.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "octagonMesh"
)
set_source_files_properties("engine/meshes/sphere.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "sphereMesh"
)
set_source_files_properties("engine/meshes/sphereSmooth.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "sphereMeshSmooth"
)
set_source_files_properties("engine/meshes/backgroundNoFloor.mesh"
    PROPERTIES QT_RESOURCE_ALIAS "backgroundNoFloorMesh"
)
set(mesh_resource_files
    # .mesh files
    "engine/meshes/background.mesh"
    "engine/meshes/arrowFlat.mesh"
    "engine/meshes/arrowSmooth.mesh"
    "engine/meshes/barFilledFlat.mesh"
    "engine/meshes/barFilledSmooth.mesh"
    "engine/meshes/barFlat.mesh"
    "engine/meshes/barSmooth.mesh"
    "engine/meshes/coneFilledFlat.mesh"
    "engine/meshes/coneFilledSmooth.mesh"
    "engine/meshes/coneFlat.mesh"
    "engine/meshes/coneSmooth.mesh"
    "engine/meshes/cubeFilledFlat.mesh"
    "engine/meshes/cubeFilledSmooth.mesh"
    "engine/meshes/cubeFlat.mesh"
    "engine/meshes/cubeSmooth.mesh"
    "engine/meshes/cylinderFilledFlat.mesh"
    "engine/meshes/cylinderFilledSmooth.mesh"
    "engine/meshes/cylinderFlat.mesh"
    "engine/meshes/cylinderSmooth.mesh"
    "engine/meshes/minimalFlat.mesh"
    "engine/meshes/minimalSmooth.mesh"
    "engine/meshes/plane.mesh"
    "engine/meshes/pyramidFilledFlat.mesh"
    "engine/meshes/pyramidFilledSmooth.mesh"
    "engine/meshes/pyramidFlat.mesh"
    "engine/meshes/pyramidSmooth.mesh"
    "engine/meshes/octagon.mesh"
    "engine/meshes/sphere.mesh"
    "engine/meshes/sphereSmooth.mesh"
    "engine/meshes/backgroundNoFloor.mesh"
)

set_source_files_properties("engine/shaders/3dsliceframes.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragment3DSliceFrames"
)
set_source_files_properties("engine/shaders/position.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexPosition"
)
set_source_files_properties("engine/shaders/bars.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentBars"
)
set_source_files_properties("engine/shaders/barsinstancing.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentBarsInstancing"
)
set_source_files_properties("engine/shaders/barsinstancing.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexBarsInstancing"
)
set_source_files_properties("engine/shaders/surface.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentSurface"
)
set_source_files_properties("engine/shaders/surface.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexSurface"
)
set_source_files_properties("engine/shaders/scatter.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexScatter"
)
set_source_files_properties("engine/shaders/scatter.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentScatter"
)
set_source_files_properties("engine/shaders/scatterinstancing.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexScatterInstancing"
)
set_source_files_properties("engine/shaders/scatterinstancing.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentScatterInstancing"
)
set_source_files_properties("engine/shaders/surfaceGrid.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragSurfaceGrid"
)
set_source_files_properties("engine/shaders/surfaceGrid.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexSurfaceGrid"
)
set_source_files_properties("engine/shaders/surfaceSlice.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexSurfaceSlice"
)
set_source_files_properties("engine/shaders/texture3d.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3D"
)
set_source_files_properties("engine/shaders/texture3d.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexTexture3D"
)
set_source_files_properties("engine/shaders/texture3dlowdef.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3DLowDef"
)
set_source_files_properties("engine/shaders/texture3dslice.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3DSlice"
)
set_source_files_properties("engine/shaders/objectgradient.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexobjectgradient"
)
set_source_files_properties("engine/shaders/objectgradient.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentobjectgradient"
)
set_source_files_properties("engine/shaders/objectgradientinstancing.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexobjectgradientinstancing"
)
set_source_files_properties("engine/shaders/objectgradientinstancing.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentobjectgradientinstancing"
)
set_source_files_properties("engine/shaders/rangegradient.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexrangegradient"
)
set_source_files_properties("engine/shaders/rangegradient.frag"
    PROPERTIES QT_RESOURCE_ALIAS "fragmentrangegradient"
)
set_source_files_properties("engine/shaders/rangegradientinstancing.vert"
    PROPERTIES QT_RESOURCE_ALIAS "vertexrangegradientinstancing"
)
set(shader_resource_files
    "engine/shaders/3dsliceframes.frag"
    "engine/shaders/position.vert"
    "engine/shaders/objectgradient.vert"
    "engine/shaders/objectgradient.frag"
    "engine/shaders/objectgradientinstancing.vert"
    "engine/shaders/objectgradientinstancing.frag"
    "engine/shaders/rangegradient.vert"
    "engine/shaders/rangegradient.frag"
    "engine/shaders/rangegradientinstancing.frag"
    "engine/shaders/rangegradientinstancing.vert"
    "engine/shaders/surface.vert"
    "engine/shaders/surface.frag"
    "engine/shaders/surfaceGrid.vert"
    "engine/shaders/surfaceGrid.frag"
    "engine/shaders/bars.frag"
    "engine/shaders/barsinstancing.vert"
    "engine/shaders/barsinstancing.frag"
    "engine/shaders/scatter.vert"
    "engine/shaders/scatter.frag"
    "engine/shaders/scatterinstancing.vert"
    "engine/shaders/scatterinstancing.frag"
    "engine/shaders/surfaceSlice.vert"
    "engine/shaders/texture3d.frag"
    "engine/shaders/texture3d.vert"
    "engine/shaders/texture3dlowdef.frag"
    "engine/shaders/texture3dslice.frag"
)

qt_internal_add_resource(Graphs "graphsshaders"
    PREFIX
        "/shaders"
    BASE
        "engine/shaders"
    FILES
        ${shader_resource_files}
)

qt_internal_add_resource(Graphs "graphsmeshes"
    PREFIX
        "/defaultMeshes"
    BASE
        "engine/meshes"
    FILES
        ${mesh_resource_files}
        ${obj_resource_files}
)

qt_internal_add_resource(Graphs "qml_component_resources"
    PREFIX
        "/datapointModels"
    BASE
        "qml"
    FILES
        ${qml_component_resources}
)

qt_internal_add_resource(Graphs "qml_axis_resurces"
    PREFIX
        "/axis"
    BASE
        "qml"
    FILES
        ${qml_axis_resurces}
)

qt_internal_add_resource(Graphs "qml_material_resources"
    PREFIX
        "/materials"
    BASE
        "qml"
    FILES
        ${qml_material_resources}
)

qt_internal_extend_target(Graphs CONDITION MACOS
    LIBRARIES
        Qt::GuiPrivate
    PUBLIC_LIBRARIES
        Qt::Gui
    PRIVATE_MODULE_INTERFACE
        Qt::GuiPrivate
)
qt_internal_add_docs(Graphs
    doc/qtgraphs.qdocconf
)

