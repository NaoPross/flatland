#! /usr/bin/env python3
import os
import re

def find_sources(path):
    sources = tuple(filter(lambda f: f.endswith(".cpp"), os.listdir(path)))
    objects = tuple(map(lambda f: re.sub(".cpp$", ".o", f), sources))

    # add path prefix to files
    sources = tuple(map(lambda f: path + "/" + f, sources))
    objects = tuple(map(lambda f: "build/" + path + "/" + f, objects))

    return sources, objects


with open("build.ninja", "w") as bf:

    # include rules
    print("include ninja/rules.ninja", file=bf)
    print("\n", file=bf)

    # build libmm dependencies
    print("build lib/libmm/build/libmm.so: ninja lib/libmm", file=bf)
    print("build lib/libmm/build/libmm.a: ninja lib/libmm", file=bf)
    print("\n", file=bf)

    # build libwsdl2 dependencies
    print("build lib/libwsdl2/build/libwsdl2.so: ninja lib/libmm", file=bf)
    print("build lib/libwsdl2/build/libwsdl2.a: ninja lib/libmm", file=bf)
    print("\n", file=bf)

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
