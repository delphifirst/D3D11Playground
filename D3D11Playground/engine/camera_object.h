#pragma once

#include <DirectXMath.h>

#include "object.h"

class CameraObject : public Object
{
public:
	CameraObject(const std::wstring &name, double fovy, double zn, double zf);

	void OnUpdate(double delta_time) override;

	DirectX::XMMATRIX GetProjectMatrix(int width, int height) const;
	DirectX::XMMATRIX GetViewMatrix() const;

private:
	double fovy_, zn_, zf_;
};