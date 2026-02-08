#version 450 core

out vec4 FragColor;
uniform float u_radius;
uniform vec2 u_center;
uniform vec4 u_color;
uniform int u_window_height;

// circles are rendered as quads where the fragments that lie outside the circle
// are transparent
void main() {
    vec2 coord = gl_FragCoord.xy;

    // gl_FragCoord's origin is at the bottom left
    coord.y = float(u_window_height) - coord.y;

    // t > 0 for pixels outside the circle
    // t < 0 for pixels inside the circle
    float t = distance(coord, u_center) - u_radius;

    // used to get a resolution-independent delta value for smoothstep().
    // fwidth(t) calculates how much t changes when going one pixel to the right, or one pixel up
    // https://rubendv.be/posts/fwidth/
    float delta = fwidth(t);

    // smooth transition between 0 and 1
    float u = smoothstep(-delta, 0.0f, t);

    FragColor = mix(u_color, vec4(0.0f), u);
}
