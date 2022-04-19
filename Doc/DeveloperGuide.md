# Developer Guide for GPCS4

## Branches
1. `master` branch is under heavy develpment, mainly for graphics, so the code maybe not stable for test. If you aim to do graphics development, checkout this branch.
2. `no_graphics` branch is used for non-graphics development. It will emulate GPU interrupts and label write, preventing program from trapping into infinite loop waiting for GPU events.
3. If you just want to do some simple graphics test, checkout the latest `tag` branch.

## Build GPCS4:
Install prerequisite:

1. Visual Studio 2019 / 2022
2. [LunarG Vulkan SDK](https://vulkan.lunarg.com/)

Start Visual Studio, select `Tools`->`Get Tools and Features...`, open the Installer window.  
Make sure `Desktop development with C++` workloads is installed.  
Then open `Individual components` tab, install `C++ Clang-cl for v14x build tools (x64/x86)` and `C++ Clang Compiler for Windows (xx.x.x)`.  
If you can't find these two components, update your Visual Studio version.

Build steps:
1. Checkout a proper branch.
2. Close your Visual Studio first, install Vulkan SDK listed above.
3. Build `pthreads4w` manually:  
   Open `x64 Native Tools Command Prompt for VS 2019`,  
   cd to path: `GPCS4\3rdParty\pthreads4w`  
   type `nmake VC-static-debug` then `nmake VC-static`  
4. If you are using VS2019, you need to fix clang compatible bug (VS2022 with Clang 13 not needed):  
   Open `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\Llvm\x64\lib\clang\12.0.0\include\stdatomic.h`  
   search for  
   `void atomic_thread_fence(memory_order);`  
   `void atomic_signal_fence(memory_order);`  
   then comment these two lines
5. Restart Visual Studio, build 3rdParty libraries one by one, then GPCS4.
6. If you still can't build, try to change clang or vulkan sdk version. My clang version is 13.0.0 and vulkan sdk version is 1.3.204.1. Other versions are not tested.
   
   TODO:  
   Step 3 needs to be fixed.  
   We should be able to build `pthreads4w` directly within Visual Studio. The environment variable is different between cmd console and visual studio.  
 
## Run demos/games:
Currently, GPCS4 need a path of main elf/bin as input parameter, and will redirect all `app0` access to the `Current Working Directory`.  
ie. `/app0/shader_vv.sb` to `E:\Code\GPCS4\Debug\shader_vv.sb`  

Follow the steps:
1. Checkout a proper branch, the master branch is under heavy development and maybe not stable for test.
2. Extract `lib` from [your download](https://pastebin.com/bUxckm3y) to the folder where your game is located.
3. Open properties window of GPCS4 project in Visual Studio, switch to Debugging tab,  
set `Working Directory` to your game's folder.  
set `Command Arguments` according to the help message, ie. `-E eboot.bin`.   
Note you don't need to specify the full path because the `Working Directory` has been set to the folder the elf/bin locate.
4. Build the `x64 Debug` version and press F5.

Note, if you want to test the graphics part, currently I've not implemented constant buffers, so you need to 
`#define SCE_GNMX_ENABLE_GFX_LCUE 1` before building any demos.

## For short term bug fix & feature add:
Read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md), then just make a pull request, we'll see.


## For long term participation:

***Make sure you have enough free time to do this job. Don't just have a brief period of enthusiasm. This is the most important part.***

Note, before you write any code, read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md) carefully, code which do not follow the style will not be accepted.  
Also, you can take the existing code as examples.

Follow the steps:
1. Build GPCS4 following the above steps.
2. Download all the files here, these are all you need to develop GPCS4.  
    https://pastebin.com/bUxckm3y
3. Read the roadmap on the main page, select a part which you're insterested in. Basically, you have 3 ways to get started with the project:  
     - Pick an issue from github's issues page.  
     - Search for a TODO in the source files.  
     - Or If you are skilled enough, try to run a game or demo you like, see why it doesn't work or missing any features, then focus on what you're really interested in.  
4. Then you know what to do next.

