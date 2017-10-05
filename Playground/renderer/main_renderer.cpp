#include "main_renderer.h"

#include "device.h"
#include "render_object.h"

using namespace std;

namespace playground
{
	void MainRenderer::Init(std::shared_ptr<IDevice> device)
	{
		device_ = device;
	}

	void MainRenderer::Render(float delta_time)
	{
		device_->Clear(0, 0.1, 0.2, 1);
		for (shared_ptr<IRenderObject> render_object : render_objects_)
			render_object->Draw(device_);
		device_->Present();
	}

	void MainRenderer::AddRenderObject(shared_ptr<IRenderObject> render_object)
	{
		render_objects_.push_back(render_object);
	}
}