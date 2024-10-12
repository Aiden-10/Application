#include <iostream>
#include "Window.h"
#include "UI.h"
#include "include/Aiden_Log/a_log.h"


int main()
{
	Log::Clear();
    Window window("Application", { 1080, 1080 });
	UI ui(window);

	HRESULT hr = window.LoadTextureFromFile(L"assets\\maps\\example.png", &ui.radarImage);
	hr = window.LoadTextureFromFile(L"assets\\icons\\settings.png", &ui.icon_settings);

	while (window.isRun())
	{
		window.broadcast();

		if (!window.isRun()) { return 1; }

		window.onNewFrame();

		// Render Here
		ui.Render();

		window.onUpdate();

		Sleep(0);
	}
}
