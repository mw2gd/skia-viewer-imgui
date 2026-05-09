#ifndef APP_H
#define APP_H

#include <memory>
#include "SDL3/SDL.h"
#include "imgui.h"
#include "include/core/SkRefCnt.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/gpu/ganesh/GrDirectContext.h"

class App {
public:
    // public functions
    void virtual loop() final;
    virtual int init()      = 0;
    virtual void draw()     = 0;
    virtual void cleanup()  = 0;
    virtual ~App() {}
protected:
    // constructor
    App() {}

    // private functions
    void imguiInit();
    void drawcommon();
    static bool callback(void *userdata, SDL_Event *event);

    // class variables
    SDL_Window                  *_window;
    sk_sp<SkSurface>            _surface;
    SkCanvas                    *_canvas;
    SkPaint                     _paint;
    SkPaint                     _fFontPaint;
    sk_sp<GrDirectContext>      _grDirectContext;
    SDL_DisplayMode             _dm;
    int                         _dw, _dh;
    SDL_Event                   _event;
    bool                        _quit = false;
    float                       _prevTime = 0.0;
};

#endif