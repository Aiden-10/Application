#pragma once

// Define WIN32_LEAN_AND_MEAN to exclude unnecessary headers
#define _WINSOCKAPI_   


#include <Windows.h>

#include <string>
#include <d3d11.h>

#include "include/Texture/WICTextureLoader.h"

#include "include/ImGui/imgui.h"
#include "include/ImGui/imgui_impl_win32.h"
#include "include/ImGui/imgui_impl_dx11.h"

#include "include/Aiden_Log/a_log.h"

class Window
{
public:
	Window(const char name[24], POINT size);

	bool broadcast();

	bool release();

	bool isRun();

	void onResize(UINT width, UINT height);

	void onDestroy();

	void onUpdate();

	void onNewFrame();

	POINT GetWindowSize();

	HRESULT LoadTextureFromFile(const std::wstring& fileName, ID3D11ShaderResourceView** textureView);

	~Window();

private:
	POINT size;
	HWND m_hwnd;
	bool b_is_run;
	ID3D11Device* device{ nullptr };
	ID3D11DeviceContext* device_context{ nullptr };
	IDXGISwapChain* swap_chain{ nullptr };
	ID3D11RenderTargetView* render_target_view{ nullptr };
	D3D_FEATURE_LEVEL level{};
};