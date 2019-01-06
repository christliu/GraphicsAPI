#pragma once
#include "Base.h"

extern Base* g_pApp;

class Derived:public Base
{
public:
	Derived() {};
	void Run() { g_pApp->Draw(); };
};
