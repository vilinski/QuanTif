// QuanTif.x64.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
extern "C" double CallPrice(double principal, double strike, double rate, double volatility, double timeToMaturity);
extern "C" double P1(double x);

double P1(double x) { return log(x); }
double CallPriceRep(double principal, double strike, double rate, double volatility, double timeToMaturity, int n) {
	for (int i = 1; i < n; i++) CallPrice(100., 100., 0.05, 0.2, 1.);
	return CallPrice(100., 100., 0.05, 0.2, 1.);
}
int main()
{
	//int i = 1;
	//log(2.0);
	double cp =  CallPrice(100., 100., 0.05, 0.2, 1.);
	//for (int i = 0; i < 10000000; i++) CallPrice(100., 100., 0.05, 0.2, 1.);
}
