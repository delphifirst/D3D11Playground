#include "scene_object.h"

#include "engine.h"

using namespace std;

SceneObject::SceneObject(const wstring& name)
	: Object(name)
{

}

void SceneObject::OnDraw()
{
	Object::OnDraw();

	resource_.Use();
	shader_.Use();
	render_state_.Use();
}