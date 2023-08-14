set_project("GPCS4")
set_xmakever("2.8.1")

add_rules("mode.release", "mode.debug")

target("GPCS4")
    set_kind("binary")
    set_languages("cxx17")
    set_plat(os.host())
    set_arch("x64")
    set_toolchains("clang-cl")

    if is_mode("debug") then
        set_runtimes("MTd")
    else
        set_runtimes("MT")
    end

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
    add_linkdirs("$(env VULKAN_SDK)/Lib")
    add_links("vulkan-1.lib")
    add_syslinks("ksuser.lib",
                 "mfplat.lib",
                 "mfuuid.lib",
                 "wmcodecdspuuid.lib",
                 "legacy_stdio_definitions.lib")