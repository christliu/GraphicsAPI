#include <stdio.h>
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