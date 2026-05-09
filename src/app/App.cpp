#include "app/App.hpp"
#include "backends/imgui_impl_sdl3.h"
#include "include/private/base/SkAssert.h"
#include "include/private/base/SkTDArray.h"
#include "include/core/SkShader.h"
#include "include/core/SkVertices.h"
#include "include/core/SkSwizzle.h"

//===================================================
// public common functions (platform agnostic)
//===================================================

/**
 * This function runs a while loop at 60hz. Each iteration is
 * a rendering frame.
 */
void App::loop()
{
    while (!_quit) {
        while (SDL_PollEvent(&_event) != 0)
        {
            switch (_event.type) 
            {
                case SDL_EVENT_QUIT:
                    _quit = true;
                    break;
            }
            ImGui_ImplSDL3_ProcessEvent(&_event);
        }

        // render all the content using skia
        // requires subclass implementation
        draw();
        
        SDL_Delay(16); // 60hz
    }
}


//===================================================
// private common functions (platform agnostic)
//===================================================
/**
 * This sets up imgui for use with sdl and skia
 */
bool App::callback(void *userdata, SDL_Event *event)
{
    App *app = dynamic_cast<App*>(static_cast<App*>(userdata));
    SkASSERT(app != nullptr);

    if (event->type == SDL_EVENT_WINDOW_EXPOSED)
    {
        app->draw();
    }

    return true;
}

/**
 * This sets up imgui for use with sdl and skia
 */
void App::imguiInit()
{
    // initialize imgui context
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // build font atlas for imgui
    int w, h;
    unsigned char* pixels;
    io.Fonts->GetTexDataAsAlpha8(&pixels, &w, &h);
    SkImageInfo info = SkImageInfo::MakeA8(w, h);
    SkPixmap pmap(info, pixels, info.minRowBytes());
    SkMatrix localMatrix = SkMatrix::Scale(1.0f / w, 1.0f / h);
    auto fontImage = SkImages::RasterFromPixmap(pmap, nullptr, nullptr);
    auto fontShader = fontImage->makeShader(SkSamplingOptions(SkFilterMode::kLinear), localMatrix);
    _fFontPaint.setShader(fontShader);
    _fFontPaint.setColor(SK_ColorWHITE);
    io.Fonts->TexID = reinterpret_cast<uintptr_t>(&_fFontPaint);

    // Initialize ImGui SDL2 backend (input handling only)
    ImGui_ImplSDL3_InitForOther(_window);
}

/**
 * This function renders graphics to the screen using skia.
 * Assumes skia has already been setup with backend.
 */
void App::drawcommon()
{
    //########################
    //Render IMGUI
    //########################
    int w, h;
    SDL_GetWindowSize(_window, &w, &h);

    // update imgui display size based on window size
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)w, (float)h);

    // update imgui delta between frames
    Uint64 currTime = SDL_GetPerformanceCounter();
    io.DeltaTime = (float)(currTime - _prevTime) / (float)SDL_GetPerformanceFrequency();
    _prevTime = currTime;

    ImGui::NewFrame();
    ImGui::ShowDemoWindow();

    // This causes ImGui to rebuild vertex/index data based on all immediate-mode commands
    // (widgets, etc...) that have been issued
    ImGui::Render();

    const ImDrawData* drawData = ImGui::GetDrawData();
    SkTDArray<SkPoint> pos;
    SkTDArray<SkPoint> uv;
    SkTDArray<SkColor> color;
    for (int i = 0; i < drawData->CmdListsCount; ++i) {
        const ImDrawList* drawList = drawData->CmdLists[i];

        // De-interleave all vertex data (sigh), convert to Skia types
        pos.clear(); uv.clear(); color.clear();
        for (int j = 0; j < drawList->VtxBuffer.size(); ++j) {
            const ImDrawVert& vert = drawList->VtxBuffer[j];
            pos.push_back(SkPoint::Make(vert.pos.x, vert.pos.y));
            uv.push_back(SkPoint::Make(vert.uv.x, vert.uv.y));
            color.push_back(vert.col);
        }
        // ImGui colors are RGBA
        SkSwapRB(color.begin(), color.begin(), color.size());

        int indexOffset = 0;

        // Draw everything with canvas.drawVertices...
        for (int j = 0; j < drawList->CmdBuffer.size(); ++j) {
            const ImDrawCmd* drawCmd = &drawList->CmdBuffer[j];

            SkAutoCanvasRestore acr(_canvas, true);

            // TODO: Find min/max index for each draw, so we know how many vertices (sigh)
            if (drawCmd->UserCallback) {
                drawCmd->UserCallback(drawList, drawCmd);
            } else {
                intptr_t idIndex = (intptr_t)drawCmd->TextureId;

                SkPaint* paint = reinterpret_cast<SkPaint*>((uintptr_t)drawCmd->TextureId);
                SkASSERT(paint);

                _canvas->clipRect(SkRect::MakeLTRB(drawCmd->ClipRect.x, drawCmd->ClipRect.y,
                                                drawCmd->ClipRect.z, drawCmd->ClipRect.w));
                auto vertices = SkVertices::MakeCopy(SkVertices::kTriangles_VertexMode,
                                                    drawList->VtxBuffer.size(),
                                                    pos.begin(), uv.begin(), color.begin(),
                                                    drawCmd->ElemCount,
                                                    drawList->IdxBuffer.begin() + indexOffset);
                _canvas->drawVertices(vertices, SkBlendMode::kModulate, *paint);

                indexOffset += drawCmd->ElemCount;
            }
        }
    }

    //########################
    //end
    //########################
}