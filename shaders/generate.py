#!/usr/bin/env python3

import sys
import os

def name_is_cxx_keyword(name: str) -> bool:
    keywords = [ "default" ]
    return name in keywords

def gen_entry(file, name: str, path: str):
    if name_is_cxx_keyword(name):
        name += "_"

    file.write(f"""\
inline constexpr char {name}[] = {{
#embed "../shaders/{path}" suffix(,)
'\\0'
}};\n\n\
""");

def gen_namespace(file, name: str, prefix: str, shaders: list[str]):

    file.write(f"namespace {name} {{\n\n")

    for shader in shaders:
        name, _ = shader.split(".")
        gen_entry(file, name, f"{prefix}/{shader}")

    file.write(f"}} // namespace {name}\n\n")


def split_shader_types(shaders_filenames: list[str]) -> tuple[list[str], list[str]]:

    vertex_shaders = []
    fragment_shaders = []

    for filename in shaders_filenames:
        _, extension = filename.split(".")
        match extension:
            case "frag":
                fragment_shaders.append(filename)
            case "vert":
                vertex_shaders.append(filename)

    return vertex_shaders, fragment_shaders

def main() -> int:

    if len(sys.argv) != 2:
        print("expected shader type as argument (`gl` or `es`)")
        return 1

    match sys.argv[1]:
        case "gl":
            subdir = "opengl4.5"
        case "es":
            subdir = "gles3"
        case _:
            print("unknown shader type (must be either `gl` or `es`)")
            return 1

    file = open("shaders.h", "w")
    file.write("// THIS FILE IS GENERATED\n\n")
    file.write("#pragma once\n\n")
    file.write("namespace shaders {\n\n")

    shaders = os.listdir(subdir)
    vertex, fragment = split_shader_types(shaders)

    gen_namespace(file, "vertex", subdir, vertex)
    gen_namespace(file, "fragment", subdir, fragment)

    file.write("} // namespace shaders\n")
    return 0


if __name__ == '__main__':
    sys.exit(main())
