#!/usr/bin/env bash
set -euxo pipefail

just -f ../../Justfile
c++ main.cc -std=c++23 -L../../build -I../../include -lgfx -lglfw -lfreetype
