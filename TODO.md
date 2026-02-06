# TODO

## Web

- [ ] build emscripten with cmake
- [x] custom emscripten wrapper shell
- [ ] fix missing emscripten gl functions
- [ ] build examples for the web

## Graphics

- [ ] systems for coordinating animations (seq, parallel)
- [ ] spritesheet example
- [x] render bezier curves
- [ ] cubic bezier
- [ ] MSAA for circle and bezier curve
- [ ] line thickness setting for bezier curves
- [ ] splines
- [ ] fix broken text rotation: every character gets rotated
- [ ] do copying of textures on the gpu, so there's no need for glGetTexImage anymore
- [ ] stencil buffer
- [x] draw from spritesheet directly

## Misc

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
