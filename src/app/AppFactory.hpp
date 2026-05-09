#ifndef APP_FACTORY_H
#define APP_FACTORY_H

#include "app/App.hpp"

//==============================
// platform specific includes
//==============================
#if defined(__APPLE__) && 0
    #include "app/metal/AppMetal.hpp"
#else
    #include "app/opengl/AppOpenGL.hpp"
#endif

//==============================
// platform specific factory
//==============================
class AppFactory
{
public:
    static std::unique_ptr<App> createRenderer()
    {
#if defined(__APPLE__) && 0
        // return metal version
        return std::make_unique<MetalApp>();
#else
        // return gl version
        return std::make_unique<OpenGLApp>();
#endif
    }
};

#endif