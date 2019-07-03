#include "sce_libc.h"
#include <cmath>

// declarations in this file can be found in FreeBSD 9, ymath.h


int PS4API scec_tan(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_tanf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_tanh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec__Log(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec__Cosh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec__FDtest(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


float PS4API scec__FLog(float arg, int n)
{
	LOG_SCE_TRACE("arg %f", arg);
	return logf(arg);
}


int PS4API scec__FSin(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


double PS4API scec__Sin(double arg, unsigned int m, int n)
{
	LOG_SCE_TRACE("arg %f", arg);
	return sin(arg);
}


int PS4API scec__Sinh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_acos(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_acosf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_acosh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_asin(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_asinf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_asinh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_atan(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_atan2(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_atan2f(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_atanh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_div(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_erf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_erfc(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_exp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_exp2(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_exp2f(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_expf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fmod(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_fmodf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_modf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_modff(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_pow(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_powf(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}

int PS4API scec_hypot(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_ldexp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_frexp(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


int PS4API scec_cbrt(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}
