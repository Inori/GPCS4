target("zycore")
    set_kind("static")
    set_group("3rdParty")

    add_defines("ZYCORE_STATIC_BUILD")
    add_includedirs("dependencies/zycore/include")

    add_headerfiles("dependencies/zycore/include/Zycore/**.h")
    add_files("dependencies/zycore/src/**.c")
    add_filegroups("src", {rootdir = "dependencies/zycore/src/"})
    add_filegroups("include", {rootdir = "dependencies/zycore/include/Zycore/"})

    if is_os("windows") then
        add_defines("WINVER=0x0502",
                    "_WIN32_WINNT=0x0502",
                    "NTDDI_VERSION=0x05020000",
                    "_CRT_SECURE_NO_WARNINGS")

        -- add_syslinks("Ole32.lib")
    else
        print("TODO")
    end

target("zydis")
    set_kind("static")
    set_group("3rdParty")

    add_cxxflags("-Wno-deprecated-declarations",
                 "-Wno-ignored-pragmas")

    add_defines("ZYCORE_STATIC_BUILD",
                "ZYDIS_STATIC_BUILD")
 
    add_includedirs("include")
    add_includedirs("dependencies/zycore/include")
    add_includedirs("src")

    add_headerfiles("include/Zydis/**.h")
    add_files("src/**.c")
    add_filegroups("include", {rootdir = "include/Zydis/"})

    if is_os("windows") then
        add_defines("WINVER=0x0502",
                    "_WIN32_WINNT=0x0502",
                    "NTDDI_VERSION=0x05020000",
                    "_CRT_SECURE_NO_WARNINGS")

        -- add_syslinks("Ole32.lib")
    else
        print("TODO")
    end

    add_deps("zycore")