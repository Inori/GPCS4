# Developer Guide for GPCS4

## Branches
1. `master` branch is under heavy develpment, mainly for graphics, so the code maybe not stable for test. If you aim to do graphics development, checkout this branch.
2. `dev` branch mainly focus on one specific game at a time, code is very unstable, may contain temporary test code or comment out blocks for any purpose. Do not use this branch if you are not on the same game.
3. To get the most stable code, checkout the latest `tag` branch.

## Build GPCS4:
Install prerequisite:

1. Visual Studio 2022
2. [LunarG Vulkan SDK](https://vulkan.lunarg.com/)

Start Visual Studio, select `Tools`->`Get Tools and Features...`, open the Installer window.  
Make sure `Desktop development with C++` workloads is installed.  
Then open `Individual components` tab, install `C++ Clang-cl for v143 build tools (x64/x86)` and `C++ Clang Compiler for Windows (13.0.0)`.  
If you can't find these two components, update your Visual Studio version.

Build steps:
1. Update your GPU driver to the latest.  
2. Checkout a proper branch.  
3. Close your Visual Studio first, install Vulkan SDK listed above.
4. Restart Visual Studio, build 3rdParty libraries one by one, then GPCS4.
5. If you still can't build, try to change clang or vulkan sdk version. My clang version is 16.0.6 and vulkan sdk version is 1.3.204.1. Other versions are not tested. Or see the the [action script](https://github.com/Inori/GPCS4/blob/master/.github/workflows/build-windows.yml)

 
## Run demos/games:
Currently, GPCS4 needs a path to the main elf/bin file as input parameter, and will redirect all `app0` access to the `Current Working Directory`.  
ie. `/app0/shader_vv.sb` to `E:\Code\GPCS4\Debug\shader_vv.sb`  

Follow the steps:
1. Checkout a proper branch, the master branch is under heavy development and maybe not stable for testing.
2. Extract `lib` from [your download](https://pastebin.com/bUxckm3y) to the folder where your game is located.
3. Open properties window of GPCS4 project in Visual Studio, switch to Debugging tab,  
set `Working Directory` to your game's folder.  
set `Command Arguments` according to the help message, ie. `-E eboot.bin`.   
Note you don't need to specify the full path because the `Working Directory` has been set to the folder that the elf/bin located.
4. Build the `x64 Debug` version and press F5.

Note, if you want to test the graphics part, currently I've not implemented constant buffers, so you need to 
`#define SCE_GNMX_ENABLE_GFX_LCUE 1` before building any demos.

There is a `GPCS4_NO_GRAPHICS` macro which will turn off all graphcis code, mainly used for developing non-graphics part.
But may need some simple fix now.

## For short term bug fixes & feature additions:
Read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md), then just make a pull request, we'll see.


## For long term participation:

***Make sure you have enough free time to do this job. Don't just have a brief period of enthusiasm. This is the most important part.***

Note, before you write any code, read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md) carefully, code which does not follow the style will not be accepted.  
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

