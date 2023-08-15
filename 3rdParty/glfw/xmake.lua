target("glfw")
    set_kind("static")
    set_group("3rdParty")

    add_defines("_CRT_SECURE_NO_WARNINGS",
                "_GLFW_USE_CONFIG_H")

    add_headerfiles("glfw3.h", "glfw3native.h")
    add_filegroups("include", {rootdir = "./", files = {"*.h"}})

    if is_os("windows") then
        add_files("src/context.c",
                  "src/egl_context.c",
                  "src/init.c",
                  "src/input.c",
                  "src/monitor.c",
                  "src/osmesa_context.c",
                  "src/vulkan.c",
                  "src/wgl_context.c",
                  "src/win32_init.c",
                  "src/win32_joystick.c",
                  "src/win32_monitor.c",
                  "src/win32_thread.c",
                  "src/win32_time.c",
                  "src/win32_window.c",
                  "src/window.c")

        add_syslinks("Shell32.lib",
                     "Gdi32.lib")
    else
        print("TODO")
    end

    -- add_filegroups("include", {rootdir = "./", files = {"src/*.h"}})