#version 330 core

out vec4 FragColor;
uniform sampler2D tex;

in VSOut {
    vec2 uv;
} vs_out;

void main() {
    vec4 color = texture(tex, vs_out.uv);
    if (color.a == 0.0) discard;
    FragColor = color;
}
