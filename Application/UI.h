#pragma once
#include <iostream>

#include "Window.h"
#include "Mods/dayz.h"

#include "include/ImGui/imgui.h"
#include "include/ImGui/imgui_impl_win32.h"
#include "include/ImGui/imgui_impl_dx11.h"

class Window;

class UI
{
public:
	UI(Window& window)
		: windowRef(window),
		game("DayZ_x64.exe") 
	{
		game.start_mods();
	}

	void Render();

public:
	ID3D11ShaderResourceView* radarImage{ nullptr };
	ID3D11ShaderResourceView* icon_settings{ nullptr };

private:
	Window& windowRef;

	Game game;

	// ---------------- //
	// ------Radar----- //
	// ---------------- //

	void DrawRadar();
	void HandleRadar();

	float zoom = 0.4f;
	const float minZoom = 0.05f;
	const float maxZoom = 10.0f;
	bool radarHovered;
	bool isMouseDragging = false;
	ImVec2 windowSize;
	ImVec2 RadarMousePos;
	ImVec2 mouseDownPos;
	ImVec2 offset = { 0.f, 0.f };



	// ---------------- //
	// ------Fuser----- //
	// ---------------- //

	// void DrawFuser();


};