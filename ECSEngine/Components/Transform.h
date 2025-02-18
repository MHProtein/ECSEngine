#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../ECS/ECS.h"

namespace ECSEngine
{

    struct GlobalTransform : public Component<GlobalTransform>
    {
        glm::mat4 matrix;

        glm::vec3 GetPosition();
        glm::vec3 GetRotation();
        glm::vec3 GetScale();
    };

    inline glm::vec3 GlobalTransform::GetPosition()
    {
        return { matrix[3] };
    }

    inline glm::vec3 GlobalTransform::GetRotation()
    {
        float y;
        float cosPitch = std::sqrt(std::pow(matrix[0][0], 2.0f) + std::pow(matrix[0][1], 2.0f));
        y = (cosPitch > 0.0001f ? std::atan2(-matrix[0][2], cosPitch) : 0.0f);

        return {
            std::atan2(matrix[1][2], matrix[2][2]),
            y,
            std::atan2(matrix[0][1], matrix[0][0])
        };
    }

    inline glm::vec3 GlobalTransform::GetScale()
    {
        return {
            glm::length(glm::vec3(matrix[0][0], matrix[1][0], matrix[2][0])), 
            glm::length(glm::vec3(matrix[0][1], matrix[1][1], matrix[2][1])), 
            glm::length(glm::vec3(matrix[0][2], matrix[1][2], matrix[2][2])), 
        };
    }

    struct LocalTransform : public Component<LocalTransform>
    {
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;

        LocalTransform() :
            position({0.0f, 0.0f, 0.0f}), rotation({0.0f, 0.0f, 0.0f}), scale({1.0f, 1.0f, 1.0f}) { }
        LocalTransform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
            position(position), rotation(rotation), scale(scale) { }

        glm::mat4 GetMatrix() const;
        
    };
    
    inline glm::mat4 LocalTransform::GetMatrix() const
    {
        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(  rotation)));

        return glm::translate(glm::mat4(1.0f), position) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
    }

    struct Parent : Component<Parent>
    {
        Entity entity;

        Parent(const Entity& entity) : entity(entity){ }
        
    };
    
    struct FormerParent : Component<Parent>
    {
        Entity entity;
        
        FormerParent(const Entity& entity) : entity(entity){ }
    };

    struct Children : Component<Parent>
    {
        std::pmr::vector<Entity> children;
    };
    
}



#endif