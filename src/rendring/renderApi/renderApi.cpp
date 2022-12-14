#include "renderApi.hpp"
#include "openGLRenderer.hpp"
#include "textureManger.hpp"
#include "vertexArrayManger.hpp"
#include "shaderManger.hpp"
#include "frameBuffersManger.hpp"


renderApi::renderApi()
{
    framebuffersPool = new entityPool(255);
    texturesPool = new entityPool(1000);
    vaosPool = new entityPool(10000);
    shadersPool = new entityPool(1000);
    uniformBuffersPool = new entityPool(1000);


    framebuffersApiType = new apiTypeComponents(framebuffersPool);
    texturesApiType = new apiTypeComponents(texturesPool);
    vaosApiType = new apiTypeComponents(vaosPool);
    shadersApiType = new apiTypeComponents(shadersPool);
    uniformBuffersApiType = new apiTypeComponents(uniformBuffersPool);


    openGLRenderEngine::framebuffers::init(framebuffersPool);
    openGLRenderEngine::textures::init(texturesPool);
    openGLRenderEngine::vaos::init(vaosPool);
    openGLRenderEngine::shaders::init(shadersPool);
    openGLRenderEngine::uniformBuffers::init(uniformBuffersPool);
}

renderApi::~renderApi()
{
    openGLRenderEngine::framebuffers::close();
    openGLRenderEngine::textures::close();
    openGLRenderEngine::vaos::close();
    openGLRenderEngine::shaders::close();
    openGLRenderEngine::uniformBuffers::close();


    delete framebuffersApiType;
    delete texturesApiType;
    delete vaosApiType;
    delete shadersApiType;
    delete uniformBuffersApiType;


    delete framebuffersPool;
    delete texturesPool;
    delete vaosPool;
    delete shadersPool;
    delete uniformBuffersPool;
}


framebufferId renderApi::allocFramebuffer(supportedRenderApis apiType)
{
    framebufferId temp = framebuffersPool->allocEntity();
    framebuffersApiType->setComponent(temp, apiType);
    return temp;
}

textureId renderApi::allocTexture(supportedRenderApis apiType)
{
    textureId temp = texturesPool->allocEntity();
    texturesApiType->setComponent(temp, apiType);
    return temp;
}

vaoId renderApi::allocVao(supportedRenderApis apiType)
{
    vaoId temp = vaosPool->allocEntity();
    vaosApiType->setComponent(temp, apiType);
    return temp;
}

shaderId renderApi::allocShader(supportedRenderApis apiType)
{
    shaderId temp = shadersPool->allocEntity();
    shadersApiType->setComponent(temp, apiType);
    return temp;
}

uniformBufferId renderApi::allocUniformBuffer(supportedRenderApis apiType)
{
    uniformBufferId temp = uniformBuffersPool->allocEntity();
    uniformBuffersApiType->setComponent(temp, apiType);
    return temp;
}

void renderApi::deallocFramebuffer(framebufferId id)
{
    framebuffersPool->freeEntity(id);
}

void renderApi::deallocTexture(textureId id)
{
    texturesPool->freeEntity(id);
}

void renderApi::deallocVao(vaoId id)
{
    vaosPool->freeEntity(id);
}

void renderApi::deallocShader(shaderId id)
{
    shadersPool->freeEntity(id);
}


void renderApi::deallocUniformBuffer(uniformBufferId id)
{
    uniformBuffersPool->freeEntity(id);
}

void renderApi::setFramebuffer(frameBufferInfo data)
{
    switch (framebuffersApiType->getComponent(data.id))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::framebuffers::setFrameBufferData(data);
            break;
        
        default:
            break;
    }
}

void renderApi::setTexture(textureInfo data)
{
    switch (texturesApiType->getComponent(data.id))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::textures::setTextureData(data);
            break;
        
        default:
            break;
    }
}

void renderApi::setVao(VAOInfo data)
{
    switch (vaosApiType->getComponent(data.id))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::vaos::setVAOData(data);
            break;
        
        default:
            break;
    }
}

void renderApi::setShader(shaderInfo data)
{
    switch (shadersApiType->getComponent(data.id))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::shaders::setShadersData(data);
            break;
        
        default:
            break;
    }
}

void renderApi::setUniformBuffer(uniformBufferInfo data)
{
    switch (shadersApiType->getComponent(data.id))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::uniformBuffers::setUniformBufferData(data);
            break;
        
        default:
            break;
    }
}



frameBufferInfo *renderApi::getFramebuffer(framebufferId id)
{
    switch (shadersApiType->getComponent(id))
    {
        case supportedRenderApis::openGl:
                return openGLRenderEngine::framebuffers::getFrameBuffer(id);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }
    return nullptr;
}

textureInfo *renderApi::getTexture(textureId id)
{
    switch (shadersApiType->getComponent(id))
    {
        case supportedRenderApis::openGl:
                return openGLRenderEngine::textures::getTexture(id);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }    

    return nullptr;
}

VAOInfo *renderApi::getVao(vaoId id)
{
    switch (shadersApiType->getComponent(id))
    {
        case supportedRenderApis::openGl:
                return openGLRenderEngine::vaos::getVAO(id);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }    

    return nullptr;
}

shaderInfo *renderApi::getShader(shaderId id)
{
    switch (shadersApiType->getComponent(id))
    {
        case supportedRenderApis::openGl:
                return openGLRenderEngine::shaders::getShaders(id);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }    

    return nullptr;
}


uniformBufferInfo *renderApi::getUniformBuffer(uniformBufferId id)
{
    switch (shadersApiType->getComponent(id))
    {
        case supportedRenderApis::openGl:
                return openGLRenderEngine::uniformBuffers::getUniformBuffer(id);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }    

    return nullptr;
}


void renderApi::renderRequest(const renderRequestInfo& data)
{
    switch (framebuffersApiType->getComponent(data.frameBufferId))
    {
        case supportedRenderApis::openGl:
                openGLRenderEngine::openGLRenderer::renderRequest(data);
            break;
        default:
            LOG_FATAL("id is not valid");
        
    }
}


void defaultDeleteBuffer(void *buffer)
{
    free(buffer);
}