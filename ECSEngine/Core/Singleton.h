#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

namespace ECSEngine
{
    template<class T>
    class Singleton
    {
    public:
        Singleton() = default;
        static std::unique_ptr<T>& Instance();
    };

    template <class T>
    std::unique_ptr<T>& Singleton<T>::Instance()
    {
        static std::unique_ptr<T> instance = std::make_unique<T>();
        return instance;
    } 
}
#endif


