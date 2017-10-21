#pragma once

enum VertexFormat
{
	VERTEX_FORMAT_R32G32B32_FLOAT,
	VERTEX_FORMAT_SIZE
};

enum IndexBufferType
{
	INDEX_FORMAT_UINT16,
	INDEX_FORMAT_UINT32
};

enum PrimitiveTopology
{
	PRIMITIVE_TOPOLOGY_TRIANGLELIST,
	PRIMITIVE_TOPOLOGY_SIZE
};

enum MouseMoveKeyMask
{
	MOUSE_MOVE_LBUTTON = 0x1,
	MOUSE_MOVE_RBUTTON = 0x2
};