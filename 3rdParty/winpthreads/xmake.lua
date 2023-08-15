target("winpthreads")
    set_kind("static")
    set_group("3rdParty")

    add_cxxflags("-Wno-deprecated-declarations",
                 "-Wno-ignored-pragmas")

    add_includedirs("include")
    add_defines("__x86_64__")

    add_headerfiles("include/*.h")
    add_headerfiles("src/*.h")
    add_files("src/**.c|libgcc/*")