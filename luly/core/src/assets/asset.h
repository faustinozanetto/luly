#pragma once

#include "utils/uuid.h"

#include <memory>

namespace luly::assets
{
    enum class asset_type
    {
        shader,
        texture,
        model
    };

    struct asset_metadata
    {
        asset_type type;
        std::string name;
        utils::uuid uuid;

        asset_metadata(const asset_type type, std::string name) : type(type), name(std::move(name)), uuid(utils::uuid())
        {
        }
    };

    class asset
    {
    public:
        asset(asset_metadata metadata) : m_metadata(metadata)
        {
        }

        virtual ~asset() = default;

        const asset_metadata& get_metadata() const { return m_metadata; }

        template <typename T>
        void set_data(std::shared_ptr<T> reference)
        {
            m_data = std::static_pointer_cast<void>(reference);
        }

        template <typename T>
        std::shared_ptr<T> get_data() const
        {
            return std::static_pointer_cast<T>(m_data);
        }

    protected:
        asset_metadata m_metadata;
        std::shared_ptr<void> m_data;
    };
}
