# GPCS4 Graphics Stack Design

        +-------------+
        |     Game    |
        +-------------+
               |           
               v
    +--------------------------+
    | SceModules::SceGnmDriver |
    +--------------------------+
               |           
               v
        +------------+
        |     Sce    |
        +------------+
               |           
               v
        +------------+
        |     Gnm    |
        +------------+
               |           
               v
        +------------+      +------------+      +-------------+
        |   Violet   | <--- |    Pssl    | <--- |    Spirv    |
        +------------+      +------------+      +-------------+
               |           
               v
        +------------+
        |   Vulkan   |
        +------------+
               |           
               v
      +-----------------+
      |   GPU Hardware  |
      +-----------------+

#### 1. Game
This is the start of the journey.  
Gnm libraries are all static libraries compiled into the game's main executable.  
So we won't receive any Gnm draw/state call directly from the game.  
Instead, we can only receive the command buffers during a submit call.

#### 2. SceModules::SceGnmDriver
This is the interface between the game and GPCS4.  
Apis in this module are all the game's import functions. That is to say, the function pointer will be filled directly into the game's import table, and the game will call these apis directly.

This module will only call functions in `Sce` module.

#### 3. Sce
This is the top level graphics hardware abstraction.  
`SceVideoOut` abstracts for display/television hardware for a real PS4 machine. For GPCS4, it abstracts for the window system.  
`SceGnmDriver` is the graphics driver module of PS4 system. For GPCS4, it abstracts for graphics driver and GPU too.

This module will only call functions in `Gnm` module.

#### 4. Gnm
This module is the reverse engineering and api translation module.  
It's responsible for three things:  
First, it will parse command buffers (PM4 packet queue) received from the game, recover the original Gnm calls. (`GnmCmdStream`).  
Second, it will reverser engineering Gnm structures and converting Gnm enumerations/constants into Viole/Vulkan's format.(`cvt namespace` and various structure definations.).    
Finally, it will call Viole functions, translating Gnm calls into Viole calls.(`GnmCommandBufferDraw`)  

This module will only call functions in `Viole` module.

#### 5. Viole
This is the vulkan engine.  
The name Violet comes from Violet Evergarden.  
It is the abstraction layer of vulkan. Most of vulkan objects will be wrapped into Violet classes for convenience.  
It will maintain vulkan render states, pipelines, optimizations and so on.

#### 6. Pssl
This is the shader translator/recompiler.  
PS4 use AMD's GCN bytecode in its' shader binary.  
We need to translate these GCN bytecode into vulkan's Spir-V bytecode.  
Translating result will be used by `Viole` module.  
Also, this module is responsible for parsing PSSL shader binary format.  

#### 7. Spirv
This is a helper module using to generate spir-v bytecode.  
It is used by `Pssl` module directly.

#### 8. Vulkan
This is the graphics api which GPCS4 uses.  
Not much to say.

#### 9. GPU Hardware
This is the lowest hardware GPU.  
Not much to say.