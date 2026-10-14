// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#ifndef QTGRAPHS_QCUSTOM3DVOLUME_H
#define QTGRAPHS_QCUSTOM3DVOLUME_H

#include <QtGraphs/qcustom3ditem.h>
#include <QtGraphs/qgraphsglobal.h>
#include <QtGui/qcolor.h>
#include <QtGui/qimage.h>

QT_BEGIN_NAMESPACE

class QCustom3DVolumePrivate;
class QQuaternion;

class Q_GRAPHS_EXPORT QCustom3DVolume : public QCustom3DItem
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QCustom3DVolume)
    Q_PROPERTY(
        int textureWidth READ textureWidth WRITE setTextureWidth NOTIFY textureWidthChanged FINAL)
    Q_PROPERTY(int textureHeight READ textureHeight WRITE setTextureHeight NOTIFY
                   textureHeightChanged FINAL)
    Q_PROPERTY(
        int textureDepth READ textureDepth WRITE setTextureDepth NOTIFY textureDepthChanged FINAL)
    Q_PROPERTY(int sliceIndexX READ sliceIndexX WRITE setSliceIndexX NOTIFY sliceIndexXChanged FINAL)
    Q_PROPERTY(int sliceIndexY READ sliceIndexY WRITE setSliceIndexY NOTIFY sliceIndexYChanged FINAL)
    Q_PROPERTY(int sliceIndexZ READ sliceIndexZ WRITE setSliceIndexZ NOTIFY sliceIndexZChanged FINAL)
    Q_PROPERTY(
        QList<QRgb> colorTable READ colorTable WRITE setColorTable NOTIFY colorTableChanged FINAL)
    Q_PROPERTY(QList<uchar> *textureData READ textureData WRITE setTextureData NOTIFY
                   textureDataChanged FINAL)
    Q_PROPERTY(float alphaMultiplier READ alphaMultiplier WRITE setAlphaMultiplier NOTIFY
                   alphaMultiplierChanged FINAL)
    Q_PROPERTY(bool preserveOpacity READ preserveOpacity WRITE setPreserveOpacity NOTIFY
                   preserveOpacityChanged FINAL)
    Q_PROPERTY(bool useHighDefShader READ useHighDefShader WRITE setUseHighDefShader NOTIFY
                   useHighDefShaderChanged FINAL)
    Q_PROPERTY(bool drawSlices READ drawSlices WRITE setDrawSlices NOTIFY drawSlicesChanged FINAL)
    Q_PROPERTY(bool drawSliceFrames READ drawSliceFrames WRITE setDrawSliceFrames NOTIFY
                   drawSliceFramesChanged FINAL)
    Q_PROPERTY(QColor sliceFrameColor READ sliceFrameColor WRITE setSliceFrameColor NOTIFY
                   sliceFrameColorChanged FINAL)
    Q_PROPERTY(QVector3D sliceFrameWidths READ sliceFrameWidths WRITE setSliceFrameWidths NOTIFY
                   sliceFrameWidthsChanged FINAL)
    Q_PROPERTY(QVector3D sliceFrameGaps READ sliceFrameGaps WRITE setSliceFrameGaps NOTIFY
                   sliceFrameGapsChanged FINAL)
    Q_PROPERTY(QVector3D sliceFrameThicknesses READ sliceFrameThicknesses WRITE
                   setSliceFrameThicknesses NOTIFY sliceFrameThicknessesChanged FINAL)
    QML_NAMED_ELEMENT(Custom3DVolume)

public:
    explicit QCustom3DVolume(QObject *parent = nullptr);
    explicit QCustom3DVolume(QVector3D position,
                             QVector3D scaling,
                             const QQuaternion &rotation,
                             int textureWidth,
                             int textureHeight,
                             int textureDepth,
                             QList<uchar> *textureData,
                             QImage::Format textureFormat,
                             const QList<QRgb> &colorTable,
                             QObject *parent = nullptr);
    ~QCustom3DVolume() override;

    int textureWidth() const;
    int textureHeight() const;
    int textureDepth() const;
    int textureDataWidth() const;

    int sliceIndexX() const;
    int sliceIndexY() const;
    int sliceIndexZ() const;

    QList<QRgb> colorTable() const;

    QList<uchar> *createTextureData(const QList<QImage *> &images);
    QList<uchar> *textureData() const;

    QImage::Format textureFormat() const;

    float alphaMultiplier() const;
    bool preserveOpacity() const;

    bool useHighDefShader() const;

    bool drawSlices() const;
    bool drawSliceFrames() const;

    QColor sliceFrameColor() const;
    QVector3D sliceFrameWidths() const;
    QVector3D sliceFrameGaps() const;
    QVector3D sliceFrameThicknesses() const;

    QImage renderSlice(Qt::Axis axis, int index);

public Q_SLOTS:
    void setTextureWidth(int value);
    void setTextureHeight(int value);
    void setTextureDepth(int value);
    void setTextureDimensions(int width, int height, int depth);
    void setSliceIndexX(int value);
    void setSliceIndexY(int value);
    void setSliceIndexZ(int value);
    void setSliceIndices(int x, int y, int z);
    void setColorTable(const QList<QRgb> &colors);
    void setTextureData(QList<uchar> *data);
    void setSubTextureData(Qt::Axis axis, int index, const uchar *data);
    void setSubTextureData(Qt::Axis axis, int index, const QImage &image);
    void setTextureFormat(QImage::Format format);
    void setAlphaMultiplier(float mult);
    void setPreserveOpacity(bool enable);
    void setUseHighDefShader(bool enable);
    void setDrawSlices(bool enable);
    void setDrawSliceFrames(bool enable);
    void setSliceFrameColor(QColor color);
    void setSliceFrameWidths(QVector3D values);
    void setSliceFrameGaps(QVector3D values);
    void setSliceFrameThicknesses(QVector3D values);

Q_SIGNALS:
    void textureWidthChanged(int value);
    void textureHeightChanged(int value);
    void textureDepthChanged(int value);
    void sliceIndexXChanged(int value);
    void sliceIndexYChanged(int value);
    void sliceIndexZChanged(int value);
    void colorTableChanged();
    void textureDataChanged(QList<uchar> *data);
    void textureFormatChanged(QImage::Format format);
    void alphaMultiplierChanged(float mult);
    void preserveOpacityChanged(bool enabled);
    void useHighDefShaderChanged(bool enabled);
    void drawSlicesChanged(bool enabled);
    void drawSliceFramesChanged(bool enabled);
    void sliceFrameColorChanged(QColor color);
    void sliceFrameWidthsChanged(QVector3D values);
    void sliceFrameGapsChanged(QVector3D values);
    void sliceFrameThicknessesChanged(QVector3D values);

private:
    Q_DISABLE_COPY(QCustom3DVolume)
};

QT_END_NAMESPACE

#endif
