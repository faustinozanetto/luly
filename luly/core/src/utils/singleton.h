﻿#pragma once

#include <logging/log.h>
#include <utils/assert.h>

namespace luly
{
    template <class T>
    class singleton
    {
    public:
        static T& get()
        {
            if (!s_instance)
                s_instance = new T();
            return *s_instance;
        }

        template <typename... TArgs>
        static void initialize(TArgs... args)
        {
            LY_ASSERT_MSG(s_instance == nullptr, "Singleton already initialized!");
            s_instance = new T(std::forward<TArgs>(args)...);
        }

        static void release()
        {
            if (s_instance)
            {
                delete s_instance;
                s_instance = nullptr;
            }
        }

    protected:
        singleton()
        = default;

        virtual ~singleton() = default;

        static T* s_instance;
    };

    template <class T>
    T* singleton<T>::s_instance = nullptr;
}
