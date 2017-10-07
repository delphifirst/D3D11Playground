#include "windows_displayer.h"

#include <memory>
#include <cstdlib>
#include <cassert>

#include "platform_constant.h"

using namespace std;

LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	std::shared_ptr<playground::EventListener> event_listener = playground::gDisplayer->GetEventListener();
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_LBUTTONDOWN:
		event_listener->OnLButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_RBUTTONDOWN:
		event_listener->OnRButtonDown(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_MOUSEMOVE:
		{
			uint32_t mouse_move_key_mask = 0;
			if (wParam & MK_LBUTTON)
				mouse_move_key_mask |= MOUSE_MOVE_LBUTTON;
			if (wParam & MK_RBUTTON)
				mouse_move_key_mask |= MOUSE_MOVE_RBUTTON;
			event_listener->OnMouseMove(mouse_move_key_mask, LOWORD(lParam), HIWORD(lParam));
		}
		break;
	case WM_KEYDOWN:
		event_listener->OnKeyDown(wParam);
		break;
	case WM_KEYUP:
		event_listener->OnKeyUp(wParam);
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}

	return 0;
}

namespace playground
{
	shared_ptr<IDisplayer> gDisplayer;
	shared_ptr<WindowsDisplayer> gWindowsDisplayer;

	void CreateDisplayer()
	{
		gWindowsDisplayer = make_shared<WindowsDisplayer>();
		gDisplayer = gWindowsDisplayer;
	}

	void WindowsDisplayer::Init(const string& title)
	{
		window_title_ = title;
		window_class_name_ = title + "_playground";
		event_listener_ = make_shared<EventListener>();
	}

	void WindowsDisplayer::InitDisplay(int width, int height)
	{
		width_ = width;
		height_ = height;

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

		assert(main_hwnd_ != 0);

		ShowWindow(main_hwnd_, SW_SHOWDEFAULT);
		UpdateWindow(main_hwnd_);
	}

	bool WindowsDisplayer::ProcessEvent()
	{
		MSG msg;
		for (;;)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
					return false;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				return true;
			}
		}
	}

	void WindowsDisplayer::SetEventListener(shared_ptr<EventListener> event_listener)
	{
		event_listener_ = event_listener;
	}

	void WindowsDisplayer::SetTitle(const std::string& title)
	{
		window_title_ = title;
		SetWindowText(main_hwnd_, title.c_str());
	}
}