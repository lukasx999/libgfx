build: configure
    cmake --build build

configure:
    cmake -Bbuild -GNinja

check:
    glslangValidator shaders/gles3/* shaders/opengl4.5/*

web:
    emcmake cmake -Bbuild -GNinja
    cmake --build build

install: build
    cmake --install build
