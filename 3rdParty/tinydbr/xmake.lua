target("tinydbr")
    set_kind("static")
    set_group("3rdParty")
    
    add_includedirs("./")
    add_includedirs("$(projectdir)/3rdParty/xbyak")
    add_includedirs("$(projectdir)/3rdParty/zydis/dependencies/zycore/include")
    add_includedirs("$(projectdir)/3rdParty/zydis/include")

    add_headerfiles("*.h", "arch/**.h")
    add_files("*.cpp", "arch/**.cpp")

    add_filegroups("src", {rootdir = "./"})

    if is_os("windows") then
        add_headerfiles("windows/*.h")
        add_files("windows/*.cpp")
    else
        print("tinydbr TODO")
    end

    add_deps("zydis")