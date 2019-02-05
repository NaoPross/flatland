--[[ project solution / workspace, global configuration ]]--
solution "flatland"
    configurations { "debug", "release" }

    -- build configurations
    flags { "ExtraWarnings" }
    buildoptions { "-std=c++17" }

    --- for linux specifically
    configuration { "linux", "gmake" }
        buildoptions {
            "-pedantic", "-Wall", "-Wextra", "-Wcast-qual", "-Wcast-align",
            "-Wpointer-arith", "-Winit-self", "-Wshadow", "-Wswitch-enum", 
            "-Wredundant-decls", "-Wfloat-equal", "-Wundef", "-Wvla", 
            "-Wconversion", "-Wstrict-aliasing"
        }

--[[ sdl ]]--
sdl = libdirs { os.findlib("SDL2") }

if (#sdl ~= 0) then
    sdl = sdl[1].."/libSDL2.so"
    print("SDL library found: "..sdl)
else
    sdl = nil
    print("SDL library not found")
end

--[[ custom lua code ]]--
local test = {
    add_specific = function(name, src, tested_src, deps)
        local sources = {}
        table.insert(sources, src)

        io.write("Added specific test " .. src .. " to test { ")
        for k, v in pairs(tested_src) do
            io.write(v .. " " )
            table.insert(sources, v)
        end
        print("}")

        project("libflatland-test-" .. name)
            language("C++")
            kind("ConsoleApp")
            location("build/")

            includedirs({ "engine/include", "lib/include" })
            files(sources)

            links(deps)

            configuration("debug")
                targetdir("build/debug")
                defines({ "DEBUG" })
                flags({ "Symbols" })

            configuration("release")
                targetdir("build/release")
                flags({ "OptimizeSpeed" })
    end
}

local dependency = {
    build = function(path, commands)
        print("Building dependency under " .. path)
        for k, cmd in pairs(commands) do
            print("  Running " .. cmd)
            os.execute("cd " .. path .. "; " .. cmd)
        end
    end
}

--[[ dependencies ]]--
--dependency.build("lib/libwsdl2", { "./configure.py", "ninja" })
--dependency.build("lib/libmm", { "ninja" })

--[[ project libmm ]]--
project "mm"
    -- project
    language "C++"
    kind "SharedLib"
    location "lib/libmm/build"
    
    -- source code
    files { "lib/libmm/*.cpp" }
    includedirs { "lib/libmm/include" }

    configuration "debug"
        targetdir "lib/libmm/build/debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "release"
        targetdir "lib/libmm/build/release"
        flags { "OptimizeSpeed" }

--[[ project libwsdl2 ]]--
project "wsdl2"
    -- project
    language "C++"
    kind "SharedLib"
    location "lib/libwsdl2/build"

    -- source code
    files { "lib/libwsdl2/*.cpp" }
    includedirs { "lib/libwsdl2/include", "lib/include" }

    --dependencies
    links { "mm"}

    configuration "debug"
        targetdir "lib/libwsdl2/build/debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "release"
        targetdir "lib/libwsdl2/build/release"
        flags { "OptimizeSpeed" }

--[[ main project ]]--
project "flatland"
    -- project
    language "C++"
    kind "SharedLib"
    location "build/"

    -- source code
    files { "engine/**.cpp" }
    includedirs { "lib/include", "engine/include" }

    --dependencies
    links { "mm", "wsdl2" }

    configuration "debug"
        targetdir "build/debug"
        defines { "DEBUG" }
        flags { "Symbols" }
    
    configuration "release"
        targetdir "build/release"
        flags { "OptimizeSpeed" }

-- add tests
test.add_specific("task", "test/task_test.cpp", {
    "engine/task.cpp"
}, {})

sig_test_dep = {"wsdl2"}

if sdl ~= nil then
    sig_test_dep[#sig_test_dep + 1] = "SDL2"
end

test.add_specific("signal", "test/signal_test.cpp", {
    "engine/signal.cpp",
    "engine/labelled.cpp",
    "engine/task.cpp",
    "engine/flatland.cpp"
}, sig_test_dep)

--[[ other tools ]]--
-- generate tags file for vim
os.execute("if type ctags; then ctags -R --extra=f -f .tags . ; fi")
