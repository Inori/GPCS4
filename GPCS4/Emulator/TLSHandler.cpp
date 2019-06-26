#include "TLSHandler.h"

CTLSHandler::CTLSHandler()
{
}

CTLSHandler::~CTLSHandler()
{
}



//////////////////////////////////////////////////////////////////////////
#ifdef GPCS4_WINDOWS

CTLSHandlerWin::CTLSHandlerWin()
{
}

CTLSHandlerWin::~CTLSHandlerWin()
{
}



#else


CTLSHandlerLinux::CTLSHandlerLinux()
{
}

CTLSHandlerLinux::~CTLSHandlerLinux()
{
}


#endif  //GPCS4_WINDOWS

