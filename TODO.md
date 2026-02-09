# TODO

## Web

- [ ] fix missing emscripten gl functions
- [ ] build examples for the web
- [x] build emscripten with cmake
- [x] custom emscripten wrapper shell

## Graphics

- [x] AA for circle and bezier curve (using fwidth())
- [ ] reuse functionality in renderers using inheritance
- [ ] systems for coordinating animations (seq, parallel)
- [ ] spritesheet example
- [x] render bezier curves
- [x] cubic bezier
- [x] line thickness setting for bezier curves
- [ ] splines
- [ ] fix broken text rotation: every character gets rotated
- [ ] do copying of textures on the gpu, so there's no need for glGetTexImage anymore
- [ ] stencil buffer
- [x] draw from spritesheet directly

## Misc

- [ ] generate shaders.h file in build folder
- [ ] cmake options for opengl function loaders
- [ ] asset packer
- [ ] hot reloading
- [ ] cmake fetchcontent for glm
- [ ] make pkg-config file work
- [ ] clean up vector operations
- [ ] detect default font (via fontconfig)
- [ ] let glPixelStorei() only affect texture ctor
- [ ] cursor always starts out at 0 0
- [ ] font copy/move ctor
- [ ] dynamic resizing for external context?
- [ ] make texture parameters configurable
- [x] rect scale method
