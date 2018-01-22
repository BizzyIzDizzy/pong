#pragma once

#include <optional>
#include <detail/type_mat4x4.hpp>
#include "../utils/common.h"

namespace gfx{
    class shader{
    public:
        std::optional<uint32_t> id;
        bool linked = false;

        shader(const std::string& vertex_shader_file_path, const std::string& fragment_shader_file_path);

        void use() const;
        void dispose();

        void set_bool(const std::string& name, bool value) const;
        void set_int(const std::string& name, int32_t value) const;
        void set_float(const std::string& name, float value) const;
        void set_mat4(const std::string& name, glm::mat4 matrix) const;
    };
}