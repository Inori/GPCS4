add_requires("glslang", {configs = {binaryonly = true}})

add_requires("vulkansdk")

if is_mode("release") and is_plat("windows") then
    add_requires("vc-ltl5")
end
