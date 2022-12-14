//
// Created by 이윤상 on 2022/10/06.
//

#include "fmt/core.h"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "runner.h"

#include "opengl_status_checker/opengl_status_checker.h"
#include "stb/stb_image.h"
#include "camera/camera.h"
#include "time/time.h"
#include "texture/texture.h"

namespace highp {
    constexpr static const float vertices[]{
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    Runner::Runner(int width, int height, const char *title,
                   std::string_view cube_vertex_shader_path,
                   std::string_view cube_fragment_shader_path,
                   std::string_view light_vertex_shader_path,
                   std::string_view light_fragment_shader_path,
                   std::string_view wooden_box_diffuse_tex_path,
                   std::string_view wooden_box_specular_tex_path)
            : _width(width),
              _height(height),
              _title(title),
              _window{nullptr},
              _cube_shader{std::make_unique<shared::shader>()},
              _light_shader{std::make_unique<shared::shader>()},
              _cube_vertex_shader_path{cube_vertex_shader_path},
              _cube_fragment_shader_path{cube_fragment_shader_path},
              _light_vertex_shader_path{light_vertex_shader_path},
              _light_fragment_shader_path{light_fragment_shader_path},
              _wooden_box_diffuse_tex_path{wooden_box_diffuse_tex_path},
              _wooden_box_specular_tex_path{wooden_box_specular_tex_path} {
        //
    }

    Runner::~Runner() {
        glfwTerminate();
    }

    int Runner::init(bool enable_wireframe) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        _window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
        if (_window == nullptr) {
            fmt::print("[error] failed to create GLFW window\n");
            glfwTerminate();

            return -1;
        }

        glfwMakeContextCurrent(_window);

        if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
            fmt::print("[error] failed to init GLAD\n");

            return -1;
        }

        glViewport(0, 0, _width, _height);

        glfwSetFramebufferSizeCallback(_window, Runner::on_resize_frame_buffer);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetCursorPosCallback(_window, Runner::on_receive_mouse_event_impl);

        glfwSetScrollCallback(_window, Runner::on_receive_scroll_event);

        opengl_status_checker::check_max_shader_attributes();

        _cube_shader->compile_and_link({
                                               _cube_shader->compile_vertex_shader(_cube_vertex_shader_path.data()),
                                               _cube_shader->compile_fragment_shader(_cube_fragment_shader_path.data())
                                       });

        _light_shader->compile_and_link({
                                                _light_shader->compile_vertex_shader(_light_vertex_shader_path.data()),
                                                _light_shader->compile_fragment_shader(
                                                        _light_fragment_shader_path.data())
                                        });

        // create and bind VBO.

        unsigned vbo;
        glGenBuffers(1, &vbo);

        // create and bind VAO.
        unsigned cube_vao;
        glGenVertexArrays(1, &cube_vao);

        unsigned light_vao;
        glGenVertexArrays(1, &light_vao);

#pragma region cube vao

        // bind the VAO first, then bind and set vertex buffer(s),
        // then configure vertex buffer(s).
        glBindVertexArray(cube_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo bind.

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // linking vertex attributes.
        const size_t cube_vertex_attrib_size = 8 * sizeof(float);

        const size_t start_offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset));
        glEnableVertexAttribArray(0);

        const size_t offset_1 = 3 * sizeof(float);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset + offset_1));
        glEnableVertexAttribArray(1);

        const size_t offset_2 = 3 * sizeof(float);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size,
                              reinterpret_cast<void *> (start_offset + offset_1 + offset_2));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.

#pragma endregion cube vao

#pragma region light vao

        glBindVertexArray(light_vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo); // vbo bind.

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cube_vertex_attrib_size, (void *) nullptr);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0); // vbo unbind.

#pragma endregion light vao

        glBindVertexArray(0);

