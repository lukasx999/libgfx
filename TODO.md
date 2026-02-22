# TODO

## Web

- [ ] build examples for the web (gallery on github pages)
- [ ] fix missing emscripten gl functions
- [x] build emscripten with cmake
- [x] custom emscripten wrapper shell

## Graphics

- [ ] reuse functionality in renderers using inheritance
- [ ] systems for coordinating animations (seq, parallel)
- [ ] spritesheet example
- [ ] splines
- [ ] fix broken text rotation: every character gets rotated
- [ ] do copying of textures on the gpu, so there's no need for glGetTexImage anymore
- [ ] stencil buffer
- [x] draw from spritesheet directly
- [x] AA for circle and bezier curve (using fwidth())
- [x] render bezier curves
- [x] cubic bezier
- [x] line thickness setting for bezier curves

## Misc

- [ ] fix gtkmm example
- [ ] switch to pure gles3
- [ ] gradients
- [ ] fix glsllangValidator in justfile
- [ ] utf-8 support for text renderer
- [ ] generate shaders.h file in build folder
- [ ] cmake options for opengl function loaders
- [ ] hot reloading
- [ ] cmake fetchcontent for glm
- [ ] make pkg-config file work
- [ ] clean up vector operations
- [ ] detect default font (via fontconfig)
- [ ] let glPixelStorei() only affect texture ctor
- [ ] cursor always starts out at 0 0
- [ ] font copy/move ctor
- [ ] make texture parameters configurable
- [x] rect scale method
