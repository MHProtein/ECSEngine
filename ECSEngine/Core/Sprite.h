#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <string>
#include <glm/vec2.hpp>

namespace ECSEngine
{
    struct Rect
    {
        Rect(float posX, float posY, float width, float height)
            : center(posX, posY),
              extent(width, height)
        {
        
        }
        
        Rect(const glm::vec2& center, const glm::vec2& extent)
            : center(center),
              extent(extent)
        {
        
        }

        glm::vec2 center;
        glm::vec2 extent;
    };
    
    struct Sprite
    {
        Sprite(const std::string& path, SDL_Surface* surface, SDL_Texture* texture) : path(path), surface(surface), texture(texture)
        {
        
        }
    
        std::string path;
        SDL_Surface* surface;
        SDL_Texture* texture;
    };
}


#endif