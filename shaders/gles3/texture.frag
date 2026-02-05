#version 300 es
precision highp float;

out vec4 FragColor;
uniform sampler2D tex;

in vec2 uv;

void main() {
    vec4 color = texture(tex, uv);
    if (color.a == 0.0) discard;
    FragColor = color;
}
