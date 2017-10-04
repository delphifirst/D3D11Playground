#pragma once

#include <string>
#include <memory>
#include <functional>

namespace playground
{
	class IDisplayer
	{
	public:
		virtual void Init(const std::string& title) = 0;
		virtual void InitDisplay(int width, int height) = 0;
		virtual bool ProcessEvent() = 0;

		virtual void SetEventCallback(std::function<void()> callback) = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual void* GetHandle() const = 0;
	};

	std::shared_ptr<IDisplayer> CreateDisplayer();
}