#! /usr/bin/env python3
import os
import re

# -- helper functions --

def build_library(path, commands):
    print("building library under {}".format(path))

    old_path = os.getcwd()
    os.chdir(path)
    for cmd in commands:
        print("running {}".format(cmd))
        print(os.popen(cmd).read())

    os.chdir(old_path)

def find_sources(path):
    print("searching sources under {}".format(path))

    sources = tuple(filter(lambda f: f.endswith(".cpp"), os.listdir(path)))
    objects = tuple(map(lambda f: re.sub(".cpp$", ".o", f), sources))

    # add path prefix to files
    sources = tuple(map(lambda f: path + "/" + f, sources))
    objects = tuple(map(lambda f: "build/" + path + "/" + f, objects))

    return sources, objects


# -- prepare build.ninja --

with open("build.ninja", "w") as bf:

    # include rules
    print("include ninja/rules.ninja", file=bf)
    print("\n", file=bf)

    # build dependencies
    build_library("lib/libmm", ["ninja"])
    build_library("lib/libwsdl2", ["./configure.py", "ninja"])

    # find engine sources
    sources, objects = find_sources("engine")

    # create build directories
    print("build build/engine: mkdir", file=bf)
    print("build build/test: mkdir", file=bf)
    print("\n", file=bf)


    # build engine files
    for s, o in zip(sources, objects):
        print("build {}: cpp {}".format(o, s), file=bf)

    # build engine library
    print("build build/libflatland.so: link-shared " + " ".join(objects), file=bf)
    print("build build/libflatland.a: link-static " + " ".join(objects), file=bf)
    print("\n", file=bf)

    # set as default
    print("default build/libflatland.so build/libflatland.a", file=bf)
    print("\n", file=bf)

    # find test sources
    sources, objects = find_sources("test")
    binaries = tuple(map(lambda f: re.sub(".o$", "", f), objects))

    # build tests
    for s, o, b in zip(sources, objects, binaries):
        print("build {}: cpp {}".format(o, s), file=bf)
        print("build {}: link {}".format(b, o), file=bf)
        print("    lflags = $lflags ../build/libflatland.so", file=bf)
        print("\n", file=bf)
