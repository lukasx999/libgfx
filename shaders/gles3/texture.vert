#version 300 es
precision highp float;

in vec2 a_pos;
in vec2 a_uv;
uniform mat4 u_mvp;

out vec2 uv;

void main() {
    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
    uv = a_uv;
}
