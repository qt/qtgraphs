
// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
import QtQuick
import QtGraphs
import QtTest

Item {
    width: 400
    height: 300

    PieSeries {
        id: pieSeries
        name: "pie"

        SignalSpy {
            id: addedSpy
            target: pieSeries
            signalName: "added"
        }
        SignalSpy {
            id: removedSpy
            target: pieSeries
            signalName: "removed"
        }
        SignalSpy {
            id: sumChangedSpy
            target: pieSeries
            signalName: "sumChanged"
        }
        SignalSpy {
            id: countChangedSpy
            target: pieSeries
            signalName: "countChanged"
        }
        SignalSpy {
            id: spanChangedSpy
            target: pieSeries
            signalName: "angleSpanVisibleLimitChanged"
        }
        SignalSpy {
            id: modeChangedSpy
            target: pieSeries
            signalName: "angleSpanLabelVisibilityChanged"
        }
        SignalSpy {
            id: sliceDataChangedSpy
            target: pieSeries
            signalName: "sliceDataChanged"
        }
        SignalSpy {
            id: sliceLabelsChangedSpy
            target: pieSeries
            signalName: "sliceLabelsChanged"
        }
    }

    PieSeries {
        id: initializedDeclarative

        sliceData: [3.4,5,8.7,9,0,3]
        sliceLabels: ["A","B", "C", "D"]
    }

    PieSeries {
        id: initializedImperative

        PieSlice { id: sliceImperative1; value: 8 }
        PieSlice { id: sliceImperative2; value: 7 }
        PieSlice { id: sliceImperative3; value: 2 }
    }

    PieSlice {
        id: slice1
        value: 4
    }

    PieSlice {
        id: slice2
        value: 2
    }

    PieSlice {
        id: slice3
        value: 8
    }

    TestCase {
        name: "tst_qml-qtquicktest PieSeries"

        function test_01_properties() {
            compare(pieSeries.endAngle, 360)
            compare(pieSeries.holeSize, 0)
            compare(pieSeries.horizontalPosition, 0.5)
            compare(pieSeries.pieSize, 0.7)
            compare(pieSeries.startAngle, 0)
            compare(pieSeries.sum, 0)
            compare(pieSeries.verticalPosition, 0.5)
            compare(pieSeries.angleSpanVisibleLimit, 0)
            compare(pieSeries.angleSpanLabelVisibility, PieSeries.LabelVisibility.First)

            pieSeries.clear()
        }

        function test_02_sliceproperties() {
            var slice = pieSeries.append("slice", 10)
            compare(slice.angleSpan, 360.0)
            verify(slice.borderColor !== undefined)
            compare(slice.borderWidth, 0)
            verify(slice.color !== undefined)
            compare(slice.explodeDistanceFactor, 0.15)
            compare(slice.exploded, false)
            compare(slice.label, "slice")
            compare(slice.labelArmLengthFactor, 0.15)
            verify(slice.labelColor !== undefined)
            compare(slice.labelFont.bold, false)
            compare(slice.labelPosition, PieSlice.LabelPosition.Outside)
            compare(slice.labelVisible, false)
            compare(slice.percentage, 1.0)
            compare(slice.startAngle, 0.0)
            compare(slice.value, 10.0)

            pieSeries.clear()
        }

        function test_03_take() {
            var count = 20
            for (var i = 0; i < count; i++)
                pieSeries.append("slice" + i, Math.random() + 0.01)

            verify(pieSeries.take(pieSeries.find("slice" + 5)))
            verify(pieSeries.take(pieSeries.find("slice" + 6)))
            compare(pieSeries.count, 18)

            pieSeries.clear()
        }

        function test_04_append() {
            addedSpy.clear()
            countChangedSpy.clear()
            sumChangedSpy.clear()
            var count = 50
            for (var i = 0; i < count; i++)
                pieSeries.append("slice" + i,
                                 Math.random() + 0.01) // Add 0.01 to avoid zero
            compare(addedSpy.count, count)
            compare(countChangedSpy.count, count)
            compare(sumChangedSpy.count, count)
            pieSeries.clear()
        }

        function test_05_remove() {
            removedSpy.clear()
            countChangedSpy.clear()
            sumChangedSpy.clear()

            var count = 50
            for (var i = 0; i < count; i++)
                pieSeries.append("slice" + i,
                                 Math.random() + 0.01) // Add 0.01 to avoid zero
            for (var j = 0; j < 10; j++)
                pieSeries.remove(pieSeries.at(0))

            compare(removedSpy.count, 10)
            compare(countChangedSpy.count, count + 10)
            compare(sumChangedSpy.count, count + 10)
            compare(pieSeries.count, 40)

            for (var j = 0; j < 10; j++)
                pieSeries.remove(0)

            compare(removedSpy.count, 20)
            compare(countChangedSpy.count, count + 20)
            compare(sumChangedSpy.count, count + 20)
            compare(pieSeries.count, 30)

            pieSeries.removeMultiple(0, 10)
            compare(removedSpy.count, 21)
            compare(countChangedSpy.count, count + 21)
            compare(sumChangedSpy.count, count + 30)
            compare(pieSeries.count, 20)

            pieSeries.clear()
        }

        function test_06_find_and_at() {
            var count = 50
            for (var i = 0; i < count; i++)
                pieSeries.append("slice" + i,
                                 Math.random() + 0.01) // Add 0.01 to avoid zero
            for (var j = 0; j < count; j++) {
                compare(pieSeries.find("slice" + j).label, "slice" + j)
            }
            pieSeries.clear()
        }

        function test_07_adjust_limit_and_mode() {
            modeChangedSpy.clear()
            spanChangedSpy.clear()

            var count = 10
            for (let i = 0; i < count; i++)
                pieSeries.append("slice" + i, i + 0.1) // Produces angle span range from ~0.8 to ~71.2

            pieSeries.angleSpanLabelVisibility = PieSeries.LabelVisibility.None
            compare(modeChangedSpy.count, 1)

            pieSeries.angleSpanVisibleLimit = 20
            compare(spanChangedSpy.count, 1)

            var visiblecount = 0
            for (let i = 0; i < count; i++)
                visiblecount += pieSeries.at(i).labelVisible
            compare(visiblecount, 7)

            pieSeries.angleSpanVisibleLimit = 35
            compare(spanChangedSpy.count, 2)

            visiblecount = 0
            for (let i = 0; i < count; i++)
                visiblecount += pieSeries.at(i).labelVisible
            compare(visiblecount, 5)

            pieSeries.angleSpanLabelVisibility = PieSeries.LabelVisibility.First
            compare(modeChangedSpy.count, 2)

            pieSeries.angleSpanVisibleLimit = 75 // This covers all the slices
            compare(spanChangedSpy.count, 3)

            visiblecount = 0
            for (let i = 0; i < count; i++)
                visiblecount += pieSeries.at(i).labelVisible
            compare(visiblecount, 1)

            pieSeries.angleSpanLabelVisibility = PieSeries.LabelVisibility.Even
            compare(modeChangedSpy.count, 3)

            visiblecount = 0
            for (let i = 0; i < count; i++)
                visiblecount += pieSeries.at(i).labelVisible
            compare(visiblecount, 5)

            pieSeries.clear()
        }

        function test_08_declarative_sliceData() {
            sliceDataChangedSpy.clear()
            compare(pieSeries.sliceData.length, 0)
            compare(pieSeries.sliceLabels.length, 0)

            pieSeries.sliceData = [4,3,6,7]
            compare(pieSeries.sliceData.length, 4)
            compare(pieSeries.count, 4)
            compare(pieSeries.at(2).value, 6)

            pieSeries.sliceData = [3.4,5,8.7,9,0,3]
            compare(pieSeries.sliceData.length, 6)
            compare(pieSeries.count, 6)
            compare(pieSeries.at(1).value, 5)


            pieSeries.sliceData = []
            compare(pieSeries.sliceData.length, 0)
            compare(pieSeries.count, 0)

            compare(sliceDataChangedSpy.count, 3)

            pieSeries.clear();
        }

        function test_09_declarative_sliceLabels() {
            sliceLabelsChangedSpy.clear()
            compare(pieSeries.sliceLabels.length, 0)

            pieSeries.sliceLabels = ["A","B", "C", "D"]
            compare(pieSeries.sliceLabels.length, 4)

            pieSeries.sliceData = [4,3,6,7]
            compare(pieSeries.at(1).label, 'B')

            pieSeries.sliceData = [3.4,5,8.7,9,0,3]
            compare(pieSeries.at(1).label, 'B')
            compare(pieSeries.at(4).label, '5.0')

            pieSeries.sliceLabels = ["h","j", "k"]
            compare(pieSeries.sliceLabels.length, 3)
            compare(pieSeries.at(1).label, 'j')
            compare(pieSeries.at(3).label, '4.0')

            pieSeries.sliceLabels = []
            compare(sliceLabelsChangedSpy.count, 3)

            pieSeries.clear();
        }

        function test_10_mix_declarative_imperative() {
            let warningMessage = new RegExp("Mixing declarative and imperative datainput is not supported.")
            ignoreWarning(warningMessage)
            initializedDeclarative.append("slice4" , Math.random() + 0.01)

            ignoreWarning(warningMessage)
            initializedDeclarative.append(slice1)

            var slices = [slice2, slice3]
            ignoreWarning(warningMessage)
            initializedDeclarative.append(slices)
            ignoreWarning(warningMessage)
            initializedDeclarative.removeMultiple(1, 2)
            ignoreWarning(warningMessage)
            initializedDeclarative.remove(slice1)
            ignoreWarning(warningMessage)
            initializedDeclarative.insert(0, slice1)
            ignoreWarning(warningMessage)
            initializedDeclarative.replace(0, slice3)
            ignoreWarning(warningMessage)
            initializedDeclarative.replace(slice2, slice3)
            ignoreWarning(warningMessage)
            initializedDeclarative.replace(slices)
            ignoreWarning(warningMessage)
            initializedDeclarative.take(slice2)
            initializedDeclarative.sliceLabels = ["h","j", "k"]
            initializedDeclarative.clear()
            initializedDeclarative.sliceLabels = ["h","j", "k"]
            ignoreWarning(warningMessage)
            initializedDeclarative.append(slice1)
        }

        function test_11_mix_imperative_declarative() {
            let warningMessage = new RegExp("Mixing declarative and imperative datainput is not supported.")
            ignoreWarning(warningMessage)
            initializedImperative.sliceData = [4, 5, 6, 2, 3, 8]

            initializedImperative.clear()
            initializedImperative.append(slice1)

            ignoreWarning(warningMessage)
            initializedImperative.sliceData = [4, 5, 6, 2, 3, 8]
            ignoreWarning(warningMessage)
            initializedImperative.sliceLabels = ["h","j", "k"]

            initializedImperative.clear()
            initializedImperative.sliceData = [4, 5, 6, 2, 3, 8]
        }
    }
}
