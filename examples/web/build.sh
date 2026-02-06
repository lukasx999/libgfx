#!/bin/sh
set -e

em++ -std=c++23 main.cc -sUSE_GLFW=3 -sMIN_WEBGL_VERSION=2 -sUSE_FREETYPE=1 \
    -L../../build/ \
    -I../../include/ \
    -lgfx \
-o index.js

python -m http.server
