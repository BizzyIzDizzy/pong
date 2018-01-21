#include "texture.h"
#include "../utils/log.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace gfx{
    texture::texture(const std::string &image_path, uint32_t format) {
        auto log = utils::get_default_logger();

        uint32_t _id;
        glGenTextures(1, &_id);
        glBindTexture(GL_TEXTURE_2D, _id);

        int32_t width, height, number_of_channels;
        stbi_set_flip_vertically_on_load(true);
        auto data = stbi_load(image_path.c_str(), &width, &height, &number_of_channels, 0);
        if(data){
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            id = _id;
            loaded = true;
        }else{
            log->error("Failed to load texture from file '{}'!", image_path);
            glDeleteTextures(1, &_id);
        }
        stbi_image_free(data);
    }

    void texture::use() const {
        if(loaded){
            glBindTexture(GL_TEXTURE_2D, *id);
        }else{
            auto log = utils::get_default_logger();
            log->error("Texture can't be used because it's not loaded yet!");
        }
    }

    void texture::dispose() {
        if(loaded){
            glDeleteTextures(1, &(*id));
        }else{
            auto log = utils::get_default_logger();
            log->error("Texture can't be disposed because it's not loaded yet!");
        }
    }
}