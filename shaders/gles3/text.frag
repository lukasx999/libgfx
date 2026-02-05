#version 300 es
precision highp float;

out vec4 FragColor;
uniform sampler2D tex;
uniform vec4 u_color;

in vec2 uv;

void main() {
    if (texture(tex, uv).r == 0.0) discard;
    FragColor = u_color;
}
