#version 450 core

in vec2 a_pos;
in vec2 a_uv;
uniform mat4 u_mvp;

out VSOut {
    vec2 uv;
} vs_out;

void main() {
    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);
    vs_out.uv = a_uv;
}
