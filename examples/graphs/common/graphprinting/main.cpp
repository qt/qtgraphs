// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtGui/qguiapplication.h>
#include <QtQml/qqmlcontext.h>
#include <QtQml/qqmlengine.h>
#include <QtQuick/qquickview.h>
#include "graphprinter.h"

using namespace Qt::StringLiterals;

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQuickView viewer;

  viewer.setTitle(QStringLiteral("Graph Printing"));

  //! [0]
  GraphPrinter graphPrinter;
  viewer.rootContext()->setContextProperty("graphPrinter", &graphPrinter);
  //! [0]

  viewer.setMinimumSize({1280, 720});
  viewer.loadFromModule(u"GraphPrintingExample"_s, u"Main"_s);
  viewer.setResizeMode(QQuickView::SizeRootObjectToView);
  viewer.setColor("white");
  viewer.show();

  return app.exec();
}
