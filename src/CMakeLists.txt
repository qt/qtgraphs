# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/graphs2d/configure.cmake")
qt_feature_evaluate_features("${CMAKE_CURRENT_SOURCE_DIR}/graphs3d/configure.cmake")

if(NOT QT_FEATURE_graphs_2d AND NOT QT_FEATURE_graphs_3d)
    message("You must have either FEATURE_graphs_2d or FEATURE_graphs_3d ON, or nothing will be built.")
else()
    # First check if 3D is enabled. If it is, we must add dependencies to QtQuick3D.
    # TODO: We could add more branches based on the feature flags for disabling 2D -specific
    # dependencies (QtQuickPrivate, Qt::QuickShapesPrivate)
    if(QT_FEATURE_graphs_3d)
        set(qml_files
            graphs3d/qml/designer/Bars3DSpecifics.qml
            graphs3d/qml/designer/Scatter3DSpecifics.qml
            graphs3d/qml/designer/Surface3DSpecifics.qml
            graphs3d/qml/designer/GraphsCameraSection.qml
            graphs3d/qml/designer/GraphsSection.qml
            graphs3d/qml/designer/default/Bars3D.qml
            graphs3d/qml/designer/default/Scatter3D.qml
            graphs3d/qml/designer/default/Surface3D.qml
        )

        set_source_files_properties(${qml_files} PROPERTIES
            QT_QML_SKIP_QMLDIR_ENTRY TRUE
        )

        qt_internal_add_qml_module(Graphs
            URI "QtGraphs"
            TARGET_PRODUCT "Qt Graphs (Qt $$QT_VERSION)"
            TARGET_DESCRIPTION "Qt Graphs Visualization component for Qt."
            VERSION "${PROJECT_VERSION}"
            PAST_MAJOR_VERSIONS 1
            DEPENDENCIES
                QtQuick
                # 2D specific
                QtQuickPrivate
                # 3D specific
                QtQuick3D
            QML_FILES
                ${qml_files}
            LIBRARIES
                # 2D specific
                Qt::QuickPrivate
                Qt::QuickShapesPrivate
                # 3D specific
                Qt::Quick3DPrivate
                Qt::Quick3DRuntimeRenderPrivate
            PUBLIC_LIBRARIES
                Qt::Core
                Qt::Gui
                Qt::Quick
                Qt::QuickWidgets
                Qt::Quick3D
            PRIVATE_MODULE_INTERFACE
                # 3D specific
                Qt::Quick3DPrivate
                Qt::Quick3DRuntimeRenderPrivate
            GENERATE_CPP_EXPORTS
        )

        # CMake 3.16 forces these to be added here instead of the graphs3d folder
        # TODO: Create a branch that has 3D -only, to avoid including these
        set(shader_resource_files_2D
            "graphs2d/data/gridshader.frag.qsb"
            "graphs2d/data/gridshader.vert.qsb"
            "graphs2d/data/tickershader.frag.qsb"
            "graphs2d/data/tickershader.vert.qsb"
            "graphs2d/data/tickershaderhorizontal.frag.qsb"
            "graphs2d/data/tickershaderhorizontal.vert.qsb"
            "graphs2d/data/lineshaderhorizontal.frag.qsb"
            "graphs2d/data/lineshaderhorizontal.vert.qsb"
            "graphs2d/data/lineshadervertical.frag.qsb"
            "graphs2d/data/lineshadervertical.vert.qsb"
        )

        # TODO: Create a branch that has 3D -only, to avoid including these
        qt_internal_add_resource(Graphs "graphsshaders2D"
            PREFIX
                "/shaders"
            BASE
                "graphs2d/data"
            FILES
                ${shader_resource_files_2D}
        )

        set_source_files_properties("graphs3d/qml/resources/DatapointSphere.qml"
            PROPERTIES QT_RESOURCE_ALIAS "DatapointSphere"
        )

        set_source_files_properties("graphs3d/qml/resources/DatapointCube.qml"
            PROPERTIES QT_RESOURCE_ALIAS "DatapointCube"
        )

        set_source_files_properties("graphs3d/qml/resources/AxisLabel.qml"
            PROPERTIES QT_RESOURCE_ALIAS "AxisLabel"
        )

        set_source_files_properties("graphs3d/qml/resources/GridLine.qml"
            PROPERTIES QT_RESOURCE_ALIAS "GridLine"
        )

        set_source_files_properties("graphs3d/qml/resources/ItemLabel.qml"
            PROPERTIES QT_RESOURCE_ALIAS "ItemLabel"
        )

        set_source_files_properties("graphs3d/qml/resources/TitleLabel.qml"
            PROPERTIES QT_RESOURCE_ALIAS "TitleLabel"
        )
        set_source_files_properties("graphs3d/qml/resources/GridSurfaceMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "GridSurfaceMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/SurfaceSliceMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "SurfaceSliceMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/BarsMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "BarsMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/BarsMaterialInstancing.qml"
            PROPERTIES QT_RESOURCE_ALIAS "BarsMaterialInstancing"
        )
        set_source_files_properties("graphs3d/qml/resources/SurfaceMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "SurfaceMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/ScatterMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "ScatterMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/ScatterMaterialInstancing.qml"
            PROPERTIES QT_RESOURCE_ALIAS "ScatterMaterialInstancing"
        )
        set_source_files_properties("graphs3d/qml/resources/ObjectGradientMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "ObjectGradientMaterial"
        )
        set_source_files_properties("graphs3d/qml/resources/ObjectGradientMaterialInstancing.qml"
            PROPERTIES QT_RESOURCE_ALIAS "ObjectGradientMaterialInstancing"
        )

        set_source_files_properties("graphs3d/qml/resources/VolumeMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "VolumeMaterial"
        )

        set_source_files_properties("graphs3d/qml/resources/VolumeLowDefMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "VolumeLowDefMaterial"
        )

        set_source_files_properties("graphs3d/qml/resources/VolumeSliceMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "VolumeSliceMaterial"
        )

        set_source_files_properties("graphs3d/qml/resources/VolumeFrameMaterial.qml"
            PROPERTIES QT_RESOURCE_ALIAS "VolumeFrameMaterial"
        )

        set(qml_component_resources
            "graphs3d/qml/resources/DatapointSphere.qml"
            "graphs3d/qml/resources/DatapointCube.qml"
        )

        set(qml_axis_resurces
            "graphs3d/qml/resources/AxisLabel.qml"
            "graphs3d/qml/resources/GridLine.qml"
            "graphs3d/qml/resources/ItemLabel.qml"
            "graphs3d/qml/resources/TitleLabel.qml"
        )

        set(qml_material_resources
            "graphs3d/qml/resources/ObjectGradientMaterial.qml"
            "graphs3d/qml/resources/ObjectGradientMaterialInstancing.qml"
            "graphs3d/qml/resources/BarsMaterial.qml"
            "graphs3d/qml/resources/BarsMaterialInstancing.qml"
            "graphs3d/qml/resources/SurfaceMaterial.qml"
            "graphs3d/qml/resources/ScatterMaterial.qml"
            "graphs3d/qml/resources/ScatterMaterialInstancing.qml"
            "graphs3d/qml/resources/GridSurfaceMaterial.qml"
            "graphs3d/qml/resources/SurfaceSliceMaterial.qml"
            "graphs3d/qml/resources/SurfaceShadowNoTex.qml"
            "graphs3d/qml/resources/VolumeMaterial.qml"
            "graphs3d/qml/resources/VolumeLowDefMaterial.qml"
            "graphs3d/qml/resources/VolumeSliceMaterial.qml"
            "graphs3d/qml/resources/VolumeFrameMaterial.qml"
        )

        set(resources
            "graphs2d/qml/designer/Graphs.metainfo"
            "graphs2d/qml/designer/images/bars3d-icon.png"
            "graphs2d/qml/designer/images/bars3d-icon16.png"
            "graphs2d/qml/designer/images/scatter3d-icon.png"
            "graphs2d/qml/designer/images/scatter3d-icon16.png"
            "graphs2d/qml/designer/images/surface3d-icon.png"
            "graphs2d/qml/designer/images/surface3d-icon16.png"
        )

        # .mesh file aliases
        set_source_files_properties("graphs3d/engine/meshes/arrowFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "arrowMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/arrowSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "arrowMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/background.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "backgroundMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/barFilledFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/barFilledSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshSmoothFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/barFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "bevelBarMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/barSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "bevelBarMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/cubeFilledFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "barMeshFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/cubeFilledSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "barMeshSmoothFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/cubeFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "barMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/cubeSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "barMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/coneFilledFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "coneMeshFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/coneFilledSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "coneMeshSmoothFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/coneFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "coneMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/coneSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "coneMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/cylinderFilledFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/cylinderFilledSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshSmoothFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/cylinderFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "cylinderMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/cylinderSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "cylinderMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/minimalFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "minimalMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/minimalSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "minimalMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/plane.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "planeMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/pyramidFilledFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/pyramidFilledSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshSmoothFull"
        )
        set_source_files_properties("graphs3d/engine/meshes/pyramidFlat.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "pyramidMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/pyramidSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "pyramidMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/octagon.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "octagonMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/sphere.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "sphereMesh"
        )
        set_source_files_properties("graphs3d/engine/meshes/sphereSmooth.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "sphereMeshSmooth"
        )
        set_source_files_properties("graphs3d/engine/meshes/backgroundNoFloor.mesh"
            PROPERTIES QT_RESOURCE_ALIAS "backgroundNoFloorMesh"
        )
        set(mesh_resource_files
            # .mesh files
            "graphs3d/engine/meshes/background.mesh"
            "graphs3d/engine/meshes/arrowFlat.mesh"
            "graphs3d/engine/meshes/arrowSmooth.mesh"
            "graphs3d/engine/meshes/barFilledFlat.mesh"
            "graphs3d/engine/meshes/barFilledSmooth.mesh"
            "graphs3d/engine/meshes/barFlat.mesh"
            "graphs3d/engine/meshes/barSmooth.mesh"
            "graphs3d/engine/meshes/coneFilledFlat.mesh"
            "graphs3d/engine/meshes/coneFilledSmooth.mesh"
            "graphs3d/engine/meshes/coneFlat.mesh"
            "graphs3d/engine/meshes/coneSmooth.mesh"
            "graphs3d/engine/meshes/cubeFilledFlat.mesh"
            "graphs3d/engine/meshes/cubeFilledSmooth.mesh"
            "graphs3d/engine/meshes/cubeFlat.mesh"
            "graphs3d/engine/meshes/cubeSmooth.mesh"
            "graphs3d/engine/meshes/cylinderFilledFlat.mesh"
            "graphs3d/engine/meshes/cylinderFilledSmooth.mesh"
            "graphs3d/engine/meshes/cylinderFlat.mesh"
            "graphs3d/engine/meshes/cylinderSmooth.mesh"
            "graphs3d/engine/meshes/minimalFlat.mesh"
            "graphs3d/engine/meshes/minimalSmooth.mesh"
            "graphs3d/engine/meshes/plane.mesh"
            "graphs3d/engine/meshes/pyramidFilledFlat.mesh"
            "graphs3d/engine/meshes/pyramidFilledSmooth.mesh"
            "graphs3d/engine/meshes/pyramidFlat.mesh"
            "graphs3d/engine/meshes/pyramidSmooth.mesh"
            "graphs3d/engine/meshes/octagon.mesh"
            "graphs3d/engine/meshes/sphere.mesh"
            "graphs3d/engine/meshes/sphereSmooth.mesh"
            "graphs3d/engine/meshes/backgroundNoFloor.mesh"
        )

        set_source_files_properties("graphs3d/engine/shaders/3dsliceframes.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragment3DSliceFrames"
        )
        set_source_files_properties("graphs3d/engine/shaders/position.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexPosition"
        )
        set_source_files_properties("graphs3d/engine/shaders/bars.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentBars"
        )
        set_source_files_properties("graphs3d/engine/shaders/barsinstancing.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentBarsInstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/barsinstancing.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexBarsInstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/surface.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentSurface"
        )
        set_source_files_properties("graphs3d/engine/shaders/surface.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexSurface"
        )
        set_source_files_properties("graphs3d/engine/shaders/scatter.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexScatter"
        )
        set_source_files_properties("graphs3d/engine/shaders/scatter.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentScatter"
        )
        set_source_files_properties("graphs3d/engine/shaders/scatterinstancing.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexScatterInstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/scatterinstancing.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentScatterInstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/surfaceGrid.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragSurfaceGrid"
        )
        set_source_files_properties("graphs3d/engine/shaders/surfaceGrid.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexSurfaceGrid"
        )
        set_source_files_properties("graphs3d/engine/shaders/surfaceSlice.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexSurfaceSlice"
        )
        set_source_files_properties("graphs3d/engine/shaders/texture3d.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3D"
        )
        set_source_files_properties("graphs3d/engine/shaders/texture3d.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexTexture3D"
        )
        set_source_files_properties("graphs3d/engine/shaders/texture3dlowdef.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3DLowDef"
        )
        set_source_files_properties("graphs3d/engine/shaders/texture3dslice.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentTexture3DSlice"
        )
        set_source_files_properties("graphs3d/engine/shaders/objectgradient.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexobjectgradient"
        )
        set_source_files_properties("graphs3d/engine/shaders/objectgradient.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentobjectgradient"
        )
        set_source_files_properties("graphs3d/engine/shaders/objectgradientinstancing.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexobjectgradientinstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/objectgradientinstancing.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentobjectgradientinstancing"
        )
        set_source_files_properties("graphs3d/engine/shaders/rangegradient.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexrangegradient"
        )
        set_source_files_properties("graphs3d/engine/shaders/rangegradient.frag"
            PROPERTIES QT_RESOURCE_ALIAS "fragmentrangegradient"
        )
        set_source_files_properties("graphs3d/engine/shaders/rangegradientinstancing.vert"
            PROPERTIES QT_RESOURCE_ALIAS "vertexrangegradientinstancing"
        )
        set(shader_resource_files
            "graphs3d/engine/shaders/3dsliceframes.frag"
            "graphs3d/engine/shaders/position.vert"
            "graphs3d/engine/shaders/objectgradient.vert"
            "graphs3d/engine/shaders/objectgradient.frag"
            "graphs3d/engine/shaders/objectgradientinstancing.vert"
            "graphs3d/engine/shaders/objectgradientinstancing.frag"
            "graphs3d/engine/shaders/rangegradient.vert"
            "graphs3d/engine/shaders/rangegradient.frag"
            "graphs3d/engine/shaders/rangegradientinstancing.frag"
            "graphs3d/engine/shaders/rangegradientinstancing.vert"
            "graphs3d/engine/shaders/surface.vert"
            "graphs3d/engine/shaders/surface.frag"
            "graphs3d/engine/shaders/surfaceGrid.vert"
            "graphs3d/engine/shaders/surfaceGrid.frag"
            "graphs3d/engine/shaders/bars.frag"
            "graphs3d/engine/shaders/barsinstancing.vert"
            "graphs3d/engine/shaders/barsinstancing.frag"
            "graphs3d/engine/shaders/scatter.vert"
            "graphs3d/engine/shaders/scatter.frag"
            "graphs3d/engine/shaders/scatterinstancing.vert"
            "graphs3d/engine/shaders/scatterinstancing.frag"
            "graphs3d/engine/shaders/surfaceSlice.vert"
            "graphs3d/engine/shaders/texture3d.frag"
            "graphs3d/engine/shaders/texture3d.vert"
            "graphs3d/engine/shaders/texture3dlowdef.frag"
            "graphs3d/engine/shaders/texture3dslice.frag"
        )

        qt_internal_add_resource(Graphs "graphsshaders"
            PREFIX
                "/shaders"
            BASE
                "graphs3d/engine/shaders"
            FILES
                ${shader_resource_files}
        )

        qt_internal_add_resource(Graphs "graphsmeshes"
            PREFIX
                "/defaultMeshes"
            BASE
                "graphs3d/engine/meshes"
            FILES
                ${mesh_resource_files}
                ${obj_resource_files}
        )

        qt_internal_add_resource(Graphs "qml_component_resources"
            PREFIX
                "/datapointModels"
            BASE
                "graphs3d/qml"
            FILES
                ${qml_component_resources}
        )

        qt_internal_add_resource(Graphs "qml_axis_resurces"
            PREFIX
                "/axis"
            BASE
                "graphs3d/qml"
            FILES
                ${qml_axis_resurces}
        )

        qt_internal_add_resource(Graphs "qml_material_resources"
            PREFIX
                "/materials"
            BASE
                "graphs3d/qml"
            FILES
                ${qml_material_resources}
        )
    else(QT_FEATURE_graphs_2d)
        qt_internal_add_qml_module(Graphs
            URI "QtGraphs"
            TARGET_PRODUCT "Qt Graphs (Qt $$QT_VERSION)"
            TARGET_DESCRIPTION "Qt Graphs Visualization component for Qt."
            VERSION "${PROJECT_VERSION}"
            PAST_MAJOR_VERSIONS 1
            DEPENDENCIES
                QtQuick
                QtQuickPrivate
            LIBRARIES
                Qt::QuickPrivate
                Qt::QuickShapesPrivate
            PUBLIC_LIBRARIES
                Qt::Core
                Qt::Gui
                Qt::Quick
            GENERATE_CPP_EXPORTS
        )

        # CMake 3.16 forces these to be added here instead of the graphs2d folder
        set(shader_resource_files_2D
            "graphs2d/data/gridshader.frag.qsb"
            "graphs2d/data/gridshader.vert.qsb"
            "graphs2d/data/tickershader.frag.qsb"
            "graphs2d/data/tickershader.vert.qsb"
            "graphs2d/data/tickershaderhorizontal.frag.qsb"
            "graphs2d/data/tickershaderhorizontal.vert.qsb"
            "graphs2d/data/lineshaderhorizontal.frag.qsb"
            "graphs2d/data/lineshaderhorizontal.vert.qsb"
            "graphs2d/data/lineshadervertical.frag.qsb"
            "graphs2d/data/lineshadervertical.vert.qsb"
        )

        qt_internal_add_resource(Graphs "graphsshaders2D"
            PREFIX
                "/shaders"
            BASE
                "graphs2d/data"
            FILES
                ${shader_resource_files_2D}
        )
    endif()
endif()

qt_configure_add_summary_section(NAME "Qt Graphs Support")
qt_configure_add_summary_entry(ARGS "graphs-2d")
qt_configure_add_summary_entry(ARGS "graphs-3d")
qt_configure_end_summary_section() # end of "Qt Graphs Support" section

if(QT_FEATURE_graphs_3d)
    add_subdirectory(graphs3d)
endif()

if(QT_FEATURE_graphs_2d)
    add_subdirectory(graphs2d)
endif()

qt_internal_add_docs(Graphs
    doc/qtgraphs.qdocconf
)
