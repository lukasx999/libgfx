#version 330 core

in vec2 a_pos;
in vec4 a_color;
in mat4 a_mvp;
in vec2 a_center;
in float a_radius;

out VSOut {
    vec4 color;
    vec2 pos;
    vec2 center;
    float radius;
} vs_out;

void main() {
    gl_Position = a_mvp * vec4(a_pos, 0.0, 1.0);
    vs_out.color = a_color;
    vs_out.pos = a_pos;
    vs_out.radius = a_radius;
    vs_out.center = a_center;
}
