//
// Created by 이윤상 on 2022/10/06.
//

#ifndef OPENGL_RUNNER_H
#define OPENGL_RUNNER_H

#include <string>

class Runner final {
public:
    explicit Runner(int width, int height, const char *title, const char *vertex_shader_src_path_abs,
                    const char *fragment_shader_src_path_abs);

    virtual ~Runner();

    int init(bool enable_wireframe);

private:
    static void on_resize_frame_buffer(struct GLFWwindow *window, int width, int height);

    static void process_input(struct GLFWwindow *window);

public:

private:
    int _width;
    int _height;

    const char *_title;

    std::string vertex_shader_str;
    const char *vertex_shader_src;

    std::string fragment_shader_str;
    const char *fragment_shader_src;

    struct GLFWwindow *_window;
};


#endif //OPENGL_RUNNER_H
