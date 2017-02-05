#include "resource.h"

#include <DDSTextureLoader.h>

#include "utils.h"
#include "engine.h"

using namespace DirectX;

Resource::Resource(void *vertex_data, int vertex_stride, int total_bytes)
	:vertex_stride_(vertex_stride)
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = total_bytes;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	AssertSucceed(Engine::Instance().device()->CreateBuffer(&buffer_desc, nullptr, &vertex_buffer_),
		L"Cannot create vertex buffer");
	Engine::Instance().device_context()->UpdateSubresource(
		vertex_buffer_, 0, nullptr, vertex_data, 0, 0);
}

Resource::~Resource()
{
	SafeRelease(vertex_buffer_);
	for (ID3D11Buffer* buffer : vs_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11Buffer* buffer : ds_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11ShaderResourceView* texture_view : ps_texture_views_)
		SafeRelease(texture_view);
}

void Resource::AddCBuffer(ShaderType shader, int bytes)
{
	ID3D11Buffer **cbuffer = nullptr;
	switch (shader)
	{
	case ShaderType::VS:
		vs_cbuffers_.emplace_back(nullptr);
		cbuffer = &vs_cbuffers_.back();
		break;
	case ShaderType::DS:
		ds_cbuffers_.emplace_back(nullptr);
		cbuffer = &ds_cbuffers_.back();
		break;
	}

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = bytes;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	AssertSucceed(Engine::Instance().device()->CreateBuffer(
		&buffer_desc, nullptr, cbuffer), 
		L"Cannot create cbuffer");
}

void Resource::AddTexture(ShaderType shader, const std::wstring &filename)
{
	ID3D11Resource* texture = nullptr;

	ID3D11ShaderResourceView **texture_view = nullptr;
	switch (shader)
	{
	case ShaderType::PS:
		ps_texture_views_.emplace_back(nullptr);
		texture_view = &ps_texture_views_.back();
		break;
	}

	AssertSucceed(CreateDDSTextureFromFile(Engine::Instance().device(), filename.c_str(),
		&texture, texture_view), L"Cannot load texture: " + filename);
	SafeRelease(texture);
}

void Resource::UpdateCBuffer(ShaderType shader, int slot, void *data, int bytes)
{
	D3D11_MAPPED_SUBRESOURCE cbuffer_map;
	switch (shader)
	{
	case ShaderType::VS:
		AssertSucceed(Engine::Instance().device_context()->Map(
			vs_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0,
			&cbuffer_map), L"Map cbuffer failed");
		break;
	case ShaderType::DS:
		AssertSucceed(Engine::Instance().device_context()->Map(
			ds_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0,
			&cbuffer_map), L"Map cbuffer failed");
		break;
	}
	memcpy(cbuffer_map.pData, data, bytes);
	switch (shader)
	{
	case ShaderType::VS:
		Engine::Instance().device_context()->Unmap(vs_cbuffers_[slot], 0);
		break;
	case ShaderType::DS:
		Engine::Instance().device_context()->Unmap(ds_cbuffers_[slot], 0);
		break;
	}
}

void Resource::Use()
{
	Engine::Instance().device_context()->VSSetConstantBuffers(
		0, vs_cbuffers_.size(), vs_cbuffers_.data());
	Engine::Instance().device_context()->DSSetConstantBuffers(
		0, ds_cbuffers_.size(), ds_cbuffers_.data());
	Engine::Instance().device_context()->PSSetShaderResources(
		0, ps_texture_views_.size(), ps_texture_views_.data());
	UINT stride = vertex_stride_;
	UINT offset = 0;
	Engine::Instance().device_context()->IASetVertexBuffers(0, 1, &vertex_buffer_, &stride, &offset);
}