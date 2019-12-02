# Developer Guide for GPCS4

## Build GPCS4:
Follow the steps:
1. Checkout any release tag, do not checkout latest master branch.
2. Close your Visual Studio first, install the 3 components listed on the github page.
3. Restart Visual Studio, select `X64 Debug` build. Build 3rdParty libraries first, then GPCS4.
4. If you still can't build, try to change clang or vulkan sdk version. My clang version is 9.0.0.0 and vulkan sdk version is 1.1.121.2. Other versions are not tested.


## Run demos/games:
Currently, GPCS4 need a path of main elf/bin as input parameter, and will redirect all `app0` access to the folder where GPCS4.exe locate.  
ie. `/app0/shader_vv.sb` to `E:\Code\GPCS4\Debug\shader_vv.sb`  

Follow the steps:
1. Choose a proper branch, the master branch is under heavy development and maybe not stable for test.
2. Copy all the stuffs of a demo/game, including main elf/bin, shader binary file, texture and so on, in the `Debug` folder where GPCS4.exe output, and keep it's directory tree structure unchanged.
3. Open properties window of GPCS4 project in Visual Studio, switch to Debugging tab,  
set `Woring Directory` to `$(OutDir)`  
set `Command Arguments` to the name of the main elf/bin, ie. `basic_quad.elf`.   
Note you don't need to specify the full path because the `Woring Directory` has been set to the folder the elf/bin locate.
4. Build the `x64 Debug` version and press F5.

Note, if you want to test the graphics part, currently I've not implemented constant buffers, so you need to 
`#define SCE_GNMX_ENABLE_GFX_LCUE 1` before building any demos.

## For short term bug fix & feature add:
Read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md), then just make a pull request, we'll see.


## For long term participation:

***Make sure you have enough free time to do this job. Don't just have a brief period of enthusiasm. This is the most important part.***

Note, before you write any code, read our [code style document](https://github.com/Inori/GPCS4/blob/master/Doc/CodeStyle.md) carefully, code which do not follow the style will not be accepted.  
Also, you can take the existing code as example.

Follow the steps:
1. Read the roadmap on the main page, select a part which you're insterested in. Or if you're not sure, see our [Trello](https://trello.com/b/4kZu97F5/gpcs4) and take a job from `TODO list for new developers` card.
2. Build GPCS4 following the above steps.
3. Download all the files here, these are all you need to develop GPCS4.  
    https://mega.nz/#F!8ORCVQgY  
    PW : qp7HFxj8awgIBP_L5K7c3g
4. Run the demo or game you like, find why it crashes/doesn't work.
5. Then you know what to do next.

