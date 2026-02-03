build: configure
    cmake --build build

configure:
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

web:
    emcmake cmake -B build
    emmake make -C build -j$(nproc)

install: build
    cmake --install build
