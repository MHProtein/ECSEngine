#pragma once
#ifndef ANIMATION_H
#define ANIMATION_H


#include <memory>
#include <utility>
#include <vector>

#include "../ECS/ECS.h"

namespace ECSEngine
{
    struct Animation : public Component<Animation>
    {
        bool loop;
        int intervalFrames;
        size_t currentFrame;
        size_t currentRect;
        std::shared_ptr<struct Sprite> sprite;
        std::pmr::vector<struct Rect> rects;

        Animation(std::shared_ptr<Sprite> sprite, bool loop, int intervalFrames)
            : loop(loop), intervalFrames(intervalFrames), currentFrame(0), currentRect(0), sprite(std::move(sprite))
        {
        }
    };
}



#endif