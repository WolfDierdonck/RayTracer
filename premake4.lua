#!lua

includeDirList = { 
    "shared",
    "shared/gl3w",
    "shared/imgui",
    "shared/include"
}

buildOptions = {"-std=c++14"}

-- Get the current OS platform
PLATFORM = os.get()

-- Build glfw3 static library
if not os.isfile("lib/libglfw3.a") then
    os.chdir("shared/glfw-3.3.8")
    os.mkdir("build")
    os.chdir("build")
    os.execute("cmake ../")
    os.execute("make")
    os.chdir("../../../")
    os.mkdir("lib")
    os.execute("cp shared/glfw-3.3.8/build/src/libglfw3.a lib/")
end

-- Build lua-5.3.1 library
if not os.isfile("lib/liblua.a") then
    os.chdir("shared/lua-5.4.6")

    if PLATFORM == "macosx" then
        os.execute("make macosx")
    elseif PLATFORM == "linux" then
        os.execute("make linux")
    elseif PLATFORM == "windows" then
        os.execute("make mingw")
    end

    os.chdir("../../")
    os.execute("cp shared/lua-5.4.6/src/liblua.a lib/")
end


solution "BuildStaticLibs"
    configurations { "Debug", "Release" }

    configuration "Debug"
        defines { "DEBUG" }
        flags { "Symbols" }

    configuration "Release"
        defines { "NDEBUG" }
        flags { "Optimize" }

    -- Build imgui static library
    project "imgui"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        includedirs (includeDirList)
        includedirs {
            "shared/imgui/examples/opengl3_example",
            "shared/imgui/examples/libs/gl3w/",
        }
        files { 
            "shared/imgui/*.cpp",
            "shared/gl3w/GL/gl3w.c"
        }

    -- Build lodepng static library
    project "lodepng"
        kind "StaticLib"
        language "C++"
        location "build"
        objdir "build"
        targetdir "lib"
        includedirs (includeDirList)
        includedirs {
            "shared/lodepng"
        }
        files { 
            "shared/lodepng/lodepng.cpp"
        }
