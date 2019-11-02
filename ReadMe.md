# GPCS4

A Playstation 4 emulator just begin.  
A project for fun, and technical research.  

The only reason I start this project is learning 3D graphics, nothing serious.

## Latest progress

![Demo](https://github.com/Inori/GPCS4/blob/master/Misc/demo.gif)  

First texture rendered, [with hand write shaders](https://github.com/Inori/GPCS4/blob/master/Misc/tex.frag).  
The renderer code is just some test code without any design, and full of bugs, but I can get all the vertex buffers, textures and shader bytecode running so far.  
Next I will focus on the shader translator.

## Roadmap

* ### Graphics
    * [Gnm reverse engineering](https://github.com/Inori/GPCS4/tree/master/GPCS4/Graphic/Gnm)
        > Parsing PM4 command buffers and some other formats to recover the original Gnm calls.  
        Still a lot of work.
    * [Vulkan backend](https://github.com/Inori/GPCS4/tree/master/GPCS4/Graphic/Gve)
        > Translate Gnm calls into vulkan calls. Far from complete.
    * [Shader recompiler](https://github.com/Inori/GPCS4/tree/master/GPCS4/Graphic/Pssl)
        > Translate GCN bytecode into Spir-V bytecode. Far from complete.

* ### Hardware
    * Audio
        > Not started yet.
    * Gamepad
        > Not started yet.

* ### System
    * Dynamic linker
        > TLS support is not complete yet, maybe some other initialization functions either.
    * System resource management, ie. Thread, virtual memory, synchronize object, file, clock.
        > For functions which don not require system resource (ie. math functions),  
        we'll use the system libraries directly.  
        For others, we implement those one by one.

## Join
If you want to participate in the project, read the roadmap above and see which part you are insterested in.

After that, read our [code style](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md) document, and make a pull request while keeping the code style, I'll see it.  
Or if you're willing to work on the project for a pretty long time, I could make a branch for you.

For more details you could join our discord server to contact with me, or if you are used to Tencent QQ/TIM, just join our group. Here you can get more resources which are very usefull for developing.

Discord: [GPCS4](https://discord.gg/PXrUxtk)  
Tencent QQ/TIM Group Number: 470865218

## Build
Currently, only Windows build is supported.  

Visual Studio 2017  
[LLVM/Clang](https://llvm.org/)  
[LLVM Compiler Toolchain Extension](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain)  
[LunarG Vulkan SDK](https://www.lunarg.com/vulkan-sdk/)

## License
[GPL-3.0](https://github.com/Inori/GPCS4/blob/master/LICENSE)  
_But commercial usage is NOT permitted._



