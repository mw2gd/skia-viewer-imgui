#include "app/AppFactory.hpp"
#include <memory>

int main(int argc, char** argv)
{
    // create app
    std::unique_ptr<App> app =
        AppFactory::createRenderer();

    // perform initialization, run loop,
    // and cleanup before exit
    app->init();
    app->loop();
    app->cleanup();

    return 0;
}

// /*
//  * Copyright 2015 Google Inc.
//  *
//  * Use of this source code is governed by a BSD-style license that can be
//  * found in the LICENSE file.
//  *
//  */
// #include "include/gpu/ganesh/gl/GrGLDirectContext.h"
// #include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
// #include "include/gpu/ganesh/gl/GrGLTypes.h"
// #include "include/core/SkSwizzle.h"
// #include "include/core/SkVertices.h"
// #include "include/core/SkRefCnt.h"
// #include "include/gpu/ganesh/mtl/GrMtlBackendSurface.h"
// #include "include/gpu/ganesh/mtl/GrMtlDirectContext.h"
// #include "include/gpu/ganesh/mtl/GrMtlBackendContext.h"
// #include "example/external_client/src/ganesh_metal_context_helper.h"
// #include "include/core/SkColorSpace.h"
// #include "include/gpu/ganesh/GrBackendSurface.h"
// #include "SDL2/SDL.h"
// #include <SDL2/SDL_syswm.h>
// #include "include/gpu/ganesh/GrDirectContext.h"
// #include "include/gpu/ganesh/SkSurfaceGanesh.h"

// #include "imgui.h"
// #include "backends/imgui_impl_sdl2.h"

// #include "include/core/SkPath.h"

// #include "include/core/SkCanvas.h"
// #include "include/core/SkFont.h"
// #include "include/core/SkSurface.h"
// #include "src/base/SkRandom.h"
// #include "include/private/base/SkTDArray.h"
// #include "include/private/base/SkTArray.h"
// #include "include/core/SkShader.h"

// #include "include/ports/SkCFObject.h"

// #include "include/gpu/ganesh/gl/GrGLInterface.h"
// #include <cstdint>
// #include <iostream>

// #include "backends/darwin/metal.hpp"

// SkCanvas* canvas;
// SkPaint paint;
// SkPaint fFontPaint;
// SDL_Window* window;
// SDL_DisplayMode dm;
// SDL_GLContext glContext = nullptr;
// sk_sp<GrDirectContext> grContext;
// GrGLFramebufferInfo info;

// static void handle_error() {
//     const char* error = SDL_GetError();
//     SkDebugf("SDL Error: %s\n", error);
//     SDL_ClearError();
// }

// int callbackhandler(void *userdata, SDL_Event * event)
// {
//     Uint64 prevTime = 0;
//     if (event->type == SDL_WINDOWEVENT &&
//         event->window.event == SDL_WINDOWEVENT_EXPOSED)
//     {
//         printf("window size changed!\n");
//     }
//     return 0;
// }

// static void build_ImFontAtlas(ImFontAtlas& atlas, SkPaint& fontPaint) {
//     int w, h;
//     unsigned char* pixels;
//     atlas.GetTexDataAsAlpha8(&pixels, &w, &h);
//     SkImageInfo info = SkImageInfo::MakeA8(w, h);
//     SkPixmap pmap(info, pixels, info.minRowBytes());
//     SkMatrix localMatrix = SkMatrix::Scale(1.0f / w, 1.0f / h);
//     auto fontImage = SkImages::RasterFromPixmap(pmap, nullptr, nullptr);
//     auto fontShader = fontImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear), localMatrix);
//     fontPaint.setShader(fontShader);
//     fontPaint.setColor(SK_ColorWHITE);
//     atlas.TexID = reinterpret_cast<uintptr_t>(&fontPaint);
// }

// int main(int argc, char** argv)
// {
//     static const int kStencilBits = 8;  // Skia needs 8 stencil bits
//     // If you want multisampling, uncomment the below lines and set a sample count
//     static const int kMsaaSampleCount = 0; //4;

//     /*
//      * In a real application you might want to initialize more subsystems
//      */
//     if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) {
//         handle_error();
//         return 1;
//     }

//     uint32_t windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_METAL;
//     window = SDL_CreateWindow("SDL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
//         800, 600, windowFlags);
//     if (!window) {
//         handle_error();
//         return 1;
//     }

//     sk_sp<SkSurface> surface = setupMetalSkSurface(window);
//     SkASSERT(surface != nullptr);

//      // Initialize ImGui context
//     // ImGui::CreateContext();
//     // ImGuiIO& io = ImGui::GetIO();
//     // build_ImFontAtlas(*io.Fonts, fFontPaint);

//     // // Initialize ImGui SDL2 backend (input handling only)
//     // ImGui_ImplSDL2_InitForOther(window);

//     // SDL_AddEventWatch(callbackhandler, nullptr);

//     bool running = true;
//     SDL_Event event;
//     Uint64 prevTime = 0;
//     while (running)
//     {
//         while(SDL_PollEvent(&event))
//         {
//             switch (event.type)
//             {
//                 case SDL_QUIT:
//                     running = false;
//                     break;
//             }
//             // ImGui_ImplSDL2_ProcessEvent(&event);
//         }

//         canvas = surface->getCanvas();
//         SkASSERT(canvas != nullptr);
//         canvas->clear(SK_ColorWHITE);

//         paint.setColor(SK_ColorBLUE);
//         paint.setAntiAlias(true);  // For smooth edges
//         canvas->drawCircle(50,50,100,paint);
        
//         if (auto dContext = GrAsDirectContext(canvas->recordingContext())) {
//             dContext->flushAndSubmit();
//         }
//         flush();

//         SDL_Delay(100000);
//     }
//     //Destroy window
//     SDL_DestroyWindow(window);
//     //Quit SDL subsystems
//     SDL_Quit();
//     return 0;
// }
