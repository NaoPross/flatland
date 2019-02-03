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


--[[ custom lua code ]]--
local test = {
    add_specific = function(name, src, tested_src)
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
dependency.build("lib/libwsdl2", { "./configure.py", "ninja" })
dependency.build("lib/libmm", { "ninja" })

--[[ main project ]]--
project "libflatland"
    -- project
    language "C++"
    kind "SharedLib"
    location "build/"

    -- source code
    files { "engine/**.cpp" }
    includedirs { "lib/include", "engine/include" }

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
})

test.add_specific("signal", "test/signal_test.cpp", {
    "engine/signal.cpp",
    "engine/labelled.cpp",
    "engine/task.cpp"
})

--[[ other tools ]]--
-- generate tags file for vim
os.execute("if type ctags; then ctags -R --extra=f -f .tags . ; fi")
