rule("module.program")
    on_load(function (target)
        target:set("kind", "binary")
        target:set("rundir", "$(projectdir)")
        if target:is_plat("windows") and target:get("runtimes") == "MT" then
            target:add("packages", "vc-ltl5")
        end
    end)

    after_link(function (target)
        local enabled = target:extraconf("rules", "module.program", "upx")
        if (not enabled) or (not is_mode("release")) then
            return
        end

        import("core.project.depend")

        local targetfile = target:targetfile()
        depend.on_changed(function ()
            local file = path.join("build", path.filename(targetfile))
            local upx = assert(import("lib.detect.find_tool")("upx"), "upx not found!")

            os.tryrm(file)
            os.vrunv(upx.program, {targetfile, "-o", file})
        end, {files = targetfile})
    end)

rule("module.shader")
    set_extensions(".vert", ".tesc", ".tese", ".geom", ".comp", ".frag", ".comp", ".mesh", ".task", ".rgen", ".rint", ".rahit", ".rchit", ".rmiss", ".rcall", ".glsl")

    before_buildcmd_file(function (target, batchcmds, sourcefile, opt)
        import("lib.detect.find_tool")

        local glslc = assert(find_tool("glslc"), "glslc not found!")
        local outputdir = target:extraconf("rules", "module.shader", "outputdir") or path.join(target:autogendir(), "rules", "module.shader")
        local headerfile = path.join(outputdir, path.basename(sourcefile) .. ".h")

        batchcmds:show_progress(opt.progress, "${color.build.object}generating shader header %s", sourcefile)
        batchcmds:mkdir(outputdir)

        batchcmds:vrunv(glslc.program, {"-mfmt=num", "-o", path(headerfile), path(sourcefile)})

        -- add deps
        batchcmds:add_depfiles(sourcefile)
        batchcmds:set_depmtime(os.mtime(headerfile))
        batchcmds:set_depcache(target:dependfile(headerfile))
    end)
