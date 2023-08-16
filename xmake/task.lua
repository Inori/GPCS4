task("test")
    on_run(function ()
        os.exec("xmake f -m debug --test=y")
        os.exec("xmake build -g test")
        os.exec("xmake run -g test")
    end)

    set_menu{}
