#pragma once

#include "IResourceLoader.h"

#include <utility>

class VertexBuffer;
class IndexBuffer;
using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
using VertexIndexPair = std::pair<VertexBufferPtr, IndexBufferPtr>;

// TODO make a dds texture loader
class Texture2DLoader : public IResourceLoader
{
public:
	Texture2DLoader() {}
	virtual ~Texture2DLoader() {}

	virtual std::shared_ptr<Resource> LoadResource(const std::string& resourceName);
};

class MeshLoader : public IResourceLoader
{
public:
	MeshLoader() {}
	virtual ~MeshLoader() {}

	virtual std::shared_ptr<Resource> LoadResource(const std::string& resourceName);

protected:
	std::shared_ptr<Resource>	CreateMeshFromFile(const std::string& resourceName);
	std::shared_ptr<Resource>	CreateSimpleMesh(const std::string& resourceName);
	VertexIndexPair				CreateBox();
	VertexIndexPair				CreatePlane();
	VertexIndexPair				CreateSphere();
	VertexIndexPair				CreateCylinder();

protected:
	static const std::string kSphereMeshName;
	static const std::string kCylinderMeshName;
	static const std::string kBoxMeshName;
	static const std::string kPlaneMeshName;

	static const size_t		kTessellation;
	static const float		kCylinderHeight;
	static const float		kCylinderDiamater;
};