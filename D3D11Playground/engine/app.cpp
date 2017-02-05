#include "app.h"

#include <iostream>
#include <chrono>

#include "utils.h"
#include "engine.h"

using namespace std;
using namespace DirectX;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_SIZE:
		App::Instance().OnResizeWindow(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

void App::OnResizeWindow(int width, int height)
{
	// Avoid zero size
	width = max(width, 1);
	height = max(height, 1);

	wcout << "App::OnResizeWindow width:" << width << ", height:" << height << endl;
	Engine::Instance().OnResizeWindow(width, height);
}

void App::Init(const wstring& window_title, int width, int height)
{
	window_title_ = window_title;
	window_class_name_ = window_title_ + L"_game_engine";

	InitWindow(width, height);
	Engine::Instance().Init(main_hwnd_, width, height);
}

void App::EnterMainLoop()
{
	ShowWindow(main_hwnd_, SW_SHOWDEFAULT);
	UpdateWindow(main_hwnd_);

	chrono::time_point<chrono::steady_clock> last_frame_time = chrono::steady_clock::now();
	int frame_count = 0;
	chrono::duration<double> frame_timer(0);

	MSG msg;
	for (;;)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			chrono::time_point<chrono::steady_clock> current_frame_time 
				= chrono::steady_clock::now();
			chrono::duration<double> delta_time = current_frame_time - last_frame_time;

			Engine::Instance().OnTick(delta_time.count());

			last_frame_time = current_frame_time;
			frame_timer += delta_time;
			++frame_count;
			if (frame_timer > chrono::seconds(1))
			{
				frame_timer = chrono::seconds(0);
				SetWindowText(main_hwnd_, (window_title_
					+ L" (FPS: " + to_wstring(frame_count) + L")").c_str());
				frame_count = 0;
			}
		}
	}
}

void App::Dispose()
{
	Engine::Instance().Dispose();
	UnregisterClass(window_class_name_.c_str(), GetModuleHandle(0));
}

void App::InitWindow(int width, int height)
{
	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_OWNDC,
		MsgProc,
		0L,
		0L,
		GetModuleHandle(0),
		LoadIcon(0, IDI_APPLICATION),
		LoadCursor(0, IDC_ARROW),
		(HBRUSH)GetStockObject(WHITE_BRUSH),
		0,
		window_class_name_.c_str(),
		LoadIcon(0, IDI_WINLOGO)
	};

	RegisterClassEx(&wc);

	// Adjust window size so that client area match width and height
	RECT main_window_rect;
	SetRect(&main_window_rect, 100, 100, 100 + width, 100 + height);
	AdjustWindowRect(&main_window_rect, WS_OVERLAPPEDWINDOW, false);

	main_hwnd_ = CreateWindow(
		window_class_name_.c_str(),
		window_title_.c_str(),
		WS_OVERLAPPEDWINDOW,
		main_window_rect.left,
		main_window_rect.top,
		main_window_rect.right - main_window_rect.left,
		main_window_rect.bottom - main_window_rect.top,
		0,
		0,
		wc.hInstance,
		0
	);

	if (main_hwnd_ == 0)
		throw AppError(L"Cannot create main window");
}