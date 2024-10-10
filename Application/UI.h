#pragma once

#include "Window.h"

#include "include/ImGui/imgui.h"
#include "include/ImGui/imgui_impl_win32.h"
#include "include/ImGui/imgui_impl_dx11.h"

class Window;

class UI
{
public:
	UI(Window& window) : windowRef(window)
	{
	}

	void Render();

private:
	Window& windowRef;
};