# GPCS4 Graphics Stack Design

        +-------------+
        |     Game    |
        +-------------+
               |           
               v
    +---------------------------+
    | SceModules::sce_gnmdriver |
    +---------------------------+
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
        +------------+      +------------+
        |     Gve    | <--- |    Pssl    |
        +------------+      +------------+
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

#### 2. SceModules::sce_gnmdriver
This is the interface between the game and GPCS4.  
Apis in this module are all the game's import functions. That is to say, the function pointer will be filled directly into the game's import table, and the game will call these apis directly.

This module will only call functions in `Sce` module.

#### 3. Sce
This is the top level graphics hardware abstraction.  
`SceVideoOut` is abstract for display/television hardware for a real PS4 hardware. For GPCS4, it abstract for the window.  
`SceGnmDriver` is the graphics driver module of PS4 system. For GPCS4, it abstract for graphics driver and GPU too.

This module will only call functions in `Gnm` module.

#### 4. Gnm
This module is responsible for Gnm reverse engineering and convert Gnm calls into Gve calls.  
It will parse command buffers (PM4 packet queue) received from the game, recover the original Gnm calls. (`GnmCmdStream`)  
Together with reverser engineering Gnm structures and converting Gnm enumerations/contants into Gve/Vulkan's format.(`cvt namespace`)  
Finally, it will call Gve functions, translating Gnm calls into Gve calls.(`GnmCommandBufferDraw`)  

This module will only call functions in `Gve` module.

#### 5. Gve
This the vulkan engine.  
Gve means "Gnm to Vulkan tranlation engine".  
It's the abstraction layer of vulkan. Most of vulkan objects will be wrapped into Gve classes for convenience.  
And it will maintain vulkan states, pipelines, optimizations and so on.

#### 6. Pssl
This is the shader translator/recompiler.
PS4 use AMD's GCN bytecode in its' shader binary.  
We need to translate these GCN bytecode into vulkan's Spir-V bytecode.  
Translating result will be used by `Gve` module.

#### 7. Vulkan
This is the graphics api which GPCS4 uses.  
Not much to say.

#### 8. GPU Hardware
This is the lowest hardware GPU.  
Not much to say.