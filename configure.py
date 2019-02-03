#! /usr/bin/env python3
import os
import re
import sys

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

    # find files
    sources = tuple(filter(lambda f: f.endswith(".cpp"), os.listdir(path)))

    # replace extension
    objects = tuple(map(lambda f: re.sub(".cpp$", ".o", f), sources))

    # add path prefix
    sources = tuple(map(lambda f: path + "/" + f, sources))
    objects = tuple(map(lambda f: "build/" + path + "/" + f, objects))

    return sources, objects


def add_test(bf, test_source, library):
    print("adding test {}".format(test_source))
    
    # replace extension
    test_object = re.sub(".cpp$", ".o", test_source)
    test_binary = re.sub(".o$", "", test_object)

    # add path prefix
    test_source = "test/" + test_source
    test_object = "build/test/" + test_object
    test_binary = "build/test/" + test_binary
    
    print("build {}: cpp {}".format(test_object, test_source), file=bf)
    print("build {}: link {}".format(test_binary, test_object), file=bf)
    print("    lflags = $lflags {}".format(library), file=bf)
    print("\n", file=bf)


def add_specific_test(bf, test_source, tested_sources):
    print("adding specific test {}".format(test_source) + " to test {}".format(tested_sources))

    # replace extension
    test_object = re.sub(".cpp$", ".o", test_source)
    test_binary = re.sub(".o$", "", test_object)

    tested_objects = tuple(map(lambda f: re.sub(".cpp$", ".o", f), tested_sources))

    # add path prefix
    test_source = "test/" + test_source
    test_object = "build/test/" + test_object
    test_binary = "build/test/" + test_binary

    tested_objects = tuple(map(lambda f: "build/" + f, tested_objects))

    # append to build file
    print("build {}: cpp {}".format(test_object, test_source), file=bf)
    print("build {}: link {} {}".format(test_binary, test_object, " ".join(tested_objects)), file=bf)
    print("\n", file=bf)



# -- prepare build.ninja --

with open("build.ninja", "w") as bf:

    # include rules
    if "--release" in sys.argv:
        print("creating release target!\n")
        print("include ninja/release-rules.ninja", file=bf)
    else:
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

    # add tests
    add_specific_test(bf, "task_test.cpp", ["engine/task.cpp"])
    add_specific_test(bf, "signal_test.cpp", [
        "engine/task.cpp",
        "engine/labelled.cpp",
        "engine/signal.cpp",
    ])

    print("default build/test/signal_test", file=bf)

# run ctags for vim :)
os.system("if type ctags; then ctags -R --extra=f . ; fi")

