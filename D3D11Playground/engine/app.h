#pragma once

#include <string>

#include <Windows.h>

class App
{
	friend LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam,
		LPARAM lParam);

public:
	// Singleton
	static App& Instance()
	{
		static App instance;
		return instance;
	}
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	// Life cycle
	void Init(const std::wstring& window_title, int width, int height);
	void EnterMainLoop();
	void Dispose();

private:
	App() = default;
	void InitWindow(int width, int height);
	void OnResizeWindow(int width, int height);

	// Members for Win32 GUI stuff
	std::wstring window_title_;
	std::wstring window_class_name_;
	HWND main_hwnd_;
};
