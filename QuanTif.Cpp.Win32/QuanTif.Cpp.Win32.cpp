// QuanTif.Cpp.Win32.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


extern "C" int __declspec(dllexport) Pippo()
{
	return 2;
}

extern "C" double CombineA(int a, int b, int c, int d, int e, double f);

extern "C" double __declspec(dllexport) Exp_Asm(double x)
{
	//__asm {
	//	fld x
	//	fldl2e
	//	fmulp   st(1), st(0)
	//	fld1
	//	fld     st(1)
	//	fprem
	//	f2xm1
	//	faddp   st(1), st(0)
	//	fscale
	//	fstp    st(1)
	//}
	//__asm {
	//	fld x;
	//	call exp1;
	//	jmp end1;
	//exp1 :
	//	fldl2e
	//	fmulp st(1), st(0)
	//	fld1
	//	fld st(1)
	//	fprem
	//	f2xm1
	//	faddp st(1), st(0)
	//	fscale
	//	fstp st(1)
	//	ret;
	//end1:
	//}

	return CombineA(1, 2, 3, 4, 5, 6.1);
}