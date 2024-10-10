#include <dwmapi.h>
#include "Window.h"


Window* window = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return 0L;
	}

	Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

	switch (msg)
	{
	case WM_CREATE:
	{
		Window* window = (Window*)((LPCREATESTRUCT)lparam)->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)window);
		break;
	}

	case WM_SIZE:
	{
		if (window && wparam != SIZE_MINIMIZED) {
			window->onResize(LOWORD(lparam), HIWORD(lparam));
		}
		break;
	}

	case WM_DESTROY:
	{
		Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		::PostQuitMessage(0);
		window->onDestroy();
		break;
	}

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	return NULL;
}

Window::Window(const char name[24], POINT stuf)
{
	size.x = stuf.x;
	size.y = stuf.y;

	WNDCLASSEX wc = {};
	wc.cbClsExtra = NULL;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = NULL;
	wc.lpszClassName = "WindowClass";
	wc.lpszMenuName = "";
	wc.lpfnWndProc = &WndProc;

	if (!RegisterClassEx(&wc))
		LOGERROR("RegisterClassEx Failed");
	


	// Creation of the window
	m_hwnd = CreateWindowEx(NULL, "WindowClass", name, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y,
		NULL, NULL, NULL, this);

	LOG("Window Created!");


	if (!m_hwnd)
		LOGERROR("CreateWindowEx Failed");

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferDesc.RefreshRate.Numerator = 60U;
	sd.BufferDesc.RefreshRate.Denominator = 1U;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.SampleDesc.Count = 1U;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2U;
	sd.OutputWindow = m_hwnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	constexpr D3D_FEATURE_LEVEL levels[2]
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0
	};

	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0U,
		levels,
		2U,
		D3D11_SDK_VERSION,
		&sd,
		&swap_chain,
		&device,
		&level,
		&device_context
	);

	LOG("Device and Swap Chain Created!");

	ID3D11Texture2D* back_buffer{ nullptr };
	swap_chain->GetBuffer(0U, IID_PPV_ARGS(&back_buffer));

	if (back_buffer)
	{
		device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
		back_buffer->Release();
	}
	else LOGERROR("CreateRenderTargetView Failed");;

	// Show the Window
	ShowWindow(m_hwnd, SW_SHOW);
	UpdateWindow(m_hwnd);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Corbel.ttf", 14.0f);


	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hwnd);
	ImGui_ImplDX11_Init(device, device_context);

	// indicates the window is running and init
	b_is_run = true;
}

bool Window::broadcast()
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Sleep(0);

	return false;
}

bool Window::isRun()
{
	return b_is_run;
}

void Window::onResize(UINT width, UINT height) {
	size.x = width;
	size.y = height;

	if (device) {
		if (render_target_view) { render_target_view->Release(); render_target_view = nullptr; }
		swap_chain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
		ID3D11Texture2D* pBackBuffer;
		swap_chain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		device->CreateRenderTargetView(pBackBuffer, nullptr, &render_target_view);
		pBackBuffer->Release();
	}
}

void Window::onDestroy()
{
	b_is_run = false;

	LOG("Destroying Window!");

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	if (swap_chain) swap_chain->Release();
	if (device_context) device_context->Release();
	if (device) device->Release();
	if (render_target_view) render_target_view->Release();
}

HRESULT Window::LoadTextureFromFile(const std::wstring& fileName, ID3D11ShaderResourceView** textureView)
{
	if (!device || !textureView) {
		return E_POINTER;
	}

	HRESULT hr;

	// Use the DirectXTK WIC Texture Loader to create the texture
	hr = CreateWICTextureFromFile(device, device_context, fileName.c_str(), nullptr, textureView);

	if (FAILED(hr)) {
		LOGERROR("Failed Loading Texture from File");
	}
	
	return hr;
}

void Window::onUpdate()
{
	ImGui::Render();

	constexpr float color[4]{ 0.f, 0.f, 0.f, 0.f };
	device_context->OMSetRenderTargets(1U, &render_target_view, nullptr);
	device_context->ClearRenderTargetView(render_target_view, color);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();

	swap_chain->Present(1U, 0U);
}

void Window::onNewFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

POINT Window::GetWindowSize() {
	RECT rect;
	GetClientRect(m_hwnd, &rect);
	return { rect.right - rect.left, rect.bottom - rect.top };
}


bool Window::release()
{
	LOG("Releasing Window!");

	//Destroy the Window
	if (!DestroyWindow(m_hwnd))
		return false;

	return true;
}

Window::~Window()
{
}
