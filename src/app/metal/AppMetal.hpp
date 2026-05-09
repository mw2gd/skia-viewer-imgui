#ifndef APP_METAL_H
#define APP_METAL_H

#include "app/App.hpp"

class MetalApp : public App
{
    int init();
    void draw();
    void cleanup();
private:
    // void type is much easier to
    // use in c++ style header
    void              *_nsView;
    void              *_metalLayer;
    void              *_device;
    void              *_queue;

    const int WFLAGS = SDL_WINDOW_RESIZABLE          |
                       SDL_WINDOW_HIGH_PIXEL_DENSITY |
                       SDL_WINDOW_METAL;
};

#endif