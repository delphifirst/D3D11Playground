#include "input.h"

void Input::OnTick()
{
	last_x_ = current_x_;
	last_y_ = current_y_;
}

void Input::StartMouseMove(int x, int y)
{
	last_x_ = current_x_ = x;
	last_y_ = current_y_ = y;
}

void Input::UpdateMousePos(int x, int y)
{
	current_x_ = x;
	current_y_ = y;
}

void Input::UpdateKeyPressed(char key, bool is_pressed)
{
	key_pressed_[key] = is_pressed;
}