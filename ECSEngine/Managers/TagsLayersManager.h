#pragma once
#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H
#include <unordered_map>
#include <vector>
#include <cvt/wstring>

#include "../Core/Singleton.h"

namespace ECSEngine
{
    constexpr size_t MAX_LAYER_COUNT = 32;
    constexpr size_t MAX_SORTING_LAYER_COUNT = 32;
    constexpr size_t MAX_TAG_COUNT = 512;

    struct LayerMask
    {
        int mask;

        LayerMask() = default;
        LayerMask(int layer);
    };
    
    struct SortingLayerMask : LayerMask
    {
        
        SortingLayerMask() = default;
        SortingLayerMask(int layer);
    };

    class TagsLayersManager : public Singleton<TagsLayersManager>
    {
    public:
        typedef std::pair<SortingLayerMask, std::string> SortingLayerEntry;
        typedef std::pair<LayerMask, std::string> LayerEntry;
        TagsLayersManager(): sortingLayerCount(0)
        {
            sortingLayers.resize(MAX_SORTING_LAYER_COUNT);
            layers.resize(MAX_LAYER_COUNT);
            tags.resize(MAX_TAG_COUNT);

            AddSortingLayer("DefaultLayer");
            AddLayer(0, "DefaultLayer");
        }

        LayerMask& AddLayer(int layer, const std::string& layerName);
        SortingLayerMask& AddSortingLayer(const std::string& layerName);
        
        LayerMask GetLayerMaskFromName(const std::string& name) const;
        SortingLayerMask GetSortingLayerMaskFromName(const std::string& name) const;
        
    private:
        int sortingLayerCount;
        std::pmr::vector<SortingLayerEntry> sortingLayers;
        std::pmr::vector<LayerEntry> layers;
        std::pmr::vector<std::pair<LayerMask, std::pmr::string>> tags;
    };

    inline bool operator<(const LayerMask& lhs, const LayerMask& rhs)
    {
        return lhs.mask < rhs.mask;
    }
    inline bool operator==(const LayerMask& lhs, const LayerMask& rhs)
    {
        return lhs.mask == rhs.mask;
    }
    inline bool operator!=(const LayerMask& lhs, const LayerMask& rhs)
    {
        return !(lhs == rhs);
    }
    inline LayerMask operator&(const LayerMask& lhs, const LayerMask& rhs)
    {
        return {lhs.mask & rhs.mask};
    }

    inline LayerMask operator|(const LayerMask& lhs, const LayerMask& rhs)
    {
        return {lhs.mask | rhs.mask};
    }

    inline LayerMask operator^(const LayerMask& lhs, const LayerMask& rhs)
    {
        return {lhs.mask ^ rhs.mask};
    }

    inline LayerMask operator~(const LayerMask& layer)
    {
        return {~layer.mask};
    }

    inline LayerMask operator<<(const LayerMask& lhs, int rhs)
    {
        return {lhs.mask << rhs};
    }

    inline LayerMask operator>>(const LayerMask& lhs, int rhs)
    {
        return {lhs.mask >> rhs};
    }
    
}
#endif

