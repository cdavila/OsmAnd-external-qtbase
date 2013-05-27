/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWINRTSCREEN_H
#define QWINRTSCREEN_H

#include <qpa/qplatformscreen.h>
#include <qpa/qwindowsysteminterface.h>

#include <QtCore/QHash>
#include <QSurfaceFormat>

#ifdef Q_WINRT_GL
#  include <EGL/egl.h>
#endif

#include <EventToken.h>

namespace ABI {
    namespace Windows {
        namespace UI {
            namespace Core {
                struct IAutomationProviderRequestedEventArgs;
                struct ICharacterReceivedEventArgs;
                struct ICoreWindow;
                struct ICoreWindowEventArgs;
                struct IKeyEventArgs;
                struct IPointerEventArgs;
                struct IVisibilityChangedEventArgs;
                struct IWindowActivatedEventArgs;
                struct IWindowSizeChangedEventArgs;
            }
            namespace ViewManagement {
                struct IApplicationViewStatics;
            }
        }
        namespace Graphics {
            namespace Display {
                struct IDisplayPropertiesStatics;
            }
        }
    }
}
struct IInspectable;

QT_BEGIN_NAMESPACE

class QTouchDevice;
class QWinRTEGLContext;
class QWinRTPageFlipper;
class QWinRTCursor;
class QWinRTInputContext;

struct Pointer {
    enum Type { Unknown, Mouse, TouchScreen, Tablet };
    Type type;
    QTouchDevice *device;
};

class QWinRTScreen : public QPlatformScreen
{
public:
    QWinRTScreen(ABI::Windows::UI::Core::ICoreWindow *window);
    QRect geometry() const;
    int depth() const;
    QImage::Format format() const;
    QSurfaceFormat surfaceFormat() const;
    QWinRTInputContext *inputContext() const;
    QPlatformCursor *cursor() const;

    Qt::ScreenOrientation nativeOrientation() const;
    Qt::ScreenOrientation orientation() const;

    Qt::WindowState windowState() const;
    bool tryUnsnap();

#ifdef Q_WINRT_GL
    ABI::Windows::UI::Core::ICoreWindow *coreWindow() const;
    EGLDisplay eglDisplay() const;
    EGLSurface eglSurface() const;
#else
    QPlatformScreenPageFlipper *pageFlipper() const;

    void update(const QRegion &region, const QPoint &offset, const void *handle, int stride);
#endif

private:


    // Event handlers
    QHash<QEvent::Type, EventRegistrationToken> m_tokens;

    HRESULT onKey(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::IKeyEventArgs *args);
    HRESULT onCharacterReceived(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::ICharacterReceivedEventArgs *args);
    HRESULT onPointerEntered(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::IPointerEventArgs *args);
    HRESULT onPointerExited(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::IPointerEventArgs *args);
    HRESULT onPointerUpdated(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::IPointerEventArgs *args);
    HRESULT onSizeChanged(ABI::Windows::UI::Core::ICoreWindow *window, ABI::Windows::UI::Core::IWindowSizeChangedEventArgs *args);

    HRESULT onActivated(ABI::Windows::UI::Core::ICoreWindow *, ABI::Windows::UI::Core::IWindowActivatedEventArgs *args);
    HRESULT onClosed(ABI::Windows::UI::Core::ICoreWindow *, ABI::Windows::UI::Core::ICoreWindowEventArgs *args);
    HRESULT onVisibilityChanged(ABI::Windows::UI::Core::ICoreWindow *, ABI::Windows::UI::Core::IVisibilityChangedEventArgs *args);
    HRESULT onAutomationProviderRequested(ABI::Windows::UI::Core::ICoreWindow *, ABI::Windows::UI::Core::IAutomationProviderRequestedEventArgs *args);

    HRESULT onOrientationChanged(IInspectable *);

    void handleWindowState();

    ABI::Windows::UI::Core::ICoreWindow *m_window;
    ABI::Windows::UI::ViewManagement::IApplicationViewStatics *m_applicationView;
    bool m_visible;
    Qt::WindowState m_windowState;
    QRect m_geometry;
    QImage::Format m_format;
    QSurfaceFormat m_surfaceFormat;
    int m_depth;
    QWinRTInputContext *m_inputContext;
    QWinRTCursor *m_cursor;

    ABI::Windows::Graphics::Display::IDisplayPropertiesStatics *m_displayProperties;
    Qt::ScreenOrientation m_nativeOrientation;
    Qt::ScreenOrientation m_orientation;

    QHash<quint32, QString> m_activeKeys;
    QHash<quint32, Pointer> m_pointers;
    QHash<quint32, QWindowSystemInterface::TouchPoint> m_touchPoints;
#ifdef Q_WINRT_GL
    EGLDisplay m_eglDisplay;
    EGLSurface m_eglSurface;
#else
    QWinRTPageFlipper *m_pageFlipper;
#endif
};

QT_END_NAMESPACE

#endif // QWINRTSCREEN_H
