#include "game.h"

namespace pong{

    bool game::init_resources() {
        float vertices[] = {
                0.5f,  0.5f, 0.0f,  // top right
                0.5f, -0.5f, 0.0f,  // bottom right
                -0.5f, -0.5f, 0.0f,  // bottom left
                -0.5f,  0.5f, 0.0f   // top left
        };

        unsigned int indices[] = {  // note that we start from 0!
                0, 1, 3,   // first triangle
                1, 2, 3    // second triangle
        };

        // generate buffers
        unsigned int vertex_buffer;
        glGenBuffers(1, &vertex_buffer);

        unsigned int element_buffer;
        glGenBuffers(1, &element_buffer);

        shader = std::make_shared<gfx::shader>("media/vs.txt", "media/fs.txt");

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

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // tell shader how to interpret vertex data
        // index = 0 corresponds to vertex shader input with location 0
        // size = 3 -> 3 floats are one vertex (x,y,z)
        // type = GL_FLOAT
        // we don't want OpenGL to normalize our values (they are already normalized) to the -1...1 normalized space
        // stride = 3 * sizeof(float) - how much bytes between two vertex data
        // nullptr == 0 -> offset in the buffer where the data begins
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        // unbind VAO
        glBindVertexArray(0);

        // we can now delete buffer since it's already referenced by VAO
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteBuffers(1, &element_buffer);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        int nr_attributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr_attributes);
        log->info("Maximum number of vertex attributes supported: {}!", nr_attributes);

        timer = std::make_shared<utils::timer>();
        timer->start();

        return true;
    }

    void game::process_input() {
        timer->tick();
    }

    void game::render() {
        glClear(GL_COLOR_BUFFER_BIT);

        // set uniform
        const auto time = timer->get_time();
        const auto green_value = (sin(time) / 2.0f) + 0.5f;

        shader->use();
        shader->set_float("color", (float) green_value);

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

        // call base cleanup method
        window::cleanup();
    }
}