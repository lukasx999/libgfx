[1mdiff --git a/include/detail/texture.hh b/include/detail/texture.hh[m
[1mindex 16b3c87..2243593 100644[m
[1m--- a/include/detail/texture.hh[m
[1m+++ b/include/detail/texture.hh[m
[36m@@ -61,33 +61,27 @@[m [mprivate:[m
 [m
 namespace gfx::detail {[m
 [m
[31m-class TextureRenderer : public IDeferredRenderer {[m
[32m+[m[32mclass TextureRenderer {[m
     gfx::Window& m_window;[m
 [m
     GLuint m_program;[m
     GLuint m_vertex_array;[m
     GLuint m_vertex_buffer;[m
     GLuint m_index_buffer;[m
[31m-    GLuint m_uv_buffer;[m
[31m-    GLuint m_transform_buffer;[m
[31m-[m
[31m-    struct RenderGroup {[m
[31m-        const gfx::Texture& texture;[m
[31m-        std::vector<glm::vec2> vertices;[m
[31m-        std::vector<unsigned int> indices;[m
[31m-        std::vector<glm::vec2> uvs;[m
[31m-        std::vector<glm::mat4> transforms;[m
[32m+[m
[32m+[m[32m    static constexpr std::array m_indices {[m
[32m+[m[32m        0u, // top-left[m
[32m+[m[32m        1u, // top-right[m
[32m+[m[32m        2u, // bottom-left[m
[32m+[m[32m        3u, // bottom-right[m
[32m+[m[32m        2u, // bottom-left[m
[32m+[m[32m        1u, // top-right[m
     };[m
 [m
[31m-    // the texture sampler is a uniform, so we create groups of transforms per texture[m
[31m-    // and batch-render each group, resulting in one draw call per texture[m
[31m-    //[m
[31m-    // for identifying each group, we just hash the raw pointer to the heap-allocated[m
[31m-    // memory of the texture[m
[31m-    // this means that theoretically 2 identical textures could end up being in a[m
[31m-    // different group, if the user has 2 seperate `gfx::Texture` objects[m
[31m-    // however, this should never be a problem in practice[m
[31m-    std::unordered_map<unsigned char*, RenderGroup> m_render_group;[m
[32m+[m[32m    struct Vertex {[m
[32m+[m[32m        glm::vec2 pos;[m
[32m+[m[32m        glm::vec2 uv;[m
[32m+[m[32m    };[m
 [m
 public:[m
     explicit TextureRenderer(gfx::Window& window);[m
[36m@@ -101,7 +95,6 @@[m [mpublic:[m
         const gfx::Texture& texture,[m
         glm::mat4 view[m
     );[m
[31m-    void flush() override;[m
 [m
 };[m
 [m
[1mdiff --git a/include/detail/triangle.hh b/include/detail/triangle.hh[m
[1mindex e6c890a..b3a382c 100644[m
[1m--- a/include/detail/triangle.hh[m
[1m+++ b/include/detail/triangle.hh[m
[36m@@ -8,23 +8,16 @@[m
 [m
 namespace gfx::detail {[m
 [m
[31m-class TriangleRenderer : public IDeferredRenderer {[m
[32m+[m[32mclass TriangleRenderer {[m
     gfx::Window& m_window;[m
 [m
     GLuint m_program;[m
     GLuint m_vertex_array;[m
     GLuint m_vertex_buffer;[m
[31m-    GLuint m_color_buffer;[m
[31m-    GLuint m_transform_buffer;[m
[31m-[m
[31m-    std::vector<glm::vec2> m_vertices;[m
[31m-    std::vector<glm::vec4> m_colors;[m
[31m-    std::vector<glm::mat4> m_transforms;[m
 [m
 public:[m
     explicit TriangleRenderer(gfx::Window& window);[m
     void draw(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color, glm::mat4 view);[m
[31m-    void flush() override;[m
 [m
 };[m
 [m
[1mdiff --git a/include/renderer.hh b/include/renderer.hh[m
[1mindex 6c6b81e..f1dd080 100644[m
[1m--- a/include/renderer.hh[m
[1m+++ b/include/renderer.hh[m
[36m@@ -43,11 +43,6 @@[m [mclass Renderer {[m
     detail::LineRenderer m_line;[m
     detail::TextRenderer m_text;[m
 [m
[31m-    std::array<std::reference_wrapper<detail::IDeferredRenderer>, 2> m_deferred_renderers {[m
[31m-        m_triangle,[m
[31m-        m_texture,[m
[31m-    };[m
[31m-[m
     double m_frame_time = 0.0;[m
     double m_last_frame = 0.0;[m
 [m
[36m@@ -88,7 +83,6 @@[m [mpublic:[m
 [m
         draw_fn();[m
 [m
[31m-        flush();[m
         glfwSwapBuffers(m_window.m_window);[m
         glfwPollEvents();[m
 [m
[36m@@ -143,7 +137,6 @@[m [mpublic:[m
         const gfx::IRotation& rotation,[m
         gfx::Color color[m
     ) {[m
[31m-        flush();[m
         m_rectangle.draw(x, y, width, height, rotation, color, m_view_active);[m
     }[m
 [m
[36m@@ -181,7 +174,6 @@[m [mpublic:[m
         const gfx::IRotation& rotation,[m
         const gfx::Texture& texture[m
     ) {[m
[31m-        flush_all_except(m_texture);[m
         m_texture.draw(x, y, width, height, rotation, texture, m_view_active);[m
     }[m
 [m
[36m@@ -196,7 +188,6 @@[m [mpublic:[m
     }[m
 [m
     void draw_circle(float x, float y, float radius, gfx::Color color) {[m
[31m-        flush();[m
         m_circle.draw(x, y, radius, color, m_view_active);[m
     }[m
 [m
[36m@@ -205,7 +196,6 @@[m [mpublic:[m
     }[m
 [m
     void draw_triangle(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color) {[m
[31m-        flush_all_except(m_triangle);[m
         m_triangle.draw(x0, y0, x1, y1, x2, y2, color, m_view_active);[m
     }[m
 [m
[36m@@ -214,7 +204,6 @@[m [mpublic:[m
     }[m
 [m
     void draw_line(float x0, float y0, float x1, float y1, gfx::Color color) {[m
[31m-        flush();[m
         m_line.draw(x0, y0, x1, y1, color, m_view_active);[m
     }[m
 [m
[36m@@ -223,7 +212,6 @@[m [mpublic:[m
     }[m
 [m
     void draw_text(float x, float y, unsigned int text_size, const char* text, const gfx::Font& font, gfx::Color color) {[m
[31m-        flush();[m
         m_text.draw(x, y, text_size, text, font, color, m_view_active);[m
     }[m
 [m
[36m@@ -242,28 +230,6 @@[m [mpublic:[m
     }[m
 [m
 private:[m
[31m-    void flush() {[m
[31m-        for (auto& rd : m_deferred_renderers) {[m
[31m-            rd.get().flush();[m
[31m-        }[m
[31m-    }[m
[31m-[m
[31m-    // make sure all other shapes have already been drawn to the screen,[m
[31m-    // as otherwise the drawing order will be incorrect, leading weird[m
[31m-    // overlapping shapes[m
[31m-    //[m
[31m-    // therefore we have to flush all other shapes before drawing the current one[m
[31m-    //[m
[31m-    // this has the unfortunate effect of breaking batching optimizations[m
[31m-    // when using interleaved rendering when drawing a lot of shapes[m
[31m-    void flush_all_except(const detail::IDeferredRenderer& exception) {[m
[31m-        for (auto& rd : m_deferred_renderers) {[m
[31m-            if (reinterpret_cast<detail::IDeferredRenderer*>(&rd) == &exception)[m
[31m-                continue;[m
[31m-            rd.get().flush();[m
[31m-        }[m
[31m-    }[m
[31m-[m
     [[nodiscard]] static constexpr[m
     glm::mat4 gen_view_matrix(const Window& window, float center_x, float center_y) {[m
         glm::vec3 camera_position([m
[1mdiff --git a/shaders/texture.frag b/shaders/texture.frag[m
[1mindex 6ff1d45..77a21dd 100644[m
[1m--- a/shaders/texture.frag[m
[1m+++ b/shaders/texture.frag[m
[36m@@ -1,10 +1,15 @@[m
 #version 330 core[m
 [m
 out vec4 FragColor;[m
[31m-in vec2 frag_uv;[m
 uniform sampler2D tex;[m
 [m
[32m+[m[32min VSOut {[m
[32m+[m[32m    vec2 uv;[m
[32m+[m[32m} vs_out;[m
[32m+[m
 void main() {[m
[31m-    if (texture(tex, frag_uv).a == 0.0) discard;[m
[31m-    FragColor = texture(tex, frag_uv);[m
[32m+[m[32m    // vec4 color = texture(tex, vs_out.uv);[m
[32m+[m[32m    // if (color.a == 0.0) discard;[m
[32m+[m[32m    // FragColor = color;[m
[32m+[m[32m    FragColor = vec4(1.0, 1.0, 1.0, 1.0);[m
 }[m
[1mdiff --git a/shaders/texture.vert b/shaders/texture.vert[m
[1mindex 65b941f..b271039 100644[m
[1m--- a/shaders/texture.vert[m
[1m+++ b/shaders/texture.vert[m
[36m@@ -2,10 +2,13 @@[m
 [m
 in vec2 a_pos;[m
 in vec2 a_uv;[m
[31m-in mat4 a_mvp;[m
[31m-out vec2 frag_uv;[m
[32m+[m[32muniform mat4 u_mvp;[m
[32m+[m
[32m+[m[32mout VSOut {[m
[32m+[m[32m    vec2 uv;[m
[32m+[m[32m} vs_out;[m
 [m
 void main() {[m
[31m-    gl_Position = a_mvp * vec4(a_pos, 0.0, 1.0);[m
[31m-    frag_uv = a_uv;[m
[32m+[m[32m    gl_Position = u_mvp * vec4(a_pos, 0.0, 1.0);[m
[32m+[m[32m    vs_out.uv = a_uv;[m
 }[m
[1mdiff --git a/src/texture.cc b/src/texture.cc[m
[1mindex 1c75afa..475d521 100644[m
[1m--- a/src/texture.cc[m
[1m+++ b/src/texture.cc[m
[36m@@ -58,27 +58,21 @@[m [mTextureRenderer::TextureRenderer(gfx::Window& window)[m
     glBindVertexArray(m_vertex_array);[m
 [m
     glGenBuffers(1, &m_index_buffer);[m
[32m+[m[32m    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);[m
[32m+[m[32m    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);[m
 [m
     glGenBuffers(1, &m_vertex_buffer);[m
     glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
     GLint a_pos = glGetAttribLocation(m_program, "a_pos");[m
[31m-    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);[m
[32m+[m[32m    glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, pos)));[m
     glEnableVertexAttribArray(a_pos);[m
 [m
[31m-    glGenBuffers(1, &m_uv_buffer);[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);[m
[32m+[m[32m    glGenBuffers(1, &m_vertex_buffer);[m
[32m+[m[32m    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
     GLint a_uv = glGetAttribLocation(m_program, "a_uv");[m
[31m-    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);[m
[32m+[m[32m    glVertexAttribPointer(a_uv, 2, GL_FLOAT, false, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uv)));[m
     glEnableVertexAttribArray(a_uv);[m
 [m
[31m-    glGenBuffers(1, &m_transform_buffer);[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);[m
[31m-    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");[m
[31m-    for (int i = 0; i < 4; ++i) {[m
[31m-        glVertexAttribPointer(a_mvp+i, 4, GL_FLOAT, false, sizeof(glm::vec4)*4, reinterpret_cast<void*>(i * sizeof(glm::vec4)));[m
[31m-        glEnableVertexAttribArray(a_mvp+i);[m
[31m-    }[m
[31m-[m
     // just to make sure everything still works after unbinding, as other classes/functions may[m
     // modify opengl state after running the ctor[m
     glBindVertexArray(0);[m
[36m@@ -97,6 +91,19 @@[m [mvoid TextureRenderer::draw([m
     glm::mat4 view[m
 ) {[m
 [m
[32m+[m[32m    glUseProgram(m_program);[m
[32m+[m[32m    glBindVertexArray(m_vertex_array);[m
[32m+[m
[32m+[m[32m    auto vertices = std::to_array<Vertex>({[m
[32m+[m[32m        { { x,       y        }, { 0.0f, 0.0f } }, // top-left[m
[32m+[m[32m        { { x+width, y        }, { 1.0f, 0.0f } }, // top-right[m
[32m+[m[32m        { { x,       y+height }, { 0.0f, 1.0f } }, // bottom-left[m
[32m+[m[32m        { { x+width, y+height }, { 1.0f, 1.0f } }, // bottom-right[m
[32m+[m[32m    });[m
[32m+[m
[32m+[m[32m    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
[32m+[m[32m    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_DYNAMIC_DRAW);[m
[32m+[m
     // the translations are needed to subtract the world space coordinates (x,y)[m
     // since we want the rectangle to rotate around its top left corner, and[m
     // to set the center of rotation to the middle of the rectangle[m
[36m@@ -114,93 +121,11 @@[m [mvoid TextureRenderer::draw([m
 [m
     glm::mat4 mvp = projection * view * model;[m
 [m
[31m-    if (m_render_group.contains(texture.m_data)) {[m
[31m-        RenderGroup& g = m_render_group.at(texture.m_data);[m
[31m-[m
[31m-        int vertex_count = g.vertices.size();[m
[31m-        g.indices.push_back(0u + vertex_count); // top-left[m
[31m-        g.indices.push_back(1u + vertex_count); // top-right[m
[31m-        g.indices.push_back(2u + vertex_count); // bottom-left[m
[31m-        g.indices.push_back(3u + vertex_count); // bottom-right[m
[31m-        g.indices.push_back(2u + vertex_count); // bottom-left[m
[31m-        g.indices.push_back(1u + vertex_count); // top-right[m
[31m-[m
[31m-        g.vertices.push_back({ x,       y        }); // top-left[m
[31m-        g.vertices.push_back({ x+width, y        }); // top-right[m
[31m-        g.vertices.push_back({ x,       y+height }); // bottom-left[m
[31m-        g.vertices.push_back({ x+width, y+height }); // bottom-right[m
[31m-[m
[31m-        g.uvs.push_back({ 0.0, 0.0 }); // top-left[m
[31m-        g.uvs.push_back({ 1.0, 0.0 }); // top-right[m
[31m-        g.uvs.push_back({ 0.0, 1.0 }); // bottom-left[m
[31m-        g.uvs.push_back({ 1.0, 1.0 }); // bottom-right[m
[31m-[m
[31m-        g.transforms.push_back(mvp);[m
[31m-        g.transforms.push_back(mvp);[m
[31m-        g.transforms.push_back(mvp);[m
[31m-        g.transforms.push_back(mvp);[m
[31m-[m
[31m-    } else {[m
[31m-        RenderGroup group {[m
[31m-            texture,[m
[31m-            {[m
[31m-                { x,       y        }, // top-left[m
[31m-                { x+width, y        }, // top-right[m
[31m-                { x,       y+height }, // bottom-left[m
[31m-                { x+width, y+height }, // bottom-right[m
[31m-            },[m
[31m-            {[m
[31m-                0u, // top-left[m
[31m-                1u, // top-right[m
[31m-                2u, // bottom-left[m
[31m-                3u, // bottom-right[m
[31m-                2u, // bottom-left[m
[31m-                1u, // top-right[m
[31m-            },[m
[31m-            {[m
[31m-                { 0.0, 0.0 }, // top-left[m
[31m-                { 1.0, 0.0 }, // top-right[m
[31m-                { 0.0, 1.0 }, // bottom-left[m
[31m-                { 1.0, 1.0 }, // bottom-right[m
[31m-            },[m
[31m-            {[m
[31m-                mvp,[m
[31m-                mvp,[m
[31m-                mvp,[m
[31m-                mvp,[m
[31m-            }[m
[31m-        };[m
[31m-        m_render_group.emplace(texture.m_data, group);[m
[31m-    }[m
[31m-[m
[31m-}[m
[31m-[m
[31m-void TextureRenderer::flush() {[m
[31m-[m
[31m-    glUseProgram(m_program);[m
[31m-    glBindVertexArray(m_vertex_array);[m
[31m-[m
[31m-    for (auto& [key, value] : m_render_group) {[m
[31m-        auto& [texture, vertices, indices, uvs, transforms] = value;[m
[31m-[m
[31m-        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
[31m-        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_index_buffer);[m
[31m-        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-        glBindBuffer(GL_ARRAY_BUFFER, m_uv_buffer);[m
[31m-        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-        glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);[m
[31m-        glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), transforms.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-        glBindTexture(GL_TEXTURE_2D, texture.m_texture);[m
[31m-[m
[31m-        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);[m
[31m-    }[m
[32m+[m[32m    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");[m
[32m+[m[32m    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));[m
 [m
[31m-    m_render_group.clear();[m
[32m+[m[32m    glBindTexture(GL_TEXTURE_2D, texture.m_texture);[m
[32m+[m[32m    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);[m
 }[m
 [m
 } // namespace detail[m
[1mdiff --git a/src/triangle.cc b/src/triangle.cc[m
[1mindex 85865c2..e12ed77 100644[m
[1m--- a/src/triangle.cc[m
[1m+++ b/src/triangle.cc[m
[36m@@ -14,7 +14,7 @@[m [mTriangleRenderer::TriangleRenderer(gfx::Window& window)[m
 : m_window(window)[m
 {[m
 [m
[31m-    m_program = detail::create_shader_program(shaders::vertex::batched, shaders::fragment::batched);[m
[32m+[m[32m    m_program = detail::create_shader_program(shaders::vertex::default_, shaders::fragment::default_);[m
 [m
     glGenVertexArrays(1, &m_vertex_array);[m
     glBindVertexArray(m_vertex_array);[m
[36m@@ -25,29 +25,24 @@[m [mTriangleRenderer::TriangleRenderer(gfx::Window& window)[m
     glVertexAttribPointer(a_pos, 2, GL_FLOAT, false, sizeof(glm::vec2), nullptr);[m
     glEnableVertexAttribArray(a_pos);[m
 [m
[31m-    glGenBuffers(1, &m_color_buffer);[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);[m
[31m-    GLint a_color = glGetAttribLocation(m_program, "a_color");[m
[31m-    glVertexAttribPointer(a_color, 4, GL_FLOAT, false, sizeof(glm::vec4), nullptr);[m
[31m-    glEnableVertexAttribArray(a_color);[m
[31m-[m
[31m-    glGenBuffers(1, &m_transform_buffer);[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);[m
[31m-    GLint a_mvp = glGetAttribLocation(m_program, "a_mvp");[m
[31m-    for (int i = 0; i < 4; ++i) {[m
[31m-        glVertexAttribPointer(a_mvp+i, 4, GL_FLOAT, false, sizeof(glm::vec4)*4, reinterpret_cast<void*>(i * sizeof(glm::vec4)));[m
[31m-        glEnableVertexAttribArray(a_mvp+i);[m
[31m-    }[m
[31m-[m
     glBindVertexArray(0);[m
     glUseProgram(0);[m
     glBindBuffer(GL_ARRAY_BUFFER, 0);[m
 }[m
 [m
 void TriangleRenderer::draw(float x0, float y0, float x1, float y1, float x2, float y2, gfx::Color color, glm::mat4 view) {[m
[31m-    m_vertices.push_back({ x0, y0 });[m
[31m-    m_vertices.push_back({ x1, y1 });[m
[31m-    m_vertices.push_back({ x2, y2 });[m
[32m+[m
[32m+[m[32m    glUseProgram(m_program);[m
[32m+[m[32m    glBindVertexArray(m_vertex_array);[m
[32m+[m
[32m+[m[32m    auto vertices = std::to_array<glm::vec2>({[m
[32m+[m[32m        { x0, y0 },[m
[32m+[m[32m        { x1, y1 },[m
[32m+[m[32m        { x2, y2 },[m
[32m+[m[32m    });[m
[32m+[m
[32m+[m[32m    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
[32m+[m[32m    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), vertices.data(), GL_DYNAMIC_DRAW);[m
 [m
     glm::mat4 model(1.0);[m
 [m
[36m@@ -60,36 +55,15 @@[m [mvoid TriangleRenderer::draw(float x0, float y0, float x1, float y1, float x2, fl[m
 [m
     glm::mat4 mvp = projection * view * model;[m
 [m
[31m-    m_transforms.push_back(mvp);[m
[31m-    m_transforms.push_back(mvp);[m
[31m-    m_transforms.push_back(mvp);[m
[32m+[m[32m    GLint u_mvp = glGetUniformLocation(m_program, "u_mvp");[m
[32m+[m[32m    glUniformMatrix4fv(u_mvp, 1, false, glm::value_ptr(mvp));[m
 [m
     auto c = color.normalized();[m
[31m-    glm::vec4 cv(c.r, c.g, c.b, c.a);[m
[31m-    m_colors.push_back(cv);[m
[31m-    m_colors.push_back(cv);[m
[31m-    m_colors.push_back(cv);[m
[31m-}[m
[31m-[m
[31m-void TriangleRenderer::flush() {[m
[31m-[m
[31m-    glBindVertexArray(m_vertex_array);[m
[31m-    glUseProgram(m_program);[m
[31m-[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);[m
[31m-    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec2), m_vertices.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_color_buffer);[m
[31m-    glBufferData(GL_ARRAY_BUFFER, m_colors.size() * sizeof(glm::vec4), m_colors.data(), GL_DYNAMIC_DRAW);[m
[31m-[m
[31m-    glBindBuffer(GL_ARRAY_BUFFER, m_transform_buffer);[m
[31m-    glBufferData(GL_ARRAY_BUFFER, m_transforms.size() * sizeof(glm::mat4), m_transforms.data(), GL_DYNAMIC_DRAW);[m
[32m+[m[32m    GLint u_color = glGetUniformLocation(m_program, "u_color");[m
[32m+[m[32m    glUniform4f(u_color, c.r, c.g, c.b, c.a);[m
 [m
[31m-    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());[m
[32m+[m[32m    glDrawArrays(GL_TRIANGLES, 0, vertices.size());[m
 [m
[31m-    m_colors.clear();[m
[31m-    m_vertices.clear();[m
[31m-    m_transforms.clear();[m
 }[m
 [m
 } // namespace gfx::detail[m
