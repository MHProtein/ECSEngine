#include "StringID.h"

#include <unordered_map>

static std::pmr::unordered_map<ECSEngine::StringID, const char*> gStringTable;

ECSEngine::StringID ECSEngine::InternString(const char* str)
{
    
}