#pragma region texture

        shared::texture::set_flip_vertically(true);


        // wooden box diffuse
        const unsigned wooden_box_diffuse_tex = shared::texture::load_texture_2d(
                _wooden_box_diffuse_tex_path.data(),
                {
                        std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
                        std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT)
                },
                {
                        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
                },
                true
        );

        // wooden box specular
        const unsigned wooden_box_specular_tex = shared::texture::load_texture_2d(
                _wooden_box_specular_tex_path.data(),
                {
                        std::make_pair(GL_TEXTURE_WRAP_S, GL_REPEAT),
                        std::make_pair(GL_TEXTURE_WRAP_T, GL_REPEAT)
                },
                {
                        std::make_pair(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR),
                        std::make_pair(GL_TEXTURE_MAG_FILTER, GL_LINEAR)
                },
                true
        );


#pragma endregion texture

        const glm::vec3 coral_color{1.0f, 0.5f, 0.31f};
        const glm::vec3 light_color{1, 1, 1};

        glm::vec3 light_pos(1.2f, 5.0f, 2.0f);

        _cube_shader->use();
        _cube_shader->set_int("material.diffuse", 0);
        _cube_shader->set_int("material.specular", 1);

        _cube_shader->set_float("material.shininess", 64.0f);

        _cube_shader->set_vec3("light.ambient", {0.2f, 0.2f, 0.2f});
        _cube_shader->set_vec3("light.diffuse", {0.5f, 0.5f, 0.5f});
        _cube_shader->set_vec3("light.specular", {1.0f, 1.0f, 1.0f});

        while (!glfwWindowShouldClose(_window)) {

            // calculate delta time.
            shared::time::tick(glfwGetTime);

            process_input(_window);

            glClearColor(0, 0, 0, 0);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            if (enable_wireframe)
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            const glm::mat4 proj = glm::perspective(glm::radians(shared::camera::get_fov()),
                                                    1024.0f / 768.0f,
                                                    0.1f,
                                                    100.0f);

            const auto elapsed_time = static_cast<float>(glfwGetTime());
            light_pos = {
                    -2.0f * sin(elapsed_time),
                    0,
                    -1.5f * cos(elapsed_time)
            };

            // set cube shader params
            _cube_shader->use();

            _cube_shader->set_vec3("view_pos", shared::camera::get_camera_pos());
            _cube_shader->set_vec3("light.pos", light_pos);

            glm::mat4 cube_model{1.0f};
            _cube_shader->set_mat4("model", cube_model);
            _cube_shader->set_mat4("view", shared::camera::get_view_matrix());
            _cube_shader->set_mat4("projection", proj);

            // draw cube
            glBindVertexArray(cube_vao);
            // active diffuse / specular textures.
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wooden_box_diffuse_tex);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, wooden_box_specular_tex);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            // set light shader params
            _light_shader->use();

            glm::mat4 light_model{1.0f};

            light_model = glm::translate(light_model, light_pos);
            light_model = glm::rotate(light_model, glm::radians(80.0f) * elapsed_time, glm::vec3{0, 1, 0});
            light_model = glm::scale(light_model, glm::vec3{0.2f});

            _light_shader->set_mat4("model", light_model);
            _light_shader->set_mat4("view", shared::camera::get_view_matrix());
            _light_shader->set_mat4("projection", proj);

            // draw light
            glBindVertexArray(light_vao);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glfwSwapBuffers(_window);
            glfwPollEvents(); // poll IO events.
        }

        glDeleteVertexArrays(1, &cube_vao);
        glDeleteVertexArrays(1, &light_vao);
        glDeleteBuffers(1, &vbo);

        glfwTerminate();

        return 0;
    }

    void Runner::on_resize_frame_buffer(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void Runner::process_input(GLFWwindow *window) {
        // close a window.
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        shared::camera::on_update(window);
    }

    void Runner::on_receive_mouse_event_impl(GLFWwindow *window, double x, double y) {
        shared::camera::on_update_mouse(window, x, y);
    }

    void Runner::on_receive_scroll_event(GLFWwindow *window, double x, double y) {
        shared::camera::on_update_scroll(window, x, y);
    }
}
