# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

qt_internal_extend_target(Graphs
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
)

qt_internal_extend_target(Graphs CONDITION MACOS
    LIBRARIES
        Qt::GuiPrivate
    PUBLIC_LIBRARIES
        Qt::Gui
    PRIVATE_MODULE_INTERFACE
        Qt::GuiPrivate
)

