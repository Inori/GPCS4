import("core.project.project")
import("lib.detect.find_path")

function _locate_clang_cl()
    if os.is_host("windows") then
        local pathes = path.splitenv(vformat("$(env PATH)"))
        local clang_path = find_path("clang-cl.exe", pathes)
        return clang_path
    else
        print("non-windows build not supported yet.")
        return nil
    end
end

function find_include()
    local clang_path = _locate_clang_cl()
    if clang_path then
        local llvm_path = path.directory(clang_path)
        local lib_path = path.join(llvm_path, "lib", "clang")
        local dir_table = os.dirs(lib_path .. "/*")
        local include_parent = dir_table[1]
        local include_path = path.join(include_parent, "include")
        return include_path
    else
        return nil
    end
end