#pragma once
#include <string>

#include "Types.h"

#ifndef STRING_ID_H
#define STRING_ID_H

namespace ECSEngine
{
    typedef U32 StringID;

    extern StringID InternString(const char* str);
}

#endif

