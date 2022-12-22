#include "keyCallbackComponent.hpp"

keyCallbackComponent::keyCallbackComponent(entityPool *pool): 
    pool(pool)
{
    pool->enlistType((void*)this, deleteCallback, &IdToIndex);
}

void keyCallbackComponent::deleteComponent(entityId id)
{
    if(!pool->isIdValid(id) || IdToIndex[id.index] == -1)
        return;

    uint32_t index = IdToIndex[id.index];
    IdToIndex[id.index] = -1;
    indexToId[index] = *indexToId.end();
    data[index] = *data.end();
    data.pop_back();

}

keyCallback keyCallbackComponent::getCallback(entityId id)
{
    if(!pool->isIdValid(id) || IdToIndex[id.index] == -1)
        return nullptr;
    
    return data[IdToIndex[id.index]];
}

void keyCallbackComponent::setCallback(entityId id, keyCallback callback)
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


keyCallbackComponent::~keyCallbackComponent()
{
    pool->unenlistType(this, IdToIndex);
    free(IdToIndex);
}
