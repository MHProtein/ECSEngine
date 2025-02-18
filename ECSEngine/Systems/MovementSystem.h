#pragma once
#ifndef MOVEMENT_SYSTEM_H
#define MOVEMENT_SYSTEM_H
#include "../ECS/ECS.h"

namespace ECSEngine
{
    class Movement : public System
    {
    public:
        Movement();

        void Update() override;
    };  
}
#endif

