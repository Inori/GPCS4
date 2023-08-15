set_project("GPCS4")
set_xmakever("2.8.1")

add_rules("mode.release", "mode.debug")

-- Only support x64
set_plat(os.host())
set_arch("x64")

-- Only support clang
if is_os("windows") then
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
includes("3rdParty/winpthreads")
includes("3rdParty/rtaudio")
includes("3rdParty/zydis")
includes("3rdParty/tinydbr")


target("GPCS4")
    set_kind("binary")
    set_languages("cxx17")

    -- C/C++ Flags
    add_cxxflags("-Wno-unused-variable",
                 "-Wno-unused-private-field",
                 "-Wno-switch",
                 "-Wno-return-type",
                 "-Wno-unused-function",
                 "-Wno-microsoft-enum-forward-reference",
                 "-Wno-nonportable-include-path")
    if is_mode("release") then
        add_cxxflags("-flto=thin")
    end

    -- C/C++ Defines
    add_defines("__PTW32_STATIC_LIB",
                "_CRT_SECURE_NO_WARNINGS",
                "FMT_HEADER_ONLY")

    if is_mode("debug") then
        add_defines("_DEBUG", "GPCS4_DEBUG")
    end

    -- Include and Souce Files
    on_load(function (target)
        import("xmake.llvm")
        target:add("includedirs", llvm.find_include())
    end)

    add_includedirs("$(env VULKAN_SDK)/Include")
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

    -- linked libraries
    if is_os("windows") then
        add_linkdirs("$(env VULKAN_SDK)/Lib")
        add_links("vulkan-1.lib")
        add_syslinks("ksuser.lib",
                     "mfplat.lib",
                     "mfuuid.lib",
                     "wmcodecdspuuid.lib",
                     "legacy_stdio_definitions.lib",
                     "user32.lib")
    else
        print("TODO")
    end

    add_deps("glfw")
    add_deps("winpthreads")
    add_deps("rtaudio")
    add_deps("zydis")
    add_deps("tinydbr")