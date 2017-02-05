#include "object.h"

using namespace std;
using namespace DirectX;

Object::Object(const wstring& name) 
	: name_(name), scale(1, 1, 1)
{

}

Object::~Object()
{
	for (Object* child : children_)
		delete child;
}

void Object::AddChild(Object * child)
{
	children_.push_back(child);
}

void Object::OnUpdate(double delta_time)
{
	for (Object* child : children_)
		child->OnUpdate(delta_time);
}

void Object::OnDraw()
{
	for (Object* child : children_)
		child->OnDraw();
}

XMMATRIX XM_CALLCONV Object::GetLocalTransform() const
{
	XMMATRIX translation_matrix = XMMatrixTranslation(
		position.x, position.y, position.z);

	XMMATRIX rotation_y_matrix = XMMatrixRotationY(rotation.y);
	XMMATRIX rotation_x_matrix = XMMatrixRotationX(rotation.x);
	XMMATRIX rotation_z_matrix = XMMatrixRotationZ(rotation.z);

	XMMATRIX scale_matrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	return scale_matrix * rotation_z_matrix * rotation_x_matrix
		* rotation_y_matrix * translation_matrix;
}

void Object::UpdateWorldTransform(DirectX::CXMMATRIX parent_transform)
{
	XMMATRIX new_world_transform = GetLocalTransform() * parent_transform;
	XMStoreFloat4x4(&world_transform_, new_world_transform);
	for (Object * child : children_)
	{
		child->UpdateWorldTransform(new_world_transform);
	}
}