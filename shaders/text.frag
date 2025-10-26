#version 330 core

out vec4 FragColor;
uniform sampler2D tex;
uniform vec4 u_color;

in VSOut {
    vec2 uv;
} vs_out;

void main() {
    if (texture(tex, vs_out.uv).a == 0.0) discard;
    FragColor = u_color;
}
