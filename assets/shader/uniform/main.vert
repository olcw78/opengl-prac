#version 410 core
// in
layout (location = 0) in vec3 in_pos;

void main() {
    gl_Position = vec4(in_pos.xyz, 1.0);
}
