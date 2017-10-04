#pragma once

#include <Windows.h>

#include "displayer.h"

namespace playground
{
	class WindowsDisplayer :public IDisplayer
	{
	public:
		virtual void Init(const std::string& title) override;
		virtual void InitDisplay(int width, int height) override;
		virtual bool ProcessEvent() override;

		virtual void SetEventCallback(std::function<void()> callback) override;
		virtual void SetTitle(const std::string& title);

		virtual int GetWidth() const
		{
			return width_;
		}
		virtual int GetHeight() const
		{
			return height_;
		}
		virtual void* GetHandle() const
		{
			return main_hwnd_;
		}

	private:
		std::function<void()> event_callback_;
		
		int width_, height_;
		std::string window_title_;
		std::string window_class_name_;
		HWND main_hwnd_;
	};
}