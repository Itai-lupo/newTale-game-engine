#ifdef __linux__
#include "surface.hpp"
#include "cpuRendering.hpp"
#include "openGLRendering.hpp"
#include "linuxWindowAPI.hpp"
#include "log.hpp"
#include "toplevel.hpp"
#include "subsurface.hpp"
#include "keyboard.hpp"
#include "pointer.hpp"
#include "layer.hpp"

#include <thread>

void surface::init()
{
    surfacePool = new entityPool(500);
    surfacesInfo = new surfaceInfoComponent(surfacePool);

    layer::init(surfacePool);
    toplevel::init(surfacePool);
}

void surface::close()
{
    delete surfacesInfo;
    delete surfacePool;
    layer::close();
    toplevel::close();
}

surfaceId surface::allocateSurface(windowId winId, const surfaceSpec& surfaceDataSpec)
{
    
    surfaceId id = surfacePool->allocEntity();

    surfaceData info;
    info.height = surfaceDataSpec.height;
    info.width = surfaceDataSpec.width;
    info.rule = surfaceDataSpec.rule;
    info.rendererType = surfaceDataSpec.rendererType;
    info.id = id;
    info.parentWindowId = winId;
    info.surface = wl_compositor_create_surface(compositor);

    CONDTION_LOG_FATAL("can't  create surface", info.surface == NULL);
    surfacesInfo->setComponent(id, info);

    

    switch (info.rule)
    {
        case surfaceRule::layer:
            layer::allocateLayer(id, info.surface, surfaceDataSpec);
            break;

        case surfaceRule::topLevel:
            toplevel::allocateTopLevel(id, info.surface, surfaceDataSpec);

            break;
        
        case surfaceRule::subsurface:
            subsurface::allocateSubsurface(id, info.surface, surfaceDataSpec);
            break;
        default:
            LOG_FATAL("need to set valid window rule")
            break;
    }

    switch (info.rendererType)
    {
        case surfaceRenderAPI::openGL:        
            openGLRendering::allocateSurfaceToRender(id);
            break;

        case surfaceRenderAPI::cpu:        
            cpuRendering::allocateSurfaceToRender(id);
            break;
    
        default:
            LOG_FATAL("need to set with render api")
            break;
    }
    
    keyboard::allocateWindowEvents(id);
    pointer::allocateWindowEvents(id);
    wl_surface_commit(info.surface);

    return id;
}

void surface::deallocateSurface(surfaceId winId)
{
    if(surfacePool->isIdValid(winId))
        return;



    wl_surface *temp = surfacesInfo->getComponent(winId)->surface;
    
    //todo: make this pool extensions that auto delete
    // keyboard::deallocateWindowEvents(winId);
    // pointer::deallocateWindowEvents(winId);
    // cpuRendering::deallocateSurfaceToRender(winId);
    // toplevel::deallocateTopLevel(winId);
    // layer::deallocateLayer(winId);
    surfacePool->freeEntity(winId);
    wl_surface_destroy(temp);

}

void surface::resize(surfaceId id, int width, int height)
{
    
    surfaceData *temp = surfacesInfo->getComponent(id);
    if(!temp)
        return;

    temp->width = width;
    temp->height = height;

    switch (temp->rendererType)
    {
        case surfaceRenderAPI::openGL:        
            openGLRendering::resize(id, width, height);
            break;

        case surfaceRenderAPI::cpu:        
            cpuRendering::resize(id, width, height);
            break;
    
        default:
            LOG_FATAL("need to set with render api")
            break;
    }
}

void surface::setWindowHeight(surfaceId id, int height)
{
    surfaceData *temp = surfacesInfo->getComponent(id);

    if(temp)
        temp->height = height;
    
}


int surface::getWindowHeight(surfaceId id)
{
    surfaceData *temp = surfacesInfo->getComponent(id);

    if(temp)
        return temp->height;
        
    return -1;
}

void surface::setWindowWidth(surfaceId id, int width)
{
    surfaceData *temp = surfacesInfo->getComponent(id);

    if(temp)
        temp->width = width;
}


int surface::getWindowWidth(surfaceId id)
{
    surfaceData *temp = surfacesInfo->getComponent(id);

    if(temp)
        return temp->width;
        
    return -1;
}

#endif