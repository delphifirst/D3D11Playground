#pragma once

#include <string>
#include <memory>
#include <cstdlib>
#include <functional>

namespace playground
{
	class EventListener
	{
	public:
		virtual void OnKeyDown(uint8_t key)
		{

		}
		virtual void OnKeyUp(uint8_t key)
		{

		}
		virtual void OnLButtonDown(int x, int y)
		{

		}
		virtual void OnRButtonDown(int x, int y)
		{

		}
		virtual void OnMouseMove(uint32_t key_mask, int x, int y)
		{

		}
	};
	class IDisplayer
	{
	public:
		virtual void Init(const std::string& title) = 0;
		virtual void InitDisplay(int width, int height) = 0;
		virtual bool ProcessEvent() = 0;

		virtual void SetEventListener(std::shared_ptr<EventListener> event_listener) = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual std::shared_ptr<EventListener> GetEventListener() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual void* GetHandle() const = 0;
	};

	void CreateDisplayer();
	extern std::shared_ptr<IDisplayer> gDisplayer;
}