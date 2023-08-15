target("rtaudio")
    set_kind("static")
    set_group("3rdParty")

    add_defines("RTAUDIO_EXPORT")
 
    add_headerfiles("*.h")
    add_files("*.cpp")
    add_filegroups("src", {rootdir = "./"})

    if is_os("windows") then
        add_defines("__WINDOWS_WASAPI__",
                    "_CRT_SECURE_NO_WARNINGS")

        add_syslinks("Ole32.lib")
    else
        print("rtaudio TODO")
    end