#pragma once

#include "GPCS4Common.h"
#include "Singleton.h"


class CTLSHandler
{
public:
	CTLSHandler();
	virtual ~CTLSHandler();

private:

};


//////////////////////////////////////////////////////////////////////////
// Windows tls implementation


#ifdef GPCS4_WINDOWS

class CTLSHandlerWin : public CTLSHandler
{
public:
	CTLSHandlerWin();
	virtual ~CTLSHandlerWin();


private:

};


#else

class CTLSHandlerLinux : public CTLSHandler
{
public:
	CTLSHandlerLinux();
	virtual ~CTLSHandlerLinux();

private:

};

#endif  //GPCS4_WINDOWS
