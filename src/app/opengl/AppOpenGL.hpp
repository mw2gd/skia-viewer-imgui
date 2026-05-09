#ifndef APP_OPENGL_H
#define APP_OPENGL_H

#include "include/gpu/ganesh/gl/GrGLInterface.h"
#include "app/App.hpp"

class OpenGLApp : public App
{
    int init();
    void draw();
    void cleanup();
private:
    const int kStencilBits = 8;
    const int kMsaaSampleCount = 0;

    SDL_GLContext              _glContext;
    sk_sp<const GrGLInterface> _interface;
    sk_sp<GrDirectContext>     _grContext;
    GrGLFramebufferInfo        _info;

    const int WFLAGS = SDL_WINDOW_RESIZABLE          |
                       SDL_WINDOW_HIGH_PIXEL_DENSITY |
                       SDL_WINDOW_OPENGL;
};

#endif