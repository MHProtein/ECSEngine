#include "TagsLayersManager.h"

ECSEngine::SortingLayerMask::SortingLayerMask(int layer) : LayerMask(layer)
{
}

ECSEngine::LayerMask::LayerMask(int layer) : mask(layer)
{
    
}

ECSEngine::LayerMask& ECSEngine::TagsLayersManager::AddLayer(int layer, const std::string& layerName)
{
    layers[layer].first = {layer};
    layers[layer].second = layerName;

    return layers[layer].first;
}

ECSEngine::SortingLayerMask& ECSEngine::TagsLayersManager::AddSortingLayer(const std::string& layerName)
{
    sortingLayers[sortingLayerCount].first = {sortingLayerCount};
    sortingLayers[sortingLayerCount].second = layerName;

    return sortingLayers[sortingLayerCount++].first;
}

ECSEngine::LayerMask ECSEngine::TagsLayersManager::GetLayerMaskFromName(const std::string& name) const
{
    auto it = std::find_if(layers.cbegin(), layers.cend(),
        [&name](const LayerEntry& entry){return entry.second == name;});
    if(it != layers.cend())
        return it->first;
    return {-1};
}

ECSEngine::SortingLayerMask ECSEngine::TagsLayersManager::GetSortingLayerMaskFromName(const std::string& name) const
{
    auto it = std::find_if(sortingLayers.cbegin(), sortingLayers.cend(),
        [&name](const SortingLayerEntry& entry){return entry.second == name;});
    if(it != sortingLayers.cend())
        return it->first;
    return {-1};
}
