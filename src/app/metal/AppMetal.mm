// Objective C++ Headers
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

// C++ Headers 
#include "app/metal/AppMetal.hpp"

#include "include/core/SkColorSpace.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"
#include "include/core/SkSurface.h"
#include "include/core/SkColorType.h"
#include "include/gpu/ganesh/GrTypes.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/mtl/GrMtlBackendContext.h"
#include "include/gpu/ganesh/mtl/GrMtlBackendSurface.h"
#include "include/gpu/ganesh/mtl/GrMtlDirectContext.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "src/gpu/ganesh/mtl/GrMtlTrampoline.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/ganesh/gl/GrGLTypes.h"
#include "include/core/SkSwizzle.h"
#include "include/core/SkVertices.h"
#include "include/core/SkRefCnt.h"
#include "include/gpu/ganesh/mtl/GrMtlBackendSurface.h"
#include "include/gpu/ganesh/mtl/GrMtlDirectContext.h"
#include "include/gpu/ganesh/mtl/GrMtlBackendContext.h"
#include "example/external_client/src/ganesh_metal_context_helper.h"
#include "include/core/SkColorSpace.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "SDL3/SDL.h"
#include "include/gpu/ganesh/GrDirectContext.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"

#include "imgui.h"
#include "backends/imgui_impl_sdl3.h"

#include "include/core/SkPath.h"

#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkSurface.h"
#include "src/base/SkRandom.h"
#include "include/private/base/SkTDArray.h"
#include "include/private/base/SkTArray.h"
#include "include/core/SkShader.h"

#include "include/ports/SkCFObject.h"

#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include <cstdint>
#include <iostream>

/*
 * This function does necessary setup of sdl and metal so
 * that skia drawing to the sdl window can be performed.
 */
int MetalApp::init()
{
    SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal");
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow("MetalWindow",800, 600, WFLAGS);

    // Create a CAMetalLayer-backed NSView/UIView and
    // attach it to the specified window.
    SDL_MetalView metalView = SDL_Metal_CreateView(_window);

    // Get the NSView
    NSView* nsView = (__bridge NSView*)metalView;
    SkASSERT(nsView != nullptr);
    NSRect frame = nsView.frame; // Get the frame of the view
    NSLog(@"Width: %f, Height: %f", frame.size.width, frame.size.height);

    // get the CAMetalLayer
    CAMetalLayer* metalLayer = (__bridge CAMetalLayer*)SDL_Metal_GetLayer(metalView); 
    SkASSERT(metalLayer != nullptr);
    metalLayer.device = MTLCreateSystemDefaultDevice();
    metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    metalLayer.contentsScale = nsView.window.backingScaleFactor; // Set scale factor based on the window

    // create metal device
    const id<MTLDevice> device = metalLayer.device;
    const id<MTLCommandQueue> queue = [device newCommandQueue];

    // initialize imgui
    imguiInit();

    // setup event watcher
    SDL_AddEventWatch(callback, this);

    // save variables to object state
    _nsView     = (__bridge void*)nsView;
    _metalLayer = (__bridge void*)metalLayer;
    _device     = (__bridge void*)device;
    _queue      = (__bridge void*)queue;

    return 0;
}

/*
 * This function does one frame of drawing. This requires committing
 * the command buffer.
 */
void MetalApp::draw()
{
    // update sdl window size 
    SDL_GetWindowSizeInPixels(_window, &_dw, &_dh);

    // get drawable
    id<CAMetalDrawable> drawable = [(CAMetalLayer*)_metalLayer nextDrawable];

    // create GrDirectContext
    GrMtlBackendContext grBackendContext = {};
    grBackendContext.fDevice.reset(_device);
    grBackendContext.fQueue.reset(_queue);
    sk_sp<GrDirectContext> grDirectContext = GrDirectContexts::MakeMetal(grBackendContext);
    SkASSERT(grDirectContext != nullptr);

    // create GrMtlTextureInfo
    GrMtlTextureInfo fbInfo;
    fbInfo.fTexture.retain(drawable.texture);

    // create GrBackendRenderTarget
    GrBackendRenderTarget grTarget = GrBackendRenderTargets::MakeMtl(_dw, _dh, fbInfo);

    // create SkSurfaceProps
    SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag,
                            SkPixelGeometry::kUnknown_SkPixelGeometry);

    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendRenderTarget(
                                    grDirectContext.get(),
                                    grTarget,
                                    kBottomLeft_GrSurfaceOrigin,
                                    kBGRA_8888_SkColorType,
                                    nullptr,
                                    &props);

    _canvas = surface->getCanvas();
    SkASSERT(_canvas != nullptr);
    _canvas->clear(SK_ColorWHITE); // clear canvas every frame

    // common drawing logic (metal or opengl)
    drawcommon();
    
    if (auto dContext = GrAsDirectContext(_canvas->recordingContext())) {
        dContext->flushAndSubmit();
    }

    id<MTLCommandBuffer> buffer = [(const id<MTLCommandQueue>)_queue commandBuffer];
    [buffer presentDrawable:drawable];
    [buffer commit];
}

void MetalApp::cleanup()
{
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
