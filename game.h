#pragma once

#include <glad/glad.h>
#include <detail/type_mat4x4.hpp>

#include "window.h"
#include "utils/timer.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "utils/common.h"

namespace pong{
    class game: public window{
    private:
        uint32_t vertex_array_object;
        std::shared_ptr<utils::timer> timer;
        std::shared_ptr<gfx::shader> shader;
        std::shared_ptr<gfx::texture> texture1;
        std::shared_ptr<gfx::texture> texture2;

    protected:
        bool init_resources() override;
        void process_input() override;
        void render() override;
        void cleanup() override;
    };
}