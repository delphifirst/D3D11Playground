#pragma once

#include <Windows.h>

class Input
{
	friend LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam,
		LPARAM lParam);
public:
	static Input& Instance()
	{
		static Input instance;
		return instance;
	}
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	int GetXMove() const
	{
		return current_x_ - last_x_;
	}

	int GetYMove() const
	{
		return current_y_ - last_y_;
	}

	bool IsPressed(char key) const
	{
		return key_pressed_[key];
	}

	void OnTick();
private:
	Input() = default;
	void StartMouseMove(int x, int y);
	void UpdateMousePos(int x, int y);
	
	void UpdateKeyPressed(char key, bool is_pressed);

	int last_x_ = 0, last_y_ = 0;
	int current_x_ = 0, current_y_ = 0;
	bool key_pressed_[256] = {};
};