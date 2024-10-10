#include <iostream>
#include "Window.h"
#include "include/Aiden_Log/a_log.h"


int main()
{
	Log::Clear();
    Window window("Application", { 500, 500 });


	while (window.isRun())
	{
		window.broadcast();

		if (!window.isRun()) { return 1; }

		window.onNewFrame();

		// Render Here

		window.onUpdate();

		Sleep(0);
	}
}
