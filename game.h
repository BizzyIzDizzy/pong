#pragma once

#include <glad/glad.h>

#include "window.h"
#include "utils/timer.h"
#include "gfx/shader.h"
#include "utils/common.h"

namespace pong{
    class game: public window{
    private:
        uint32_t vertex_array_object;
        uint32_t texture;
        std::shared_ptr<utils::timer> timer;
        std::shared_ptr<gfx::shader> shader;

    protected:
        bool init_resources() override;
        void process_input() override;
        void render() override;
        void cleanup() override;
    };
}