#pragma once

namespace luly::core
{
    template <typename T>
    using unique = std::unique_ptr<T>;

    template <typename T>
    using ref = std::shared_ptr<T>;

    template <typename T, typename... Args>
    ref<T> make_ref(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}
