<div align="center">
   <img width="640" src="https://github.com/Inori/GPCS4/blob/master/Misc/logo.png" alt="logo"></br>

----
# GPCS4
A PlayStation 4 emulator which was just started.  
A project done for fun, and for technical research.  


The only reason I have started this project is for learning 3D graphics.  
It's not intended to become a serious application.
</div>




## Latest progress

![Basic Compute](https://github.com/Inori/ImageHost/blob/master/GPCS4/basic_compute.gif)  
* Support simple compute shader, compute ring, cube maps.  
* Use Direct Input to emulate pad.  


![WAD Demo](https://github.com/Inori/ImageHost/blob/master/GPCS4/wad_demo.gif)  
First commercial game support, *We are Doomed*. Thanks to our developers.  
It's not playable now, since the FPS is very low, because there are currently no optimizations.  
GPCS4 will recompile all the shaders, untiling all the textures and upload all the buffers every frame, making it less efficient.  
Besides, there are still some problems in the game pad emulation.  
We will try to resolve these issues incrementally.


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
To run or develop GPCS4, a CPU supporting AVX instruction set as well as a graphics card supporting Vulkan 1.3 are required.  
Currently, only Windows build is supported.  

For more details, see the [Developer's Guide](https://github.com/Inori/GPCS4/blob/master/Doc/DeveloperGuide.md)
## Credits
[DXVK](https://github.com/doitsujin/dxvk)  
[uplift](https://github.com/idc/uplift)

## License
[GPL-3.0](https://github.com/Inori/GPCS4/blob/master/LICENSE)  

