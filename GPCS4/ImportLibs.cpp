#include "GPCS4Config.h"

// links to static libs
//
// I don't do this in property settings page
// because I found it doesn't support relative path.
// e.g. "pthreads4w/libpthreadVCE3d.lib"
// thus I'll have to add a lib directory in that settings page
// every time I add a new lib. that's awful.


#ifdef GPCS4_WINDOWS

#pragma comment(lib, "../3rdParty/SDL2/lib/x64/SDL2main.lib")
#pragma comment(lib, "../3rdParty/SDL2/lib/x64/SDL2.lib")

#ifdef _DEBUG

#pragma comment(lib, "../3rdParty/pthreads4w/libpthreadVC3d.lib")
#pragma comment(lib, "../3rdParty/zydis/Zydisd.lib")
#pragma comment(lib, "../3rdParty/glfw/glfw3d.lib")

#else

#pragma comment(lib, "../3rdParty/pthreads4w/libpthreadVC3.lib")
#pragma comment(lib, "../3rdParty/zydis/Zydis.lib")
#pragma comment(lib, "../3rdParty/glfw/glfw3.lib")

#endif  //_DEBUG
#endif  //GPCS4_WINDOWS
