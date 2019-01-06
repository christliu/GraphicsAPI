#include <stdio.h>
/*
#include "WindowBaseApplication.h"


WindowBaseApplication winApp;

int main()
{
	if(!winApp.Initialize())
		return -1;

	while(!winApp.IsQuit())
	{
		winApp.Tick();
	}
	winApp.Finalize();
	return 0;
}
*/

#include "WindowsApplication.h"
#include "DirectX11Application.h"

BaseApplication *g_pApp = reinterpret_cast<BaseApplication*>(new DirectX11Application());


int main()
{
	if (!g_pApp->Initialize())
		return -1;

	while(!g_pApp->IsQuit())
	{
		g_pApp->Tick();
	}
	g_pApp->Finalize();
	return 0;
}