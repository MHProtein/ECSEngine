#include "Math.h"

void ECSEngine::Math::Rotate(glm::vec2& position, float angle)
{
    float cosA = cos(angle);
    float sinA = sin(angle);

    position.x = position.x * cosA - position.y * sinA;
    position.y = position.x * sinA + position.y * cosA;
}
