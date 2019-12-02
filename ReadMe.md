# GPCS4

A Playstation 4 emulator just begin.  
A project for fun, and technical research.  

The only reason I start this project is learning 3D graphics, nothing serious.

## Latest progress

![Basic Quad](https://github.com/Inori/ImageHost/blob/master/GPCS4/basic_quad.gif)  
First correct image rendered, although this is a 2D image demo, it's actuall rendered in 3D pipeline.  
There're still some hardcode test code, but with more design, and most important, this time with our shader translator, no hand write shaders anymore.  
Next I'll focus on Nier and improve the graphics implementation.

![Demo](https://github.com/Inori/ImageHost/blob/master/GPCS4/nier_logo.gif)  
This is a privious running result of Nier:Automata, not correct though, I'll improve it.

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

Read our [Developer Guide](https://github.com/Inori/GPCS4/blob/master/Doc/DeveloperGuide.md)

For more details you could join our discord server to contact with me, or if you are used to Tencent QQ/TIM, just join our group. Here you can get more resources which are very usefull for developing.

Discord: [GPCS4](https://discord.gg/PXrUxtk)  
Tencent QQ/TIM Group Number: <a target="_blank" href="//shang.qq.com/wpa/qunwpa?idkey=aacc9b682acbb7e7463c256cb76ce3fb0f7ba8a584d1561e7eeffe227e14e602">470865218</a>



## Build
To run or develop GPCS4, a CPU supporting AVX512 together with a graphics card supporting Vulkan are required.  
Currently, only Windows build is supported.  

Visual Studio 2017  
[LLVM/Clang](http://releases.llvm.org/9.0.0/LLVM-9.0.0-win64.exe)  
[LLVM Compiler Toolchain Extension](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain)  
[LunarG Vulkan SDK](https://vulkan.lunarg.com/)

For more details, see [Developer Guide](https://github.com/Inori/GPCS4/blob/master/Doc/DeveloperGuide.md)
## Credits
[DXVK](https://github.com/doitsujin/dxvk)  
[uplift](https://github.com/idc/uplift)

## License
[GPL-3.0](https://github.com/Inori/GPCS4/blob/master/LICENSE)  





