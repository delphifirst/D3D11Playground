#include "application.h"

#include <iostream>
#include <memory>
#include <chrono>

#include "displayer.h"
#include "device.h"
#include "main_renderer.h"
#include "camera.h"
#include "color_triangle.h"
#include "color.h"
#include "vec.h"
#include "math_constant.h"

using namespace std;

const int kWidth = 800;
const int kHeight = 600;

namespace playground
{
	class InputEventListener :public EventListener
	{
	public:
		InputEventListener(shared_ptr<Camera> camera)
			:camera_(camera)
		{

		}

		virtual void OnRButtonDown(int x, int y) override
		{
			last_x_ = current_x_ = x;
			last_y_ = current_y_ = y;
		}

		virtual void OnMouseMove(uint32_t key_mask, int x, int y) override
		{
			if (key_mask & MOUSE_MOVE_RBUTTON)
			{
				current_x_ = x;
				current_y_ = y;
			}
		}

		virtual void OnKeyDown(uint8_t key) override
		{
			key_pressed_[key] = true;
		}

		virtual void OnKeyUp(uint8_t key) override
		{
			key_pressed_[key] = false;
		}

		void Tick(float delta_time)
		{
			camera_->SetYaw(camera_->GetYaw() - (current_x_ - last_x_) / 200.0);
			camera_->SetPitch(camera_->GetPitch() - (current_y_ - last_y_) / 200.0);

			Mat4x4f inv_view = camera_->GetViewMat().Invert();
			Vec4f forward(0, 0, -1, 0);
			Vec4f rightward(1, 0, 0, 0);
			Vec4f upward(0, 1, 0, 0);
			forward = inv_view * forward;
			rightward = inv_view * rightward;

			float distance = 20 * delta_time;
			Vec3f new_position = camera_->GetPosition();
			if (key_pressed_['W'])
				new_position += distance * Vec3f(forward);
			if (key_pressed_['S'])
				new_position -= distance * Vec3f(forward);
			if (key_pressed_['A'])
				new_position -= distance * Vec3f(rightward);
			if (key_pressed_['D'])
				new_position += distance * Vec3f(rightward);
			if (key_pressed_['Q'])
				new_position -= distance * Vec3f(upward);
			if (key_pressed_['E'])
				new_position += distance * Vec3f(upward);

			camera_->SetPosition(new_position);

			last_x_ = current_x_;
			last_y_ = current_y_;
		}

	private:
		shared_ptr<Camera> camera_;

		int last_x_, last_y_;
		int current_x_, current_y_;
		bool key_pressed_[256] = {};
	};

	void StartApplication()
	{
		CreateDisplayer();
		gDisplayer->Init("Playground");
		gDisplayer->InitDisplay(kWidth, kHeight);

		CreateDevice();
		gDevice->Init();

		shared_ptr<MainRenderer> main_renderer = make_shared<MainRenderer>();
		main_renderer->Init(gDevice);
		main_renderer->AddRenderObject(make_shared<ColorTriangle>(
			Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0.5, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0.5, 0), Vec3f(0, 0, 1)));

		shared_ptr<Camera> camera = make_shared<Camera>(kPi / 2, static_cast<float>(kWidth) / static_cast<float>(kHeight), 0.1f, 500.0f);
		camera->SetPosition(Vec3f(0, 0, 3));

		shared_ptr<InputEventListener> event_listener = make_shared<InputEventListener>(camera);
		gDisplayer->SetEventListener(event_listener);

		chrono::time_point<chrono::steady_clock> last_frame_time = chrono::steady_clock::now();
		int frame_count = 0;
		chrono::duration<float> frame_timer(0);

		while (gDisplayer->ProcessEvent())
		{
			chrono::time_point<chrono::steady_clock> current_frame_time = chrono::steady_clock::now();
			chrono::duration<float> delta_time = current_frame_time - last_frame_time;

			event_listener->Tick(delta_time.count());
			main_renderer->Render(camera, delta_time.count());

			last_frame_time = current_frame_time;
			frame_timer += delta_time;
			++frame_count;
			if (frame_timer > chrono::seconds(1))
			{
				frame_timer = chrono::seconds(0);
				gDisplayer->SetTitle("Playground (FPS: " + to_string(frame_count) + ")");
				frame_count = 0;
			}
		}
		gDevice->Fini();
	}
}