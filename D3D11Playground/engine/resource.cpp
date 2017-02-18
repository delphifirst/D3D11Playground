#include "resource.h"

#include <cassert>

#include <DDSTextureLoader.h>

#include "utils.h"
#include "engine.h"

using namespace std;
using namespace DirectX;

Resource::~Resource()
{
	for (ID3D11Buffer* vertex_buffer: vertex_buffers_)
		SafeRelease(vertex_buffer);
	for (ID3D11Buffer* buffer : vs_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11Buffer* buffer : ds_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11Buffer* buffer : cs_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11Buffer* buffer : gs_cbuffers_)
		SafeRelease(buffer);
	for (ID3D11ShaderResourceView* shader_resource_view : ps_shader_resource_views_)
		SafeRelease(shader_resource_view);
	for (ID3D11ShaderResourceView* shader_resource_view : cs_shader_resource_views_)
		SafeRelease(shader_resource_view);
	for (ID3D11UnorderedAccessView* unordered_access_view : cs_unordered_access_views_)
		SafeRelease(unordered_access_view);
}

void Resource::AddCBuffer(ShaderType shader, int bytes)
{
	ID3D11Buffer *cbuffer = nullptr;

	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	buffer_desc.ByteWidth = bytes;
	buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	HRESULT hr = Engine::Instance().device()->CreateBuffer(&buffer_desc, nullptr, &cbuffer);
	assert(SUCCEEDED(hr));

	switch (shader)
	{
	case ShaderType::VS:
		vs_cbuffers_.push_back(cbuffer);
		break;
	case ShaderType::DS:
		ds_cbuffers_.push_back(cbuffer);
		break;
	case ShaderType::CS:
		cs_cbuffers_.push_back(cbuffer);
		break;
	case ShaderType::GS:
		gs_cbuffers_.push_back(cbuffer);
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::UpdateCBuffer(ShaderType shader, int slot, void *data, int bytes)
{
	D3D11_MAPPED_SUBRESOURCE buffer_map = {};
	HRESULT hr;
	switch (shader)
	{
	case ShaderType::VS:
		hr = Engine::Instance().device_context()->Map(
			vs_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer_map);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::DS:
		hr = Engine::Instance().device_context()->Map(
			ds_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer_map);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::CS:
		hr = Engine::Instance().device_context()->Map(
			cs_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer_map);
		assert(SUCCEEDED(hr));
		break;
	case ShaderType::GS:
		hr = Engine::Instance().device_context()->Map(
			gs_cbuffers_[slot], 0, D3D11_MAP_WRITE_DISCARD, 0, &buffer_map);
		assert(SUCCEEDED(hr));
		break;
	default:
		assert(false);
		break;
	}
	memcpy(buffer_map.pData, data, bytes);
	switch (shader)
	{
	case ShaderType::VS:
		Engine::Instance().device_context()->Unmap(vs_cbuffers_[slot], 0);
		break;
	case ShaderType::DS:
		Engine::Instance().device_context()->Unmap(ds_cbuffers_[slot], 0);
		break;
	case ShaderType::CS:
		Engine::Instance().device_context()->Unmap(cs_cbuffers_[slot], 0);
		break;
	case ShaderType::GS:
		Engine::Instance().device_context()->Unmap(gs_cbuffers_[slot], 0);
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::AddVertexBuffer(void* vertex_data, UINT stride, int bytes, bool is_dynamic)
{
	D3D11_BUFFER_DESC buffer_desc;
	buffer_desc.Usage = is_dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	buffer_desc.ByteWidth = bytes;
	buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffer_desc.CPUAccessFlags = is_dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	buffer_desc.MiscFlags = 0;
	buffer_desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = vertex_data;
	vertex_buffers_.emplace_back(nullptr);
	vertex_buffer_strides_.push_back(stride);
	vertex_buffer_offsets_.push_back(0);
	HRESULT hr = Engine::Instance().device()->CreateBuffer(&buffer_desc, 
		vertex_data ? &init_data : nullptr, &vertex_buffers_.back());
	assert(SUCCEEDED(hr));
}

void Resource::UnmapVertexBuffer(int slot)
{
	Engine::Instance().device_context()->Unmap(vertex_buffers_[slot], 0);
}

void Resource::UpdateVertexBuffer(int slot, void* vertex_data, int bytes)
{
	void *vertex_buffer = MapVertexBuffer<void>(slot);
	memcpy(vertex_buffer, vertex_data, bytes);
	UnmapVertexBuffer(slot);
}

void Resource::AddTexture(ShaderType shader, const std::wstring &filename)
{
	ID3D11Resource* texture = nullptr;

	ID3D11ShaderResourceView *shader_resource_view = nullptr;

	HRESULT hr = CreateDDSTextureFromFile(Engine::Instance().device(), 
		filename.c_str(), &texture, &shader_resource_view);
	assert(SUCCEEDED(hr));
	SafeRelease(texture);

	switch (shader)
	{
	case ShaderType::PS:
		ps_shader_resource_views_.push_back(shader_resource_view);
		break;
	case ShaderType::CS:
		cs_shader_resource_views_.push_back(shader_resource_view);
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::AddTexture(ShaderType shader, int width, int height, DXGI_FORMAT format, void* data)
{
	D3D11_TEXTURE2D_DESC texture_desc;
	texture_desc.Width = width;
	texture_desc.Height = height;
	texture_desc.MipLevels = 1;
	texture_desc.ArraySize = 1;
	texture_desc.Format = format;
	texture_desc.SampleDesc.Count = 1;
	texture_desc.SampleDesc.Quality = 0;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture_desc.CPUAccessFlags = 0;
	texture_desc.MiscFlags = 0;

	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView *shader_resource_view = nullptr;

	D3D11_SUBRESOURCE_DATA init_data;
	init_data.pSysMem = data;
	init_data.SysMemPitch = width * GetPixelSize(format);
	init_data.SysMemSlicePitch = 0;
	HRESULT hr = Engine::Instance().device()->CreateTexture2D(&texture_desc, &init_data, &texture);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_RESOURCE_VIEW_DESC texture_view_desc;
	texture_view_desc.Format = format;
	texture_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	texture_view_desc.Texture2D.MostDetailedMip = 0;
	texture_view_desc.Texture2D.MipLevels = 1;
	hr = Engine::Instance().device()->CreateShaderResourceView(
		texture, &texture_view_desc, &shader_resource_view);
	assert(SUCCEEDED(hr));
	SafeRelease(texture);

	switch (shader)
	{
	case ShaderType::CS:
		cs_shader_resource_views_.push_back(shader_resource_view);
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::AddStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count)
{
	ID3D11Buffer* structured_buffer = CreateStructuredBuffer(data, elem_bytes, elem_count);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.BufferEx.NumElements = elem_count;

	ID3D11ShaderResourceView* shader_resource_view = nullptr;
	HRESULT hr = Engine::Instance().device()->CreateShaderResourceView(
		structured_buffer, &desc, &shader_resource_view);
	assert(SUCCEEDED(hr));
	SafeRelease(structured_buffer);

	switch (shader)
	{
	case ShaderType::CS:
		cs_shader_resource_views_.push_back(shader_resource_view);
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::AddRWStructuredBuffer(ShaderType shader, void* data, int elem_bytes, int elem_count)
{
	ID3D11Buffer* structured_buffer = CreateStructuredBuffer(data, elem_bytes, elem_count);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;
	// Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.NumElements = elem_count;

	ID3D11UnorderedAccessView* unordered_access_view = nullptr;
	HRESULT hr = Engine::Instance().device()->CreateUnorderedAccessView(
		structured_buffer, &desc, &unordered_access_view);
	assert(SUCCEEDED(hr));
	SafeRelease(structured_buffer);

	switch (shader)
	{
	case ShaderType::CS:
		cs_unordered_access_views_.push_back(unordered_access_view);
		break;
	default:
		assert(false);
		break;
	}
}

ID3D11UnorderedAccessView* Resource::GetRWStructuredBuffer(ShaderType shader, int slot)
{
	switch (shader)
	{
	case ShaderType::CS:
		return cs_unordered_access_views_[slot];
		break;
	default:
		assert(false);
		break;
	}
}

void Resource::Use()
{
	Engine::Instance().device_context()->VSSetConstantBuffers(
		0, vs_cbuffers_.size(), vs_cbuffers_.data());
	Engine::Instance().device_context()->DSSetConstantBuffers(
		0, ds_cbuffers_.size(), ds_cbuffers_.data());
	Engine::Instance().device_context()->CSSetConstantBuffers(
		0, cs_cbuffers_.size(), cs_cbuffers_.data());
	Engine::Instance().device_context()->GSSetConstantBuffers(
		0, gs_cbuffers_.size(), gs_cbuffers_.data());
	Engine::Instance().device_context()->PSSetShaderResources(
		0, ps_shader_resource_views_.size(), ps_shader_resource_views_.data());
	Engine::Instance().device_context()->CSSetShaderResources(
		0, cs_shader_resource_views_.size(), cs_shader_resource_views_.data());
	Engine::Instance().device_context()->CSSetUnorderedAccessViews(
		0, cs_unordered_access_views_.size(), cs_unordered_access_views_.data(), nullptr);
}

void Resource::IASetVertexBuffers(initializer_list<int> indices)
{
	vector<ID3D11Buffer*> buffers;
	vector<UINT> strides;
	vector<UINT> offsets;
	for (int index : indices)
	{
		buffers.push_back(vertex_buffers_[index]);
		strides.push_back(vertex_buffer_strides_[index]);
		offsets.push_back(vertex_buffer_offsets_[index]);
	}
	Engine::Instance().device_context()->IASetVertexBuffers(
		0, buffers.size(), buffers.data(), strides.data(), offsets.data());
}

void Resource::SoSetTargets(initializer_list<int> indices)
{

}

ID3D11Buffer* Resource::CreateStructuredBuffer(void* data, int elem_bytes, int elem_count)
{
	ID3D11Buffer* result = nullptr;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.ByteWidth = elem_bytes * elem_count;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = elem_bytes;

	if (data)
	{
		D3D11_SUBRESOURCE_DATA init_data;
		init_data.pSysMem = data;
		HRESULT hr = Engine::Instance().device()->CreateBuffer(&desc, &init_data, &result);
		assert(SUCCEEDED(hr));
	}
	else
	{
		HRESULT hr = Engine::Instance().device()->CreateBuffer(&desc, nullptr, &result);
		assert(SUCCEEDED(hr));
	}
	return result;
}