#include "camera_object.h"

using namespace std;
using namespace DirectX;

CameraObject::CameraObject(const wstring &name, double fovy, double zn, double zf)
	: Object(name), fovy_(fovy), zn_(zn), zf_(zf)
{

}

void CameraObject::OnUpdate(double delta_time)
{
	Object::OnUpdate(delta_time);
}

XMMATRIX CameraObject::GetProjectMatrix(int width, int height) const
{
	double aspect = static_cast<double>(width) / height;
	return XMMatrixPerspectiveFovLH(fovy_, aspect, zn_, zf_);
}

XMMATRIX CameraObject::GetViewMatrix() const
{
	return XMMatrixInverse(nullptr, XMLoadFloat4x4(&world_transform()));
}