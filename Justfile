build: configure
    cmake --build build

web: configure
    cmake -B build -G Ninja -DPLATFORM_WEB=1

configure:
    cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang

install: build
    cmake --install build
