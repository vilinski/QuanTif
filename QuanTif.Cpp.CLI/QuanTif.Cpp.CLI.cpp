#include "stdafx.h"

#include "QuanTif.Cpp.CLI.h"

double callPrice(double principal, double strike, double rate, double volatility, double timeToMaturity);

namespace QuanTifCppCLI {
	double BlackScholesCppCLI::CallPrice(double principal, double strike, double rate, double volatility, double timeToMaturity) {
		return callPrice(principal, strike, rate, volatility, timeToMaturity);
	}

	double BlackScholesCppCLI::CallPriceStress(double principal, double strike, double rate, double volatility, double timeToMaturity, int n) {
		for (int i = 1; i < n; i++) callPrice(principal, strike, rate, volatility, timeToMaturity);
		return callPrice(principal, strike, rate, volatility, timeToMaturity);
	}
}