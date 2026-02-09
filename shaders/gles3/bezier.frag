#version 300 es
precision highp float;

out vec4 FragColor;
uniform vec4 u_color;
uniform vec2 u_p1;
uniform vec2 u_p2;
uniform vec2 u_control;
uniform int u_window_height;

void main() {

    vec2 coord = gl_FragCoord.xy;

    // gl_FragCoord's origin is at the bottom left
    coord.y = float(u_window_height) - coord.y;

    for (float t = 0.0f; t < 1.0f; t += 0.001f) {
        vec2 a = mix(u_p1, u_control, t);
        vec2 b = mix(u_control, u_p2, t);
        vec2 p = mix(a, b, t);

        if (ivec2(coord) == ivec2(p)) {
            FragColor = u_color;
            return;
        }
    }

    discard;
}
