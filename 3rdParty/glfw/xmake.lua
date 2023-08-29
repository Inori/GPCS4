target("glfw")
    set_kind("static")
    set_group("3rdParty")

    add_defines("_CRT_SECURE_NO_WARNINGS",
                "_GLFW_USE_CONFIG_H")

    add_headerfiles("glfw3.h", "glfw3native.h")
    add_filegroups("include", {rootdir = "./", files = {"*.h"}})

    add_packages("vulkansdk")

    if is_plat("windows") then
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

        add_syslinks("user32", "shell32", "gdi32")
    else
        print("glfw TODO")
        -- TODO: add wayland support
        add_deps("libx11", "libxrandr", "libxrender", "libxinerama", "libxfixes", "libxcursor", "libxi", "libxext")
        add_syslinks("dl", "pthread")
        add_defines("_GLFW_X11")
    end

    -- add_filegroups("include", {rootdir = "./", files = {"src/*.h"}})