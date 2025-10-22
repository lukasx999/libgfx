#version 330 core

out vec4 FragColor;
uniform int u_window_height;

in VSOut {
    vec4 color;
    vec2 pos;
    vec2 center;
    float radius;
} vs_out;

// circles are rendered as quads, where all fragments outside of the circle's radius
// are discarded
void main() {
    vec2 coord = gl_FragCoord.xy;

    // gl_FragCoord's origin is at the bottom left
    coord.y = float(u_window_height) - coord.y;

    if (distance(coord, vs_out.center) > vs_out.radius) {
        discard;
    }

    FragColor = vs_out.color;
}
