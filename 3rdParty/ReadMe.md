3rd party liibraries.
Make sure every library folder contains it's required header files and compiled libs,
which can be include like:
#include "pthreads4w/pthread.h"

and can be linked like in ImportLib.cpp, like:
#pragma comment(lib, "../3rdParty/pthreads4w/libpthreadVCE3d.lib")
