# Developer Guide for GPCS4

## Branches
1. `master` branch is under heavy develpment, mainly for graphics, so the code maybe not stable for test. If you aim to do graphics development, checkout this branch.
2. `no_graphics` branch is used for non-graphics development. It will emulate GPU interrupts and label write, preventing program from trapping into infinite loop waiting for GPU events.
3. If you just want to do some simple graphics test, checkout the latest `tag` branch.

## Build GPCS4:
Follow the steps:
1. Checkout a proper branch.
2. Close your Visual Studio first, install the 3 components listed on the github page.
3. Restart Visual Studio, select `X64 Debug` build. Build 3rdParty libraries first, then GPCS4.
4. If you still can't build, try to change clang or vulkan sdk version. My clang version is 9.0.0.0 and vulkan sdk version is 1.1.121.2. Other versions are not tested.


## Run demos/games:
Currently, GPCS4 need a path of main elf/bin as input parameter, and will redirect all `app0` access to the folder where GPCS4.exe locate.  
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
     - Pick a TODO from [Trello](https://trello.com/b/4kZu97F5/gpcs4) cards.  
     - Or If you are skilled enough, try to run a game or demo you like, see why it doesn't work or missing any features, then focus on what you're really interested in.  
4. Then you know what to do next.

