#pragma once
#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <glm/vec2.hpp>
namespace ECSEngine
{
    struct RigidBody
    {
        glm::vec3 velocity;

        RigidBody(const glm::vec3& velocity = glm::vec3(0.0f)) : velocity(velocity) { }
    };
}

#endif