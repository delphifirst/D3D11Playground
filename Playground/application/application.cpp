#include "application.h"

#include <iostream>
#include <memory>
#include <chrono>

#include "displayer.h"
#include "device.h"
#include "main_renderer.h"
#include "color_triangle.h"
#include "color.h"
#include "vec.h"

using namespace std;

namespace playground
{
	void StartApplication()
	{
		shared_ptr<IDisplayer> displayer = CreateDisplayer();
		displayer->Init("Playground");
		displayer->InitDisplay(512, 512);

		shared_ptr<IDevice> device = CreateDevice();
		device->Init(displayer);

		shared_ptr<MainRenderer> main_renderer = make_shared<MainRenderer>();
		main_renderer->Init(device);
		main_renderer->AddRenderObject(make_shared<ColorTriangle>(main_renderer,
			Vec3f(0, 0, 0), Vec3f(1, 0, 0), Vec3f(0.5, 0, 0), Vec3f(0, 1, 0), Vec3f(0, 0.5, 0), Vec3f(0, 0, 1)));

		chrono::time_point<chrono::steady_clock> last_frame_time = chrono::steady_clock::now();
		int frame_count = 0;
		chrono::duration<double> frame_timer(0);

		while (displayer->ProcessEvent())
		{
			chrono::time_point<chrono::steady_clock> current_frame_time = chrono::steady_clock::now();
			chrono::duration<double> delta_time = current_frame_time - last_frame_time;

			main_renderer->Render(delta_time.count());

			last_frame_time = current_frame_time;
			frame_timer += delta_time;
			++frame_count;
			if (frame_timer > chrono::seconds(1))
			{
				frame_timer = chrono::seconds(0);
				displayer->SetTitle("Playground (FPS: " + to_string(frame_count) + ")");
				frame_count = 0;
			}
		}
		device->Fini();
	}
}