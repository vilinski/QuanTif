#pragma once

using namespace System;

namespace QuanTifCppCLI {
	public ref class BlackScholesCppCLI
	{
	public:
		double CallPrice(double principal, double strike, double rate, double volatility, double timeToMaturity);
		double CallPriceStress(double principal, double strike, double rate, double volatility, double timeToMaturity, int n);
	};
}
