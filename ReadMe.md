# GPCS4

A Playstation 4 emulator which was just started.  
A project done for fun, and for technical research.  

The only reason I have started this project is for learning 3D graphics; I don't intend
it to become a serious application.

## Latest progress

![WAD Demo](https://github.com/Inori/ImageHost/blob/master/GPCS4/wad_demo.gif)  
First commercial game support, *We are Doomed*. Thanks to our developers.  
It's not playable now, since the FPS is very low, because there are currently no optimizations.  
GPCS4 will recompile all the shaders, untiling all the textures and upload all the buffers every frame, making it less efficient.  
Besides, there are still some problems in the game pad emulation.  
We will try to resolve these issues incrementally.

![Basic Quad](https://github.com/Inori/ImageHost/blob/master/GPCS4/basic_quad.gif)  
First correct image rendered, although this is a 2D image demo, it's actually rendered in a 3D pipeline.  
There is still some hard-coded test code, but with more design, and most importantly, using our shader translator. There won't be any hand-written shaders anymore.  
Next I'll focus on Nier and improve the graphics implementation.

![Nier Demo](https://github.com/Inori/ImageHost/blob/master/GPCS4/nier_logo.gif)  
This is a previous running result of Nier:Automata. It is not correct yet, and I'll try to improve it.

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
        > Basic implementation.
    * Gamepad
        > Basic implementation.

* ### System
    * Dynamic linker
        > TLS support is not complete yet, and possibly neither are some other initialization functions.
    * System resource management, ie. Thread, virtual memory, synchronize object, file, clock.
        > For functions which do not require system resource (e.g: math functions),  
        we'll use the system libraries directly.  
        For others, we will try to implement them, one by one.

## Join

Read our [Developer's Guide](https://github.com/Inori/GPCS4/blob/master/Doc/DeveloperGuide.md)

For more details you could join our discord server to contact me, or if you are used to Tencent QQ/TIM, just join our group. There you can get more resources which are useful for contributing.

Discord: [GPCS4](https://discord.gg/PXrUxtk)  
Tencent QQ/TIM Group Number: <a target="_blank" href="//shang.qq.com/wpa/qunwpa?idkey=aacc9b682acbb7e7463c256cb76ce3fb0f7ba8a584d1561e7eeffe227e14e602">470865218</a>



## Build
To run or develop GPCS4, a CPU supporting AVX instruction set as well as a graphics card supporting Vulkan are required.  
Currently, only Windows build is supported.  

Visual Studio 2017  
[LLVM/Clang](https://releases.llvm.org/9.0.0/LLVM-9.0.0-win64.exe)  
[LLVM Compiler Toolchain Extension](https://marketplace.visualstudio.com/items?itemName=LLVMExtensions.llvm-toolchain)  
[LunarG Vulkan SDK](https://vulkan.lunarg.com/)

For more details, see the [Developer's Guide](https://github.com/Inori/GPCS4/blob/master/Doc/DeveloperGuide.md)
## Credits
[DXVK](https://github.com/doitsujin/dxvk)  
[uplift](https://github.com/idc/uplift)

## License
[GPL-3.0](https://github.com/Inori/GPCS4/blob/master/LICENSE)  

