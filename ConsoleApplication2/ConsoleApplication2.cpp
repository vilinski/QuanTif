
#include "pch.h"
#include <iostream>

#define _USE_MATH_DEFINES
#define b0 0.2316419
#define b1 0.319381530
#define b2 -0.356563782
#define b3 1.781477937
#define b4 -1.821255978
#define b5 1.330274429
#define one_div_sqrt_of_2pi 0.3989422804

#include <iostream>
#include <cmath>
#include <tuple>

using namespace std;

#include "corecrt_math_defines.h"
#define EXP_A (1048576/M_LN2)
#define EXP_C 60801
inline double exponential(double y) {
	union {
		double d;
		struct { int j, i; } n;
	} _eco;
	_eco.n.i = (int)(EXP_A * (y)) + (1072693248 - EXP_C);
	_eco.n.j = 0;

	return _eco.d;
}

double normalPDF(double x) {
	//return one_div_sqrt_of_2pi * exp(-0.5 * x * x);
	return one_div_sqrt_of_2pi * exponential(-0.5 * x * x);
}

double normalCDF(double x) {
	if (x >= 0.0) {
		double t = 1. / (1. + b0 * x);
		double factor = t * (b1 + t * (b2 + t * (b3 + t * (b4 + t * b5))));
		double sigma = normalPDF(x);
		return 1.0 - sigma * factor;
	}
	else {
		return 1.0 - normalCDF(-x);
	}
}
tuple<double, double> d12(double principal, double strike, double rate, double volatility, double timeToMaturity) {
	double logSdivK = log(principal / strike);
	double sigmaSqrHalved = volatility * volatility * 0.5;
	double sigmaSqrT = volatility * pow(timeToMaturity, 0.5);
	return tuple<double, double>((logSdivK + (rate + sigmaSqrHalved) * timeToMaturity) / sigmaSqrT, (logSdivK + (rate - sigmaSqrHalved) * timeToMaturity) / sigmaSqrT);
}

double KexpMinusRT(double strike, double rate, double timeToMaturity) {
	//return strike * exp(-rate * timeToMaturity);
	double xp = exponential(-rate * timeToMaturity);
	return strike * exponential(-rate * timeToMaturity);
}

tuple<double, double> calcOptions(double principal, double strike, double rate, double volatility, double timeToMaturity, bool withPut) {
	tuple<double, double> _d12 = d12(principal, strike, rate, volatility, timeToMaturity);
	double nd1 = normalCDF(get<0>(_d12)), nd2 = normalCDF(get<1>(_d12));
	double _KexpMinusRT = KexpMinusRT(strike, rate, timeToMaturity);
	double callValue = principal * nd1 - _KexpMinusRT * nd2;
	if (withPut)
		return tuple<double, double>(callValue, _KexpMinusRT - principal + callValue);
	else
		return tuple<double, double>(callValue, 0.);
}

double callPrice(double principal, double strike, double rate, double volatility, double timeToMaturity) {
	return get<0>(calcOptions(principal, strike, rate, volatility, timeToMaturity, false));
}

double putPrice(double principal, double strike, double rate, double volatility, double timeToMaturity) {
	return get<1>(calcOptions(principal, strike, rate, volatility, timeToMaturity, true));
}

tuple<double, double> optionsPrice(double principal, double strike, double rate, double volatility, double timeToMaturity) {
	return calcOptions(principal, strike, rate, volatility, timeToMaturity, true);
}
#include <intrin.h>
//#include <immintrin.h>
//#include <emmintrin.h>
//#include <smmintrin.h>

double callPriceIntr1(double principal, double strike, double rate, double volatility, double timeToMaturity) {
	double logSdivK = log(principal / strike);
	__m128d vol = _mm_set1_pd(1.0);
	return 0.0;
}

/* max. rel. error = 1.72863156e-3 on [-87.33654, 88.72283] */
__m128 fast_exp_sse(__m128 x)
{
	__m128 t, f, e, p, r;
	__m128i i, j;
	__m128 l2e = _mm_set1_ps(1.442695041f);  /* log2(e) */
	__m128 c0 = _mm_set1_ps(0.3371894346f);
	__m128 c1 = _mm_set1_ps(0.657636276f);
	__m128 c2 = _mm_set1_ps(1.00172476f);

	/* exp(x) = 2^i * 2^f; i = floor (log2(e) * x), 0 <= f <= 1 */
	t = _mm_mul_ps(x, l2e);             /* t = log2(e) * x */
#ifdef __SSE4_1__
	e = _mm_floor_ps(t);                /* floor(t) */
	i = _mm_cvtps_epi32(e);             /* (int)floor(t) */
#else /* __SSE4_1__*/
	i = _mm_cvttps_epi32(t);            /* i = (int)t */
	j = _mm_srli_epi32(_mm_castps_si128(x), 31); /* signbit(t) */
	i = _mm_sub_epi32(i, j);            /* (int)t - signbit(t) */
	e = _mm_cvtepi32_ps(i);             /* floor(t) ~= (int)t - signbit(t) */
#endif /* __SSE4_1__*/
	f = _mm_sub_ps(t, e);               /* f = t - floor(t) */
	p = c0;                              /* c0 */
	p = _mm_mul_ps(p, f);               /* c0 * f */
	p = _mm_add_ps(p, c1);              /* c0 * f + c1 */
	p = _mm_mul_ps(p, f);               /* (c0 * f + c1) * f */
	p = _mm_add_ps(p, c2);              /* p = (c0 * f + c1) * f + c2 ~= 2^f */
	j = _mm_slli_epi32(i, 23);          /* i << 23 */
	r = _mm_castsi128_ps(_mm_add_epi32(j, _mm_castps_si128(p))); /* r = p * 2^i*/
	return r;
}
__m128i double_to_int64(__m128d x) {
	x = _mm_add_pd(x, _mm_set1_pd(0x0018000000000000));
	return _mm_sub_epi64(
		_mm_castpd_si128(x),
		_mm_castpd_si128(_mm_set1_pd(0x0018000000000000))
	);
}
int main()
{
	//__m128d x = _mm_set1_pd(-1.0);
	//__m128i i = double_to_int64(x);
	//__m128 x = _mm_set1_ps(1.f);
	//__m128 y = fast_exp_sse(x);
	//double e = exponential(1.0);
	//if (can_use_intel_knl_features())
	//	printf("This CPU supports AVX-512F+CD+ER+PF as introduced in Knights Landing\n");
	//else
	//	printf("This CPU does not support all Knights Landing AVX-512 features\n");
	//for (int i = 0; i < 10000000; i++) callPriceIntr1(100., 100., 0.05, 0.2, 1.);
	double cp = callPrice(100., 100., 0.05, 0.2, 1.);
}

