// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef SERIESTHEME_H
#define SERIESTHEME_H

#include <QtGui/QColor>
#include <QtGui/QFont>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

QT_BEGIN_NAMESPACE

class SeriesTheme : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(SeriesColorTheme colorTheme READ colorTheme WRITE setColorTheme NOTIFY colorThemeChanged FINAL)
    Q_ENUMS(SeriesColorTheme)
    QML_ELEMENT

public:
    enum SeriesColorTheme {
        SeriesTheme1 = 0,
        SeriesTheme2
    };

    explicit SeriesTheme(QObject *parent = nullptr);

    void resetColorTheme();

    int graphSeriesCount() const;
    void setGraphSeriesCount(int count);
    QColor graphSeriesColor(int index) const;

    SeriesTheme::SeriesColorTheme colorTheme() const;
    void setColorTheme(const SeriesTheme::SeriesColorTheme &newColorTheme);

protected:
    // from QDeclarativeParserStatus
    void classBegin() override;
    void componentComplete() ;

Q_SIGNALS:
    void update();
    void colorThemeChanged();

private:
    void setColorTheme1();
    void setColorTheme2();

private:
    bool m_componentComplete = false;
    // TODO: Consider more detailed dirty flags
    bool m_themeDirty = true;

    SeriesColorTheme m_defaultColorTheme = SeriesColorTheme::SeriesTheme1;
    SeriesColorTheme m_colorTheme = SeriesColorTheme::SeriesTheme1;
    QList<QColor> m_seriesColors;
    // TODO: Support for these?
    //QList<QColor> m_seriesBorderColors;
    //QList<QColor> m_seriesLabelColors;
    int m_seriesCount = 4;
};

#endif // SERIESTHEME_H
