#pragma once

#include <string>

#include "object.h"
#include "shader.h"
#include "render_state.h"
#include "resource.h"

class SceneObject : public Object
{
public:
	SceneObject(const std::wstring& name);

	virtual void OnDraw() override;

protected:
	Shader shader_;
	RenderState render_state_;
	Resource resource_;
};