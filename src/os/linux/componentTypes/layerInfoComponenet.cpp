#include "layerInfoComponenet.hpp"

layerInfoComponenet::layerInfoComponenet(entityPool *pool): 
    pool(pool)
{
    pool->enlistType((void*)this, deleteCallback, &IdToIndex);
}

void layerInfoComponenet::deleteComponent(entityId id)
{
    if(!pool->isIdValid(id) || IdToIndex[id.index] == -1)
        return;

    uint32_t index = IdToIndex[id.index];

    zwlr_layer_surface_v1_destroy(data[index].layerSurface);

    IdToIndex[id.index] = -1;
    indexToId[index] = *indexToId.end();
    data[index] = *data.end();
    data.pop_back();

}

layerSurfaceInfo *layerInfoComponenet::getComponent(entityId id)
{
    if(!pool->isIdValid(id) || IdToIndex[id.index] == -1)
        return nullptr;
    
    return &data[IdToIndex[id.index]];
}

void layerInfoComponenet::setComponent(entityId id, layerSurfaceInfo callback)
{
    if(!pool->isIdValid(id)){
        LOG_ERROR("culdn't set data, id(index: " << id.index << ", gen: " << (int)id.gen << ")")
        return;
    }

    if(IdToIndex[id.index] == -1)
    {
        IdToIndex[id.index] = data.size();
        data.push_back(callback);
        indexToId.push_back(id);

    }
    else
        data[IdToIndex[id.index]] = callback;
    
}


layerInfoComponenet::~layerInfoComponenet()
{
    pool->unenlistType(this, IdToIndex);
    
}
