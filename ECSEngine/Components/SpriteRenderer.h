#pragma once
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H
#include <glm/vec2.hpp>

#include "../Managers/TagsLayersManager.h"
#include "../Core/Sprite.h"
#include "../ECS/ECS.h"

class Sprite;

namespace ECSEngine
{
    struct SpriteRenderer : public Component<SpriteRenderer>
    {
        glm::vec2 size;
        Rect rect;
        SortingLayerMask layer;
        std::shared_ptr<Sprite> sprite;
        SDL_Rect srcRect;

        SpriteRenderer(std::shared_ptr<Sprite> sprite, const SortingLayerMask& mask = {0})
            : size(glm::vec2(sprite->surface->w, sprite->surface->h)),
            rect(glm::vec2(0.0f, 0.0f), glm::vec2(sprite->surface->w, sprite->surface->h)),
            layer(mask),
            sprite(std::move(sprite))
        {
            srcRect = SDL_Rect{
                0, 0, 0, 0
            };
        }

        SpriteRenderer(size_t width, size_t height, std::shared_ptr<Sprite> sprite, const SortingLayerMask& mask = {0})
            : size(glm::vec2(width, height)),
            rect(glm::vec2(0.0f, 0.0f), glm::vec2(sprite->surface->w, sprite->surface->h)),
            layer(mask),
            sprite(std::move(sprite))
        {
            srcRect = SDL_Rect{
                0, 0, 0, 0
            };
        }

        SpriteRenderer(const glm::vec2& size, std::shared_ptr<Sprite> sprite, const SortingLayerMask& mask = {0}
            ) : size(size),
                rect(glm::vec2(0.0f, 0.0f), glm::vec2(sprite->surface->w, sprite->surface->h)),
                layer(mask),
                sprite(std::move(sprite))
        {
            srcRect = SDL_Rect{
                0, 0, 0, 0
            };
        }
    };
}

#endif
