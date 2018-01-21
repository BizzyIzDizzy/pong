#include "../utils/file.h"
#include "../utils/log.h"
#include "shader.h"
#include <glad/glad.h>

namespace gfx{

    //#region helper methods

    bool check_shader_success(uint32_t shader_id)
    {
        int success;
        char info_log[512];

        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            auto log = utils::get_default_logger();
            glGetShaderInfoLog(shader_id, 512, nullptr, info_log);
            log->error("Error compiling shader! {}", info_log);

            return false;
        }

        return true;
    }

    bool check_program_success(uint32_t program_id)
    {
        int success;
        char info_log[512];

        glGetProgramiv(program_id, GL_LINK_STATUS, &success);

        if (!success)
        {
            auto log = utils::get_default_logger();

            glGetProgramInfoLog(program_id, 512, nullptr, info_log);
            log->error("Error linking program! {}", info_log);

            return false;
        }

        return true;
    }

    std::optional<uint32_t> compile_shader(GLuint type, const std::string& file_name)
    {
        auto log = utils::get_default_logger();
        if (auto shader_source = utils::read_text_file(file_name))
        {
            auto source = shader_source->c_str();
            const auto id = glCreateShader(type);
            glShaderSource(id, 1, &source, nullptr);
            glCompileShader(id);

            if (!check_shader_success(id))
            {
                log->error("Error compiling shader from file '{}'", file_name);
                return std::nullopt;
            }

            return id;
        }

        log->error("Error loading shader from file '{}'!", file_name);
        return std::nullopt;
    }

//#endregion

    shader::shader(const std::string &vertex_shader_file_path, const std::string &fragment_shader_file_path) {
        if (auto vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_file_path))
        {
            if (auto fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_file_path))
            {
                id = glCreateProgram();
                glAttachShader(*id, *vertex_shader);
                glAttachShader(*id, *fragment_shader);
                glLinkProgram(*id);

                if (!check_program_success(*id))
                {
                    auto log = utils::get_default_logger();
                    log->error("Error linking shader program!");
                }
                else
                {
                    linked = true;
                }

                glDeleteShader(*vertex_shader);
                glDeleteShader(*fragment_shader);
            }
        }
    }

    void shader::use() const {
        if (linked)
        {
            glUseProgram(*id);
        }
        else
        {
            auto log = utils::get_default_logger();
            log->error("Shader not linked!");
        }
    }

    void shader::dispose() {
        if(linked)
        {
            glDeleteProgram(*id);
            linked = false;
            id = 0;
        }else
        {
            auto log = utils::get_default_logger();
            log->error("Could not delete unlinked shader!");
        }
    }

    void shader::set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(*id, name.c_str()), static_cast<int>(value));
    }

    void shader::set_int(const std::string &name, int32_t value) const {
        glUniform1i(glGetUniformLocation(*id, name.c_str()), value);
    }

    void shader::set_float(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(*id, name.c_str()), value);
    }

}