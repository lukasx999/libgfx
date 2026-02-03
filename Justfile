web:
    emcmake cmake -B build -DPLATFORM_WEB=1
    emmake make -C build -j$(nproc)

build: configure
    cmake --build build

configure:
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

install: build
    cmake --install build
