// Copyright (C) 2026 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

//! [1]
ValueAxis {
    titleText: "Temperature °C"
    labelFormat: "%.2f"
    labelPostFormat: "%1 °C"
    // Results in "2.50 °C"
}
//! [1]

//! [2]
LogValueAxis {
    titleText: "Temperature °C"
    labelFormat: "%.2f"
    labelPostFormat: "%1 °C"
    // Results in "2.50 °C"
}
//! [2]
