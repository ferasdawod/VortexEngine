#pragma once

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Color;

struct D3D11_INPUT_ELEMENT_DESC;

struct Vertex
{
	Vector3		_position;
	Color		_color;
	Vector2		_texCoords;
	Vector3		_normal;
	Vector3		_tangant;

	Vertex() {}

	Vertex(Vector3 position)
		: _position(position) {}

	Vertex(Vector3 position, Color color)
		: _position(position), _color(color) {}

	Vertex(Vector3 position, Color color, Vector2 texCoords)
		: _position(position), _color(color), _texCoords(texCoords) {}

	Vertex(Vector3 position, Color color, Vector2 texCoords, Vector3 normal)
		: _position(position), _color(color), _texCoords(texCoords), _normal(normal) {}

	Vertex(Vector3 position, Color color, Vector2 texCoords, Vector3 normal, Vector3 tangant)
		: _position(position), _color(color), _texCoords(texCoords), _normal(normal), _tangant(tangant) {}

	static const unsigned int InputElementsCount = 5;
	static const D3D11_INPUT_ELEMENT_DESC InputElements[InputElementsCount];
};