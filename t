[1mdiff --git a/shaders/es/circle.frag b/shaders/es/circle.frag[m
[1mdeleted file mode 100644[m
[1mindex ec1d506..0000000[m
[1m--- a/shaders/es/circle.frag[m
[1m+++ /dev/null[m
[36m@@ -1,23 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform float u_radius;[m
[31m-uniform vec2 u_center;[m
[31m-uniform vec4 u_color;[m
[31m-uniform int u_window_height;[m
[31m-[m
[31m-// circles are rendered as quads, where all fragments outside of the circle's radius[m
[31m-// are discarded[m
[31m-void main() {[m
[31m-    vec2 coord = gl_FragCoord.xy;[m
[31m-[m
[31m-    // gl_FragCoord's origin is at the bottom left[m
[31m-    coord.y = float(u_window_height) - coord.y;[m
[31m-[m
[31m-    if (distance(coord, u_center) > u_radius) {[m
[31m-        discard;[m
[31m-    }[m
[31m-[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/es/default.frag b/shaders/es/default.frag[m
[1mdeleted file mode 100644[m
[1mindex 5ef7ce3..0000000[m
[1m--- a/shaders/es/default.frag[m
[1m+++ /dev/null[m
[36m@@ -1,9 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform vec4 u_color;[m
[31m-[m
[31m-void main() {[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/es/default.vert b/shaders/es/default.vert[m
[1mdeleted file mode 100644[m
[1mindex f5fdae7..0000000[m
[1m--- a/shaders/es/default.vert[m
[1m+++ /dev/null[m
[36m@@ -1,9 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-in vec2 a_pos;[m
[31m-uniform mat4 u_mvp;[m
[31m-[m
[31m-void main() {[m
[31m-    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);[m
[31m-}[m
[1mdiff --git a/shaders/es/text.frag b/shaders/es/text.frag[m
[1mdeleted file mode 100644[m
[1mindex e32a02b..0000000[m
[1m--- a/shaders/es/text.frag[m
[1m+++ /dev/null[m
[36m@@ -1,13 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform sampler2D tex;[m
[31m-uniform vec4 u_color;[m
[31m-[m
[31m-in vec2 uv;[m
[31m-[m
[31m-void main() {[m
[31m-    if (texture(tex, uv).r == 0.0) discard;[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/es/texture.frag b/shaders/es/texture.frag[m
[1mdeleted file mode 100644[m
[1mindex c537858..0000000[m
[1m--- a/shaders/es/texture.frag[m
[1m+++ /dev/null[m
[36m@@ -1,13 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform sampler2D tex;[m
[31m-[m
[31m-in vec2 uv;[m
[31m-[m
[31m-void main() {[m
[31m-    vec4 color = texture(tex, uv);[m
[31m-    if (color.a == 0.0) discard;[m
[31m-    FragColor = color;[m
[31m-}[m
[1mdiff --git a/shaders/es/texture.vert b/shaders/es/texture.vert[m
[1mdeleted file mode 100644[m
[1mindex cc12233..0000000[m
[1m--- a/shaders/es/texture.vert[m
[1m+++ /dev/null[m
[36m@@ -1,13 +0,0 @@[m
[31m-#version 300 es[m
[31m-precision highp float;[m
[31m-[m
[31m-in vec2 a_pos;[m
[31m-in vec2 a_uv;[m
[31m-uniform mat4 u_mvp;[m
[31m-[m
[31m-out vec2 uv;[m
[31m-[m
[31m-void main() {[m
[31m-    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);[m
[31m-    uv = a_uv;[m
[31m-}[m
[1mdiff --git a/shaders/gen.py b/shaders/gen.py[m
[1mindex a724f80..a1df2c8 100644[m
[1m--- a/shaders/gen.py[m
[1m+++ b/shaders/gen.py[m
[36m@@ -46,11 +46,18 @@[m [mdef split_shader_types(shaders_filenames: list[str]) -> tuple[list[str], list[st[m
 [m
 def main() -> int:[m
 [m
[31m-    if len(sys.argv) != 2 or sys.argv[1] not in ["gl", "es"]:[m
[32m+[m[32m    if len(sys.argv) != 2:[m
         print("expected shader type as argument (`gl` or `es`)")[m
         return 1[m
 [m
[31m-    subdir = sys.argv[1][m
[32m+[m[32m    match sys.argv[1]:[m
[32m+[m[32m        case "gl":[m
[32m+[m[32m            subdir = "opengl4.5"[m
[32m+[m[32m        case "es":[m
[32m+[m[32m            subdir = "gles3"[m
[32m+[m[32m        case _:[m
[32m+[m[32m            print("unknown shader type (must be either `gl` or `es`)")[m
[32m+[m[32m            return 1[m
 [m
     out_file = "../src/shaders.h"[m
 [m
[1mdiff --git a/shaders/gl/circle.frag b/shaders/gl/circle.frag[m
[1mdeleted file mode 100644[m
[1mindex de7c4e2..0000000[m
[1m--- a/shaders/gl/circle.frag[m
[1m+++ /dev/null[m
[36m@@ -1,22 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform float u_radius;[m
[31m-uniform vec2 u_center;[m
[31m-uniform vec4 u_color;[m
[31m-uniform int u_window_height;[m
[31m-[m
[31m-// circles are rendered as quads, where all fragments outside of the circle's radius[m
[31m-// are discarded[m
[31m-void main() {[m
[31m-    vec2 coord = gl_FragCoord.xy;[m
[31m-[m
[31m-    // gl_FragCoord's origin is at the bottom left[m
[31m-    coord.y = float(u_window_height) - coord.y;[m
[31m-[m
[31m-    if (distance(coord, u_center) > u_radius) {[m
[31m-        discard;[m
[31m-    }[m
[31m-[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/gl/default.frag b/shaders/gl/default.frag[m
[1mdeleted file mode 100644[m
[1mindex 4311e1e..0000000[m
[1m--- a/shaders/gl/default.frag[m
[1m+++ /dev/null[m
[36m@@ -1,8 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform vec4 u_color;[m
[31m-[m
[31m-void main() {[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/gl/default.vert b/shaders/gl/default.vert[m
[1mdeleted file mode 100644[m
[1mindex 69f958b..0000000[m
[1m--- a/shaders/gl/default.vert[m
[1m+++ /dev/null[m
[36m@@ -1,8 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-in vec2 a_pos;[m
[31m-uniform mat4 u_mvp;[m
[31m-[m
[31m-void main() {[m
[31m-    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);[m
[31m-}[m
[1mdiff --git a/shaders/gl/text.frag b/shaders/gl/text.frag[m
[1mdeleted file mode 100644[m
[1mindex 79970df..0000000[m
[1m--- a/shaders/gl/text.frag[m
[1m+++ /dev/null[m
[36m@@ -1,14 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform sampler2D tex;[m
[31m-uniform vec4 u_color;[m
[31m-[m
[31m-in VSOut {[m
[31m-    vec2 uv;[m
[31m-} vs_out;[m
[31m-[m
[31m-void main() {[m
[31m-    if (texture(tex, vs_out.uv).r == 0.0) discard;[m
[31m-    FragColor = u_color;[m
[31m-}[m
[1mdiff --git a/shaders/gl/texture.frag b/shaders/gl/texture.frag[m
[1mdeleted file mode 100644[m
[1mindex e9be9fa..0000000[m
[1m--- a/shaders/gl/texture.frag[m
[1m+++ /dev/null[m
[36m@@ -1,14 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-out vec4 FragColor;[m
[31m-uniform sampler2D tex;[m
[31m-[m
[31m-in VSOut {[m
[31m-    vec2 uv;[m
[31m-} vs_out;[m
[31m-[m
[31m-void main() {[m
[31m-    vec4 color = texture(tex, vs_out.uv);[m
[31m-    if (color.a == 0.0) discard;[m
[31m-    FragColor = color;[m
[31m-}[m
[1mdiff --git a/shaders/gl/texture.vert b/shaders/gl/texture.vert[m
[1mdeleted file mode 100644[m
[1mindex 24987b4..0000000[m
[1m--- a/shaders/gl/texture.vert[m
[1m+++ /dev/null[m
[36m@@ -1,14 +0,0 @@[m
[31m-#version 450 core[m
[31m-[m
[31m-in vec2 a_pos;[m
[31m-in vec2 a_uv;[m
[31m-uniform mat4 u_mvp;[m
[31m-[m
[31m-out VSOut {[m
[31m-    vec2 uv;[m
[31m-} vs_out;[m
[31m-[m
[31m-void main() {[m
[31m-    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);[m
[31m-    vs_out.uv = a_uv;[m
[31m-}[m
[1mdiff --git a/src/shaders.h b/src/shaders.h[m
[1mindex ec10bd4..c2ba80b 100644[m
[1m--- a/src/shaders.h[m
[1m+++ b/src/shaders.h[m
[36m@@ -7,12 +7,12 @@[m [mnamespace shaders {[m
 namespace vertex {[m
 [m
 inline constexpr char default_[] = {[m
[31m-#embed "../shaders/gl/default.vert" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/default.vert" suffix(,)[m
 '\0'[m
 };[m
 [m
 inline constexpr char texture[] = {[m
[31m-#embed "../shaders/gl/texture.vert" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/texture.vert" suffix(,)[m
 '\0'[m
 };[m
 [m
[36m@@ -21,22 +21,22 @@[m [minline constexpr char texture[] = {[m
 namespace fragment {[m
 [m
 inline constexpr char text[] = {[m
[31m-#embed "../shaders/gl/text.frag" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/text.frag" suffix(,)[m
 '\0'[m
 };[m
 [m
 inline constexpr char default_[] = {[m
[31m-#embed "../shaders/gl/default.frag" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/default.frag" suffix(,)[m
 '\0'[m
 };[m
 [m
 inline constexpr char texture[] = {[m
[31m-#embed "../shaders/gl/texture.frag" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/texture.frag" suffix(,)[m
 '\0'[m
 };[m
 [m
 inline constexpr char circle[] = {[m
[31m-#embed "../shaders/gl/circle.frag" suffix(,)[m
[32m+[m[32m#embed "../shaders/opengl4.5/circle.frag" suffix(,)[m
 '\0'[m
 };[m
 [m
