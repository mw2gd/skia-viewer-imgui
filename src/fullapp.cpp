/*
* Copyright 2017 Google Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/
#include "fullapp.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkColor.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontTypes.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkPaint.h"
#include "include/core/SkPoint.h"
#include "include/core/SkRect.h"
#include "include/core/SkShader.h"
#include "include/core/SkString.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTileMode.h"
#include "include/effects/SkGradientShader.h"
#include "tools/window/DisplayParams.h"
#include <string.h>

using namespace sk_app;
using skwindow::DisplayParams;

Application* Application::Create(int argc, char** argv, void* platformData) {
    return new FullApp(argc, argv, platformData);
}

FullApp::FullApp(int argc, char** argv, void* platformData)
        : fBackendType(Window::kRaster_BackendType),
        fRotationAngle(0) {
    SkGraphics::Init();
    fWindow = Windows::CreateNativeWindow(platformData);
    fWindow->setRequestedDisplayParams(DisplayParams());
    // register callbacks
    fWindow->pushLayer(this);
    fWindow->attach(fBackendType);
}

FullApp::~FullApp() {
    fWindow->detach();
    delete fWindow;
}

void FullApp::updateTitle() {
    if (!fWindow) {
        return;
    }
    SkString title("FullApp");
    fWindow->setTitle(title.c_str());
}

void FullApp::onBackendCreated() {
    this->updateTitle();
    fWindow->show();
    fWindow->inval();
}

void FullApp::onPaint(SkSurface* surface) {
    auto canvas = surface->getCanvas();
    
    // Clear background
    canvas->clear(SK_ColorWHITE);
    SkPaint paint;
    {
        SkPoint linearPoints[] = { { 0, 0 }, { 300, 300 } };
        SkColor linearColors[] = { SK_ColorGREEN, SK_ColorBLACK };
        paint.setShader(SkGradientShader::MakeLinear(linearPoints, linearColors, nullptr, 2,
                                                     SkTileMode::kMirror));
        paint.setAntiAlias(true);
        canvas->drawCircle(200, 200, 64, paint);
        canvas->drawCircle(200, 200, 64, paint);
        canvas->drawCircle(200, 200, 64, paint);

        // Detach shader
        paint.setShader(nullptr);
    }

    //ImGui::ShowDemoWindow(&fShowImGuiTestWindow);
}

void FullApp::onIdle() {
    // Just re-paint continuously
    fWindow->inval();
}

bool FullApp::onChar(SkUnichar c, skui::ModifierKey modifiers) {
    if (' ' == c) {
        if (Window::kRaster_BackendType == fBackendType) {
            SkDebugf("No GPU backend configured\n");
            return true;
        } else {
            fBackendType = Window::kRaster_BackendType;
        }
        fWindow->detach();
        fWindow->attach(fBackendType);
    }
    return true;
}