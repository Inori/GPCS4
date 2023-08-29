rule("debug.asan")
    on_load(function (target)
        if not is_mode("debug") then
            return
        end

        target:add("cxflags", "-fsanitize=address")
        target:add("mxflags", "-fsanitize=address")
        target:add("ldflags", "-fsanitize=address")
        target:add("shflags", "-fsanitize=address")

        if not target:get("symbols") then
            target:set("symbols", "debug")
        end

        local msvc = target:toolchain("msvc")
        if target:kind() == "binary" and msvc then
            import("lib.detect.find_tool")

            local cl = assert(find_tool("cl", {envs = msvc:runenvs()}), "cl not found!")
            target:add("runenvs", "Path", path.directory(cl.program))
        end
    end)
