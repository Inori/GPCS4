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
        add_defines("_GLFW_WIN32")
    else
        add_files("src/context.c",
                  "src/egl_context.c",
                  "src/init.c",
                  "src/input.c",
                  "src/monitor.c",
                  "src/osmesa_context.c",
                  "src/vulkan.c",
                  "src/glx_context.c",
                  "src/x11_init.c",
                  "src/linux_joystick.c",
                  "src/x11_monitor.c",
                  "src/xkb_unicode.c",
                  "src/posix_thread.c",
                  "src/posix_time.c",
                  "src/x11_window.c",
                  "src/window.c")
        -- TODO: add wayland support
        add_syslinks("X11", "Xrandr", "dl", "pthread")
        add_defines("_GLFW_X11")
    end

    -- add_filegroups("include", {rootdir = "./", files = {"src/*.h"}})
