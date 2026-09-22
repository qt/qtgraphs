// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only
// Qt-Security score:significant reason:default


#include <QtGui/qtguiglobal.h>
#if QT_CONFIG(opengl)
#include <QtGui/qoffscreensurface.h>
#endif

#include "commonutils_p.h"

#include <QtCore/qvarlengtharray.h>
#include <rhi/qrhi.h>

QT_BEGIN_NAMESPACE

static qreal s_maxTextureSize = 0.;

Q_LOGGING_CATEGORY(lcGraphsUtils, "qt.graphs.common.utils")

qreal CommonUtils::maxTextureSize()
{
    // Query maximum texture size only once
    if (!s_maxTextureSize) {
#if QT_CONFIG(opengl)
        std::unique_ptr<QSurface> surfacePtr;
#endif
        std::unique_ptr<QRhi> rhi;
#if defined(Q_OS_WIN)
        QRhiD3D12InitParams params;
        rhi.reset(QRhi::create(QRhi::D3D12, &params));
#elif defined(Q_OS_MACOS) || defined(Q_OS_IOS)
        QRhiMetalInitParams params;
        rhi.reset(QRhi::create(QRhi::Metal, &params));
#elif QT_CONFIG(opengl)
        QRhiGles2InitParams params;
        surfacePtr.reset(QRhiGles2InitParams::newFallbackSurface());
        params.fallbackSurface = surfacePtr.get();
        rhi.reset(QRhi::create(QRhi::OpenGLES2, &params));
#elif QT_CONFIG(vulkan)
        if (!qEnvironmentVariable("QSG_RHI_BACKEND").compare("vulkan")) {
            QVulkanInstance inst;
            inst.setExtensions(QRhiVulkanInitParams::preferredInstanceExtensions());
            if (inst.create()) {
                QRhiVulkanInitParams params;
                params.inst = &inst;
                rhi.reset(QRhi::create(QRhi::Vulkan, &params));
            } else {
                qCWarning(lcGraphsUtils, "Failed to create Vulkan instance");
            }
        }
#endif
        if (rhi)
            s_maxTextureSize = qreal(rhi->resourceLimit(QRhi::TextureSizeMax));
        else
            s_maxTextureSize = gradientTextureWidth;
    }

    return s_maxTextureSize;
}

QString CommonUtils::formatNumber(double number, const QString &format)
{
    const qsizetype len = format.length();
    if (len == 1) {
        static constexpr auto floatConversions = QLatin1StringView("eEfFgG");
        const QChar c = format.front();
        if (floatConversions.contains(c))
            return QString::number(number, c.toLatin1());
    }

    // Build the printf-style spec in stack storage (QVarLengthArray) instead of
    // QByteArray/QString concatenation, avoiding a heap allocation for the common
    // case of short format strings. A bare single-letter conversion (e.g. "d")
    // needs a leading '%' added.
    const bool prependPercent = (len == 1);
    const qsizetype fmtLen = len + prependPercent;
    QVarLengthArray<char, 32> fmt(fmtLen + 1);
    char *dst = fmt.data();
    if (prependPercent)
        *dst++ = '%';
    for (QChar ch : format)
        *dst++ = ch.toLatin1();
    *dst = '\0';

    static constexpr auto intConversions = QLatin1StringView("iIoOxX");
    const QChar conv = format.back();
    if (intConversions.contains(conv)) {
        return QString::asprintf(
            fmt.constData(),
            fmtLen > 2 ? static_cast<qint64>(number) : static_cast<qint32>(number));
    } else {
        return QString::asprintf(fmt.constData(), number);
    }
}

QT_END_NAMESPACE
