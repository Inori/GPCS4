set_project("GPCS4")
set_version("0.2.3")
set_xmakever("2.8.1")

add_rules("mode.release", "mode.debug")

set_allowedplats("windows")
set_allowedarchs("x64")
set_allowedmodes("debug", "release")

-- Only support clang
if is_plat("windows") then
    set_toolchains("clang-cl")

    if is_mode("debug") then
        set_runtimes("MTd")
    else
        set_runtimes("MT")
    end
else
    set_toolchains("clang")
end

if is_mode("debug") then
    add_defines("_DEBUG")
else
    add_defines("NDEBUG")
end


includes("3rdParty/glfw")
includes("3rdParty/rtaudio")
includes("3rdParty/zydis")
includes("3rdParty/tinydbr")

if is_plat("windows") then 
    includes("3rdParty/winpthreads")
end

includes("xmake")

target("GPCS4")
    add_rules("module.program")
    set_languages("c11", "cxx17")

    -- C/C++ Flags
    add_cxxflags("-Wno-unused-variable",
                 "-Wno-unused-private-field",
                 "-Wno-switch",
                 "-Wno-return-type",
                 "-Wno-unused-function",
                 "-Wno-microsoft-enum-forward-reference",
                 "-Wno-nonportable-include-path")

    if is_mode("release") then
        -- add_cxxflags("-flto=thin")
    end

    -- C/C++ Defines
    add_defines("FMT_HEADER_ONLY")

    if is_plat("windows") then 
        add_defines("GPCS4_WINDOWS")
        add_defines("__PTW32_STATIC_LIB",
                    "_CRT_SECURE_NO_WARNINGS")
    else 
        add_defines("GPCS4_LINUX")
    end 

    if is_mode("debug") then
        add_defines("GPCS4_DEBUG")
    end

    add_includedirs("GPCS4/",
                    "GPCS4/Emulator",
                    "GPCS4/Algorithm",
                    "GPCS4/Common",
                    "GPCS4/Platform",
                    "GPCS4/Util",
                    "GPCS4/Util/Allocator",
                    "GPCS4/Graphics",
                    "GPCS4/SceModules",
                    "3rdParty",
                    "3rdParty/zydis/include",
                    "3rdParty/zydis/dependencies/zycore/include",
                    "3rdParty/fmt/include",
                    "3rdParty/boost")

    add_headerfiles("GPCS4/**.h")
    add_files("GPCS4/**.cpp", "GPCS4/**.c")

    local shader_path = "GPCS4/Graphics/Sce/Shaders"
    add_rules("module.shader", {outputdir = path.join(os.projectdir(), shader_path)})
    add_files(path.join(shader_path, "*.vert"))
    add_files(path.join(shader_path, "*.frag"))

    -- linked libraries
    if is_plat("windows") then
        add_syslinks("ksuser",
                     "mfplat",
                     "mfuuid",
                     "wmcodecdspuuid",
                     "legacy_stdio_definitions",
                     "user32")
    else
        print("gpcs4 TODO")
    end

    add_packages("vulkansdk")

    add_deps("glfw")
    add_deps("rtaudio")
    add_deps("zydis")
    add_deps("tinydbr")

    if is_plat("windows") then 
        add_deps("winpthreads")
    end
