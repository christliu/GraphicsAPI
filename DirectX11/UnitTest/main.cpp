
#include <iostream>
#include "TestApp.h"
using namespace std;

Base* g_pApp = static_cast<Base*>(new TestApp());

int main()
{
	g_pApp->Run();
	system("pause");
	return 0;
}