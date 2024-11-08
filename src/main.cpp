#include <SDL3/SDL.h>
#include <include/core/SkCanvas.h>
#include <include/core/SkGraphics.h>
#include <include/core/SkImage.h>
#include <include/core/SkSurface.h>
#include <include/core/SkPaint.h>
#include <include/core/SkPath.h>

#include <iostream>
#include <include/core/SkData.h>
#include <include/core/SkStream.h>
#include <include/core/SkImageInfo.h>

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

sk_sp<SkSurface> surface;
SkCanvas* canvas;

int oldWidth  = 1;
int oldHeight = 1;

static void render(SkCanvas* canvas)
{
    int width, height;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height));
    canvas = surface->getCanvas();

    // Clear the canvas with a white color
    canvas->clear(SK_ColorWHITE);

    // Create a paint object
    SkPaint paint;
    paint.setColor(SK_ColorBLUE);
    paint.setStrokeWidth(5);
    paint.setStyle(SkPaint::kStroke_Style);

    // Draw a rectangle
    canvas->drawRect(SkRect::MakeXYWH(10, 10, 10, 10), paint);

    // Get the pixels from the Skia surface
    SkPixmap pixmap;
    surface->peekPixels(&pixmap);

    // Update the SDL texture
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, pixmap.width(), pixmap.height());
    SDL_UpdateTexture(texture, nullptr, pixmap.addr(), pixmap.rowBytes());

    // Clear the SDL renderer
    SDL_RenderClear(renderer);

    // Render the texture to the SDL renderer
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);

    // Present the renderer
    SDL_RenderPresent(renderer);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

bool SDLCALL eventcallback(void *userdata, SDL_Event *event)
{            
    switch (event->type)
    {
        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
            render(canvas);
            break;
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        return -1;
    }

    #define WINDOW_WIDTH 640
    #define WINDOW_HEIGHT 480

    if (!SDL_CreateWindowAndRenderer("fullapp", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        return -1;
    }

    SkGraphics::Init();

    int width, height;
    SDL_GetWindowSizeInPixels(window, &width, &height);
    
    surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height));
    canvas = surface->getCanvas();

    canvas->clear(SK_ColorWHITE); // This should work without segfault

    SDL_AddEventWatch(eventcallback, nullptr);

    // Main loop
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
        render(canvas);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}