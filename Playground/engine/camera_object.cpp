#include "camera_object.h"

#include "input.h"

using namespace std;
using namespace DirectX;

CameraObject::CameraObject(const wstring &name, double fovy, double zn, double zf)
	: Object(name), fovy_(fovy), zn_(zn), zf_(zf)
{

}

void CameraObject::OnUpdate(double delta_time)
{
	Object::OnUpdate(delta_time);

	rotation.y += Input::Instance().GetXMove() / 200.0;
	rotation.x += Input::Instance().GetYMove() / 200.0;

	XMVECTORF32 forward = { 0, 0, 1, 0 };
	XMVECTORF32 rightward = { 1, 0, 0, 0 };
	XMVECTORF32 upward = { 0, 1, 0, 0 };
	XMMATRIX transform = GetLocalTransform();
	XMVECTOR transformed_forward = XMVector3TransformNormal(
		forward, transform);
	XMVECTOR transformed_rightward = XMVector3TransformNormal(
		rightward, transform);

	double distance = 20 * delta_time;
	XMVECTOR new_position = XMLoadFloat3(&position);
	if (Input::Instance().IsPressed('W'))
		new_position += distance * transformed_forward;
	if (Input::Instance().IsPressed('S'))
		new_position -= distance * transformed_forward;
	if (Input::Instance().IsPressed('A'))
		new_position -= distance * transformed_rightward;
	if (Input::Instance().IsPressed('D'))
		new_position += distance * transformed_rightward;
	if (Input::Instance().IsPressed('Q'))
		new_position -= distance * upward;
	if (Input::Instance().IsPressed('E'))
		new_position += distance * upward;
	XMStoreFloat3(&position, new_position);
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