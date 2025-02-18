#pragma once
#include <glm/vec2.hpp>

namespace ECSEngine
{
    class Math
    {
    public:
        static void Rotate(glm::vec2& position, float angle);
    };
}

