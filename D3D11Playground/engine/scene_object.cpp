#include "scene_object.h"

#include "engine.h"

using namespace std;

SceneObject::SceneObject(const wstring& name, void *vertex_data, int vertex_stride, int total_bytes)
	: Object(name), resource_(vertex_data, vertex_stride, total_bytes)
{

}

void SceneObject::OnDraw()
{
	Object::OnDraw();

	resource_.Use();
	shader_.Use();
	render_state_.Use();
}