#include "app/opengl/AppOpenGL.hpp"
#include <GL/glew.h>
#include "include/core/SkColorSpace.h"
#include "include/gpu/ganesh/gl/GrGLDirectContext.h"
#include "include/gpu/ganesh/gl/GrGLBackendSurface.h"
#include "include/gpu/ganesh/GrBackendSurface.h"
#include "include/gpu/ganesh/SkSurfaceGanesh.h"

/*
 * This function does necessary setup of sdl and metal so
 * that skia drawing to the sdl window can be performed.
 */
int OpenGLApp::init()
{
    // SDL GL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // If you want multisampling, uncomment the below lines and set a sample count
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    // SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, kMsaaSampleCount);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, kStencilBits);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    // initialize SDL
    SkASSERT(SDL_Init(SDL_INIT_VIDEO) == true);

    // setup sdl window. This code will create a window
    // with the same resolution as the user's desktop.
    _window = SDL_CreateWindow("GLWindow", 800, 600, WFLAGS);
    SkASSERT(_window != nullptr);

    // try and setup a GL context
    _glContext = SDL_GL_CreateContext(_window);
    SkASSERT(_glContext != nullptr);

    // setup contexts
    _interface = GrGLMakeNativeInterface();
    SkASSERT(_interface != nullptr);
    _grContext = GrDirectContexts::MakeGL(_interface);
    SkASSERT(_grContext != nullptr);

    // Wrap the frame buffer object attached to the screen in a Skia render target so Skia can
    // render to it
    GrGLint buffer;
    glGetIntegerv( GL_FRAMEBUFFER_BINDING, &buffer);
    _info.fFBOID = (GrGLuint) buffer;
    _info.fFormat =  GL_RGB8;

    // initialize ImGui
    imguiInit();

    // // setup event watcher
    SDL_AddEventWatch(callback, this);

    return 0;
}

/*
 * This function does one frame of drawing. 
 */
void OpenGLApp::draw()
{
    // set viewport size
    SDL_GetWindowSizeInPixels(_window, &_dw, &_dh);
    glViewport(0, 0, _dw, _dh);

    // create sksurface using opengl backend
    GrBackendRenderTarget target = GrBackendRenderTargets::MakeGL(_dw, _dh, kMsaaSampleCount, kStencilBits, _info);
    SkSurfaceProps props(SkSurfaceProps::kUseDeviceIndependentFonts_Flag, SkPixelGeometry::kUnknown_SkPixelGeometry);
    sk_sp<SkSurface> surface = SkSurfaces::WrapBackendRenderTarget(_grContext.get(), target, kBottomLeft_GrSurfaceOrigin,
        kRGB_888x_SkColorType, nullptr, &props);
    SkASSERT(surface != nullptr);

    // get canvas to pass drawing commands
    _canvas = surface->getCanvas();
    SkASSERT(_canvas != nullptr);
    _canvas->clear(SK_ColorWHITE); // clear canvas every frame

    // common drawing logic (metal or opengl)
    drawcommon();

    // submit drawings calls 
    if (auto dContext = GrAsDirectContext(_canvas->recordingContext())) {
        dContext->flushAndSubmit();
    }
    SDL_GL_SwapWindow(_window);
}

void OpenGLApp::cleanup()
{
    SkASSERT(_glContext != nullptr);
    SDL_GL_DestroyContext(_glContext);
    SDL_DestroyWindow(_window);
    SDL_Quit();
}
