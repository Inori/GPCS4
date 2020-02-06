#include "sce_libc.h"
#include <cmath>

LOG_CHANNEL(SceModules.SceLibc.math);

// declarations in this file can be found in FreeBSD 9, math.h and ymath.h


double PS4API scec_tan(double arg)
{
	LOG_SCE_TRACE("arg %f", arg);
	return tan(arg);
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


float PS4API scec__FSin(float arg, unsigned int m, int n)
{
	LOG_SCE_TRACE("arg %f", arg);
	return sinf(arg);
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


float PS4API scec_acosf(float x)
{
	LOG_SCE_TRACE("x %f", x);
	return acosf(x);
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


float PS4API scec_asinf(float x)
{
	LOG_SCE_TRACE("x %f", x);
	return asinf(x);
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


float PS4API scec_atan2f(float y, float x)
{
	LOG_SCE_TRACE("y %f x %f", y, x);
	return atan2f(y, x);
}


int PS4API scec_atanh(void)
{
	LOG_FIXME("Not implemented");
	return SCE_OK;
}


sce_div_t PS4API scec_div(int numerator, int denominator)
{
	LOG_SCE_TRACE("numerator %d denominator %d", numerator, denominator);
	div_t res = div(numerator, denominator);
	sce_div_t ret;
	ret.quot = res.quot;
	ret.rem = res.rem;
	return ret;
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


double PS4API scec_exp2(double arg)
{
	LOG_SCE_TRACE("arg %f", arg);
	return exp2(arg);
}


float PS4API scec_exp2f(float arg)
{
	LOG_SCE_TRACE("arg %f", arg);
	return exp2f(arg);
}


float PS4API scec_expf(float x)
{
	LOG_SCE_TRACE("x %f", x);
	return expf(x);
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


float PS4API scec_powf(float x, float y)
{
	LOG_SCE_TRACE("x %f y %f", x, y);
	return powf(x, y);
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
