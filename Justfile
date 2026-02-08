build: configure
    cmake --build build

configure:
    command -v glslangValidator 1>/dev/null && glslangValidator shaders/gles3/* shaders/opengl4.5/*
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

web:
    emcmake cmake -B build
    emmake make -C build -j$(nproc)

install: build
    cmake --install build
