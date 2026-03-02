# TODO

## Web

- [ ] build examples for the web (gallery on github pages)
- [ ] fix missing emscripten gl functions
- [x] build emscripten with cmake
- [x] custom emscripten wrapper shell

## Graphics

- [ ] implement measure_char_total_height() - fix font measurements
- [ ] implement draw_polygon()
- [ ] segmented circles
- [ ] shape morphing using GL_POLYGON and lerp()
- [ ] spritesheet example
- [ ] splines
- [ ] fix broken text rotation: every character gets rotated
- [ ] stencil buffer
- [x] refactor animation code (put common functions into base class, find an alternative to dispatch() function)
- [x] draw from spritesheet directly
- [x] AA for circle and bezier curve (using fwidth())
- [x] render bezier curves
- [x] cubic bezier
- [x] line thickness setting for bezier curves

## Misc

- [ ] move texture impl static functions to texture src?
- [ ] InputHandler class
- [ ] Vec constructor (single value for both fields)
- [ ] integrate WaylandWindow into libgfx
- [ ] fix class member order
- [ ] implement texture write to file
- [ ] texture api for loading pixels to cpu
- [ ] fix gtkmm example
- [ ] switch to pure gles3
- [ ] gradients
- [ ] utf-8 support for text renderer
- [ ] cmake options for opengl function loaders
- [ ] hot reloading
- [ ] cmake fetchcontent for glm
- [ ] make pkg-config file work
- [ ] clean up vector operations
- [ ] detect default font (via fontconfig)
- [ ] cursor always starts out at 0 0
- [ ] make texture parameters configurable
- [ ] font glyph caching
- [x] rect scale method
- [x] generate shaders.h file in build folder
