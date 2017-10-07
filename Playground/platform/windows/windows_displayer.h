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

		virtual void SetEventListener(std::shared_ptr<EventListener> event_listener) override;
		virtual void SetTitle(const std::string& title);

		virtual std::shared_ptr<EventListener> GetEventListener() const override
		{
			return event_listener_;
		}
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
		std::shared_ptr<EventListener> event_listener_;
		
		int width_, height_;
		std::string window_title_;
		std::string window_class_name_;
		HWND main_hwnd_;
	};

	extern std::shared_ptr<WindowsDisplayer> gWindowsDisplayer;
}