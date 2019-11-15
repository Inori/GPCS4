# GPCS4

A Playstation 4 emulator just begin.  
A project for fun, and technical research.  

The only reason I start this project is learning 3D graphics, nothing serious.

## Latest progress

![Basic Quad](https://github.com/Inori/GPCS4/blob/master/Misc/basic_quad.gif)  
First correct image rendered, although this is a 2D image demo, it's actuall rendered in 3D pipeline.  
There're still some hardcode test code, but with more design, and most important, this time with our shader translator, no hand write shaders anymore.  
Next I'll focus on Nier and improve the graphics implementation.

![Demo](https://github.com/Inori/GPCS4/blob/master/Misc/nier_logo.gif)  
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

## Credits
[DXVK](https://github.com/doitsujin/dxvk)  
[uplift](https://github.com/idc/uplift)

## License
[GPL-3.0](https://github.com/Inori/GPCS4/blob/master/LICENSE)  

## _Apology_  
There is another ps4 emulator project call ps4uv, which is close source and private.
For some historical reason I got their code, and because of my careless, I let one of our developer see the code without their permission. I apologize for what I've done sincerely.  
There're some points of details of our linker code are borrowed from ps4uv's linker module's idea, and I've deleted our linker branch, and after we delete those points and reform our code, I'll make it online again, never mean to stole anyone's code.  
Apologize again. Asuka




