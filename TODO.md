# TODO

## Web

- [ ] build examples for the web (gallery on github pages)
- [ ] fix missing emscripten gl functions
- [x] build emscripten with cmake
- [x] custom emscripten wrapper shell

## Graphics

- [ ] refactor animation code (put common functions into base class, find an alternative to dispatch() function)
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
- [x] rect scale method
- [x] generate shaders.h file in build folder
- [x] let glPixelStorei() only affect texture ctor
