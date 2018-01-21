#pragma once

#include <optional>
#include "../utils/common.h"
#include <glad/glad.h>

namespace gfx{
    class texture{
    public:
        std::optional<uint32_t> id;
        bool loaded = false;

        explicit texture(const std::string& image_path, uint32_t format = GL_RGB);

        void use() const;
        void dispose();
    };
}