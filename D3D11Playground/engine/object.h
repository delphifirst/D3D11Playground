#pragma once

#include <string>
#include <vector>

#include <DirectXMath.h>

class Object
{
	friend class Engine;
public:
	Object(const std::wstring & name);
	virtual ~Object();
	Object(const Object&) = delete;
	Object& operator=(const Object&) = delete;

	const std::wstring name() const
	{
		return name_;
	}

	void set_name(const std::wstring& name)
	{
		name_ = name;
	}

	DirectX::XMFLOAT4X4 world_transform() const
	{
		return world_transform_;
	}

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;

	void AddChild(Object * child);

	DirectX::XMMATRIX XM_CALLCONV GetLocalTransform() const;

	// Override these methods to customize game objects behavior.
	virtual void OnUpdate(double delta_time);
	virtual void OnDraw();

private:
	// Only called by Engine to update world_transform_ according to
	// position, rotation and scale
	void UpdateWorldTransform(DirectX::CXMMATRIX parent_transform);
	// This matrix is generated in every frame according to the 
	// position, rotation and scale of this game object.
	DirectX::XMFLOAT4X4 world_transform_;

	std::wstring name_;
	std::vector<Object*> children_;
};