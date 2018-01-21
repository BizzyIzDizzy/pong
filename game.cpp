#include "game.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace pong{

    bool game::init_resources() {
        shader = std::make_shared<gfx::shader>("media/vs.txt", "media/fs.txt");
        float vertices[] = {
                // positions          // colors           // texture coords
                0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
                0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
        };
        unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
        };

        // generate buffers
        uint32_t vertex_buffer;
        glGenBuffers(1, &vertex_buffer);
        uint32_t index_buffer;
        glGenBuffers(1, &index_buffer);
        // generate vertex array object
        // VAO can store:
        // - Calls to glEnableVertexAttribArray or glDisableVertexAttribArray
        // - Vertex attribute configurations via glVertexAttribPointer
        // - Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer
        glGenVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        // fill buffer with vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // tell shader how to interpret vertex data
        // index = 0 corresponds to vertex shader input with location 0
        // size = 3 -> 3 floats are one vertex (x,y,z)
        // type = GL_FLOAT
        // we don't want OpenGL to normalize our values (they are already normalized) to the -1...1 normalized space
        // stride = 3 * sizeof(float) - how much bytes between two vertex data
        // nullptr == 0 -> offset in the buffer where the data begins

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        // generate texture
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int32_t width, height, number_of_channels;
        auto data = stbi_load("media/container.jpg", &width, &height, &number_of_channels, 0);
        if(data){
            log->info("Loaded texture from file!");
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }else{
            log->error("Failed to load texture from file '{}'!", "media/container.jpg");
            return false;
        }
        stbi_image_free(data);

        // unbind VAO
        glBindVertexArray(0);

        // we can now delete buffer since it's already referenced by VAO
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &index_buffer);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        timer = std::make_shared<utils::timer>();
        timer->start();

        return true;
    }

    void game::process_input() {
        timer->tick();
    }

    void game::render() {
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        shader->use();

        glBindVertexArray(vertex_array_object);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glUseProgram(0);
        glBindVertexArray(0);    }

    void game::cleanup() {
        // delete VAO
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &vertex_array_object);

        // delete shader program
        glUseProgram(0);
        shader->dispose();

        // delete texture
        glDeleteTextures(1, &texture);

        // call base cleanup method
        window::cleanup();
    }
}