#include "pch.h"

#include "ResourceLoaders.h"

#include <DirectXTK/WICTextureLoader.h>
#include <DirectXTK/DDSTextureLoader.h>
#include <DirectXTK/DirectXHelpers.h>
#include <DirectXCollision.h>

#include "Graphics/Rendering/GraphicsDevice.h"

#include "Graphics/Texture2D.h"
#include "Graphics/Mesh.h"
#include "Graphics/SubMesh.h"
#include "Graphics/Vertex.h"

#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexBuffer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/types.h>
#include <assimp\postprocess.h>

#include "GeometryGenerator.h"

using DirectX::BoundingBox;
using DirectX::BoundingSphere;

#pragma region Texture2DLoader

std::shared_ptr<Resource> Texture2DLoader::LoadResource(const std::string& resourceName)
{
	FUNC_PROFILE();

	ComPtr<ID3D11ShaderResourceView> shaderView;
	std::wstring wideFileName = Utils::Str2WideStr(resourceName);
	HRESULT hr = S_OK;

	if (Utils::WildcardMatch("*.dds", resourceName))
	{
		hr = DirectX::CreateDDSTextureFromFile(GraphicsDevice::GetPtr()->GetDevice().Get(), GraphicsDevice::GetPtr()->GetContext().Get(),
											   wideFileName.c_str(), nullptr, &shaderView);
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(GraphicsDevice::GetPtr()->GetDevice().Get(), GraphicsDevice::GetPtr()->GetContext().Get(),
											   wideFileName.c_str(), nullptr, &shaderView);
	}
	if (FAILED(hr))
	{
		auto msg = "Failed to load texture : " + resourceName;
		LOG_E(msg.c_str(), hr);
	}

	return std::shared_ptr<Resource>(DBG_NEW Texture2D(resourceName, shaderView));
}

#pragma endregion

#pragma region Mesh Loader

struct MeshImportOptions
{
	bool	_OverwriteNormals;
	bool	_GenerateNormals;
	bool	_GenerateSmoothNormals;
	bool	_InverteNormals;
	bool	_FlipWindningOrder;
	bool	_FlipUVs;

	BoundingShapeType _BoundingShapeType;

	MeshImportOptions() :
		_OverwriteNormals(false), _GenerateNormals(true), _GenerateSmoothNormals(false),
		_InverteNormals(false), _FlipWindningOrder(true), _FlipUVs(false), _BoundingShapeType(BoundingShapeType::Sphere)
	{
	}

	MeshImportOptions(TiXmlElement* optionsE) : MeshImportOptions()
	{
		ReadOptions(optionsE);
	}

	void ReadOptions(TiXmlElement* optionsE)
	{

		Assert(optionsE->ValueStr() == "MeshImportOptions", "This is not a valid options file");

		XmlHelper::FromXml(optionsE, "OverwriteNormals", _OverwriteNormals);
		XmlHelper::FromXml(optionsE, "GenerateNormals", _GenerateNormals);
		XmlHelper::FromXml(optionsE, "GenerateSmoothNormals", _GenerateSmoothNormals);
		XmlHelper::FromXml(optionsE, "InverteNormals", _InverteNormals);
		XmlHelper::FromXml(optionsE, "FlipWindingOrder", _FlipWindningOrder);
		XmlHelper::FromXml(optionsE, "FlipUVs", _FlipUVs);

		int bounderInt = -1;
		XmlHelper::FromXml(optionsE, "BoundingShapeType", bounderInt);
		if (bounderInt != -1) _BoundingShapeType = static_cast<BoundingShapeType>(bounderInt);

		if (_GenerateNormals && _GenerateSmoothNormals)
			LOG_M("invalid options for mesh loader, generate normals and generate smooth normals can't be both set");
	}

};

const std::string MeshLoader::kBoxMeshName = "Box.mesh";
const std::string MeshLoader::kCylinderMeshName = "Cylinder.mesh";
const std::string MeshLoader::kPlaneMeshName = "Plane.mesh";
const std::string MeshLoader::kSphereMeshName = "Sphere.mesh";

const size_t	MeshLoader::kTessellation = 32;
const float		MeshLoader::kCylinderDiamater = 1.0f;
const float		MeshLoader::kCylinderHeight = 2.0f;

std::shared_ptr<Resource> MeshLoader::LoadResource(const std::string& resourceName)
{
	FUNC_PROFILE();

	if (resourceName == kBoxMeshName || resourceName == kCylinderMeshName || resourceName == kPlaneMeshName || resourceName == kSphereMeshName)
	{
		return CreateSimpleMesh(resourceName);
	}
	else
	{
		return CreateMeshFromFile(resourceName);
	}
}

std::shared_ptr<Resource> MeshLoader::CreateMeshFromFile(const std::string& resourceName)
{
	// this is an external mesh that we have to load
	if (resourceName.empty())
		LOG_E("The mesh file name is empty", 0);


	Assimp::Importer meshImporter;
	MeshImportOptions importOptions;
	unsigned int optionsFlags =
		aiProcess_CalcTangentSpace |
		aiProcess_JoinIdenticalVertices |
		aiProcess_Triangulate |
		aiProcess_FixInfacingNormals |
		aiProcess_FindInvalidData |
		aiProcess_OptimizeMeshes;

	// import options loading
	std::unique_ptr<TiXmlDocument>	importDocument(DBG_NEW TiXmlDocument(resourceName + ".meta"));
	if (importDocument->LoadFile())
	{
		auto rootElement = importDocument->RootElement();
		importOptions.ReadOptions(rootElement);

		if (importOptions._FlipUVs) optionsFlags |= aiProcess_FlipUVs;
		if (importOptions._FlipWindningOrder) optionsFlags |= aiProcess_FlipWindingOrder;
		if (importOptions._OverwriteNormals)
		{
			optionsFlags |= aiProcess_RemoveComponent;
			meshImporter.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_NORMALS);
			if (importOptions._GenerateNormals) optionsFlags |= aiProcess_GenNormals;
			else if (importOptions._GenerateSmoothNormals) optionsFlags |= aiProcess_GenSmoothNormals;
			else LOG_M("mesh has overwrite normals but no generate normals flag");
		}
	}
	else
		LOG_M("Mesh : " + resourceName + " does not have an import options file");

	// original flags
	/*
	unsigned int importerFlags = aiProcess_GenNormals | aiProcess_FlipWindingOrder | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate;
	*/

	const aiScene* pScene = meshImporter.ReadFile(resourceName, optionsFlags);
	if (pScene == nullptr)
	{
		auto msg = "Failed to load the mesh : " + resourceName;
		LOG_E(msg.c_str(), 0);
	}

	if (pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
	{
		auto msg = "the mesh : " + resourceName + " has the incomplete flag";
		LOG_E(msg.c_str(), 0);
	}

	if (pScene->mNumMeshes == 0)
	{
		auto msg = "the mesh file : " + resourceName + " does not have any mesh data inside it";
		LOG_E(msg.c_str(), 0);
	}

	std::shared_ptr<Mesh> meshPtr(DBG_NEW Mesh(resourceName));

	for (size_t i = 0; i < pScene->mNumMeshes; ++i)
	{
		auto currentMesh = pScene->mMeshes[i];

		std::vector<Vertex> vertices;
		vertices.reserve(currentMesh->mNumVertices);
		std::vector<WORD> indices;
		indices.reserve(currentMesh->mNumFaces);

		// first fill the vertex buffer
		for (size_t vertexIndex = 0; vertexIndex < currentMesh->mNumVertices; vertexIndex++)
		{
			Vector3 pos = Utils::aiVec2Vector3(currentMesh->mVertices[vertexIndex]);
			Vector3 normal = currentMesh->HasNormals() ? Utils::aiVec2Vector3(currentMesh->mNormals[vertexIndex]) : Vector3::Zero;
			Vector2 texCoords = currentMesh->HasTextureCoords(0) ? Utils::aiVec2Vector2(currentMesh->mTextureCoords[0][vertexIndex]) : Vector2::Zero;

			if (importOptions._InverteNormals) normal *= -1.0f;

			vertices.emplace_back(Vertex(pos, MyColors::White, texCoords, normal));
		}

		// now to fill the index buffer
		for (size_t faceIndex = 0; faceIndex < currentMesh->mNumFaces; faceIndex++)
		{
			aiFace currentFace = currentMesh->mFaces[faceIndex];
			for (size_t currIndex = 0; currIndex < currentFace.mNumIndices; currIndex++)
			{
				indices.emplace_back(currentFace.mIndices[currIndex]);
			}
		}

		auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), vertices.data(), vertices.size(), sizeof(Vertex));
		auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), indices.data(), indices.size());

		std::shared_ptr<SubMesh> subMeshPtr(DBG_NEW SubMesh(std::make_pair(vertexBuffer, indexBuffer), D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		meshPtr->AddSubMesh(subMeshPtr);

		// bounder calculation
		if (importOptions._BoundingShapeType == BoundingShapeType::Box)
		{
			subMeshPtr->_BoundingShapeType = importOptions._BoundingShapeType;

			std::shared_ptr<BoundingBox> boundingBox(DBG_NEW BoundingBox);
			BoundingBox::CreateFromPoints(*boundingBox, vertices.size(), (Vector3*)(vertices.data()), sizeof(Vertex));

			subMeshPtr->_BoundingBox = boundingBox;
		}
		else if (importOptions._BoundingShapeType == BoundingShapeType::Sphere)
		{
			subMeshPtr->_BoundingShapeType = importOptions._BoundingShapeType;

			std::shared_ptr<BoundingSphere> boundingSphere(DBG_NEW BoundingSphere);
			BoundingSphere::CreateFromPoints(*boundingSphere, vertices.size(), (Vector3*)(vertices.data()), sizeof(Vertex));

			subMeshPtr->_BoundingSphere = boundingSphere;
		}
	}

	return std::static_pointer_cast<Resource>(meshPtr);
}

// OLD CODE
/*std::shared_ptr<Resource> MeshLoader::CreateSimpleMesh(const std::string& resourceName)
{
	// this is a simple mesh that we generate at runtime
	VertexIndexPair pair;

	if (resourceName == kBoxMeshName)
	{
		pair = CreateBox();
	}
	else if (resourceName == kCylinderMeshName)
	{
		pair = CreateCylinder();
	}
	else if (resourceName == kPlaneMeshName)
	{
		pair = CreatePlane();
	}
	else if (resourceName == kSphereMeshName)
	{
		pair = CreateSphere();
	}

	SubMeshPtr subMesh(DBG_NEW SubMesh(pair, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	std::shared_ptr<Mesh> resource(DBG_NEW Mesh(resourceName));
	resource->AddSubMesh(subMesh);

	return std::static_pointer_cast<Resource>(resource);
}*/

// NEW CODE
void ReverseWinding(std::vector<WORD>& indices, std::vector<Vertex>& vertices)
{
	assert((indices.size() % 3) == 0);
	for (auto it = indices.begin(); it != indices.end(); it += 3)
	{
		std::swap(*it, *(it + 2));
	}

	for (auto it = vertices.begin(); it != vertices.end(); ++it)
	{
		it->_texCoords.x = (1.f - it->_texCoords.x);
	}
}

std::shared_ptr<Resource> MeshLoader::CreateSimpleMesh(const std::string& resourceName)
{
	GeometryGenerator generator;
	GeometryGenerator::MeshData meshData;
	BoundingShapeType bounderShape = BoundingShapeType::Box;

	if (resourceName == kBoxMeshName)
	{
		generator.CreateBox(1.0f, 1.0f, 1.0f, meshData);
	}
	else if (resourceName == kCylinderMeshName)
	{
		generator.CreateCylinder(0.5f, 0.5f, 2.0f, 32U, 1U, meshData);
	}
	else if (resourceName == kPlaneMeshName)
	{
		generator.CreateGrid(1.0f, 1.0f, 2U, 2U, meshData);
	}
	else if (resourceName == kSphereMeshName)
	{
		generator.CreateSphere(0.5f, 32U, 32U, meshData);
		bounderShape = BoundingShapeType::Sphere;
	}
	else
	{
		LOG_E("this is not a simple resource", 0);
		return std::shared_ptr<Resource>();
	}

	std::vector<Vertex> vertices;
	std::vector<WORD> indices;

	vertices.reserve(meshData.Vertices.size());
	indices.reserve(meshData.Indices.size());

	std::for_each(meshData.Vertices.begin(), meshData.Vertices.end(), [&](const GeometryGenerator::Vertex& vertex)
	{
		vertices.push_back(Vertex(vertex.Position, MyColors::White, vertex.TexC, vertex.Normal, vertex.TangentU));
	});

	indices.assign(meshData.Indices.begin(), meshData.Indices.end());

	ReverseWinding(indices, vertices);

	auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), vertices.data(), vertices.size(), sizeof(Vertex));
	auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), indices.data(), indices.size());


	auto pair = std::make_pair(vertexBuffer, indexBuffer);
	SubMeshPtr subMesh(DBG_NEW SubMesh(pair, D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	std::shared_ptr<Mesh> resource(DBG_NEW Mesh(resourceName));
	resource->AddSubMesh(subMesh);

	// bounder calculation
	if (bounderShape == BoundingShapeType::Box)
	{
		subMesh->_BoundingShapeType = bounderShape;

		std::shared_ptr<BoundingBox> boundingBox(DBG_NEW BoundingBox);
		BoundingBox::CreateFromPoints(*boundingBox, vertices.size(), (Vector3*)(vertices.data()), sizeof(Vertex));

		subMesh->_BoundingBox = boundingBox;
	}
	else if (bounderShape == BoundingShapeType::Sphere)
	{
		subMesh->_BoundingShapeType = bounderShape;

		std::shared_ptr<BoundingSphere> boundingSphere(DBG_NEW BoundingSphere);
		BoundingSphere::CreateFromPoints(*boundingSphere, vertices.size(), (Vector3*)(vertices.data()), sizeof(Vertex));

		subMesh->_BoundingSphere = boundingSphere;
	}

	return std::static_pointer_cast<Resource>(resource);
}

VertexIndexPair MeshLoader::CreateBox()
{
	std::vector<Vertex> vertecies;
	vertecies.reserve(6 * 4);	// 4 vertices per face and we have 6 faces for a cube -> 6 (faces) * 4 ( vertices per face)

	// position	/	color	/	texture		/ normal
	
	// front face
	/*0*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, 0.5f), MyColors::White, Vector2::Zero, Vector3::Backward));
	/*1*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, 0.5f), MyColors::White, Vector2(1.0f, 0.0f), Vector3::Backward));
	/*2*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, 0.5f), MyColors::White, Vector2(0.0f, 1.0f), Vector3::Backward));
	/*3*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, 0.5f), MyColors::White, Vector2::One, Vector3::Backward));
	
	// back face
	/*4*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, -0.5f), MyColors::White, Vector2::Zero, Vector3::Forward));
	/*5*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, -0.5f), MyColors::White, Vector2::UnitX, Vector3::Forward));
	/*6*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, -0.5f), MyColors::White, Vector2::UnitY, Vector3::Forward));
	/*7*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, -0.5f), MyColors::White, Vector2::One, Vector3::Forward));

	// top face
	/*8*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, -0.5f), MyColors::White, Vector2::Zero, Vector3::Up));
	/*9*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, -0.5f), MyColors::White, Vector2::UnitX, Vector3::Up));
	/*10*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, 0.5f), MyColors::White, Vector2::UnitY, Vector3::Up));
	/*11*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, 0.5f), MyColors::White, Vector2::One, Vector3::Up));

	// bottom face
	/*12*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, 0.5f), MyColors::White, Vector2::Zero, Vector3::Up));
	/*13*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, 0.5f), MyColors::White, Vector2::UnitX, Vector3::Up));
	/*14*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, -0.5f), MyColors::White, Vector2::UnitY, Vector3::Up));
	/*15*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, -0.5f), MyColors::White, Vector2::One, Vector3::Up));

	// left face
	/*16*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, -0.5f), MyColors::White, Vector2::Zero, Vector3::Left));
	/*17*/vertecies.emplace_back(Vertex(Vector3(-0.5f, 0.5f, 0.5f), MyColors::White, Vector2::UnitX, Vector3::Left));
	/*18*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, -0.5f), MyColors::White, Vector2::UnitY, Vector3::Left));
	/*19*/vertecies.emplace_back(Vertex(Vector3(-0.5f, -0.5f, 0.5f), MyColors::White, Vector2::One, Vector3::Left));

	// right face
	/*20*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, 0.5f), MyColors::White, Vector2::Zero, Vector3::Right));
	/*21*/vertecies.emplace_back(Vertex(Vector3(0.5f, 0.5f, -0.5f), MyColors::White, Vector2::UnitX, Vector3::Right));
	/*22*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, 0.5f), MyColors::White, Vector2::UnitY, Vector3::Right));
	/*23*/vertecies.emplace_back(Vertex(Vector3(0.5f, -0.5f, -0.5f), MyColors::White, Vector2::One, Vector3::Right));

	std::vector<WORD> indices =
	{
		0, 1, 2,
		2, 1, 3,

		4, 5, 6,
		6, 5, 7,

		8, 9, 10,
		10, 9, 11,

		12, 13, 14,
		14, 13, 15,

		16, 17, 18,
		18, 17, 19,

		20, 21, 22,
		22, 21, 23,
	};

	auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), &vertecies[0], vertecies.size(), sizeof(Vertex));
	auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), &indices[0], indices.size());

	DirectX::SetDebugObjectName(vertexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyBoxVertexBuffer");
	DirectX::SetDebugObjectName(indexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyBoxIndexBuffer");

	return std::make_pair(vertexBuffer, indexBuffer);
}

VertexIndexPair MeshLoader::CreatePlane()
{
	std::vector<Vertex> verts;
	verts.reserve(4);

	verts.emplace_back(Vertex(Vector3(-0.5f, 0.0f, -0.5f), MyColors::White, Vector2::Zero, Vector3::Up));
	verts.emplace_back(Vertex(Vector3(0.5f, 0.0f, -0.5f), MyColors::White, Vector2::UnitX, Vector3::Up));
	verts.emplace_back(Vertex(Vector3(-0.5f, 0.0f, 0.5f), MyColors::White, Vector2::UnitY, Vector3::Up));
	verts.emplace_back(Vertex(Vector3(0.5f, 0.0f, 0.5f), MyColors::White, Vector2::One, Vector3::Up));

	std::vector<WORD> indices =
	{
		0, 1, 2,
		2, 1, 3,
	};

	auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), &verts[0], verts.size(), sizeof(Vertex));
	auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), &indices[0], indices.size());

	DirectX::SetDebugObjectName(vertexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyPlaneComponentIndexBuffer");
	DirectX::SetDebugObjectName(indexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyPlaneComponentVertexBuffer");

	return std::make_pair(vertexBuffer, indexBuffer);
}

VertexIndexPair MeshLoader::CreateSphere()
{
	using namespace DirectX;

	std::vector<Vertex> vertices;
	std::vector<WORD> indices;

	auto verticalSegments = kTessellation;
	auto horizontalSegments = kTessellation * 2;

	float raduis = 1 / 2.0f;

	for (size_t i = 0; i <= verticalSegments; i++)
	{
		float v = 1 - (float)i / verticalSegments;
		float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
		float dy, dxz;

		XMScalarSinCos(&dy, &dxz, latitude);

		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			float u = (float)j / horizontalSegments;
			float longitude = j * XM_2PI / horizontalSegments;
			float dx, dz;

			XMScalarSinCos(&dx, &dz, longitude);

			dx *= dxz;
			dz *= dxz;

			XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
			XMVECTOR texCoord = XMVectorSet(u, v, 0, 0);

			vertices.emplace_back(Vertex(normal * raduis, MyColors::White, texCoord, normal));
		}
	}

	size_t stride = horizontalSegments + 1;
	for (size_t i = 0; i < verticalSegments; i++)
	{
		for (size_t j = 0; j <= horizontalSegments; j++)
		{
			size_t nextI = i + 1;
			size_t nextJ = (j + 1) % stride;

			indices.push_back(i * stride + j);
			indices.push_back(nextI * stride + j);
			indices.push_back(i * stride + nextJ);

			indices.push_back(i * stride + nextJ);
			indices.push_back(nextI * stride + j);
			indices.push_back(nextI * stride + nextJ);
		}
	}

	auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), &vertices[0], vertices.size(), sizeof(Vertex));
	auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), &indices[0], indices.size());

	DirectX::SetDebugObjectName(vertexBuffer->GetRawBuffer().Get(), "TheOneAndOnlySphereComponentVertexBuffer");
	DirectX::SetDebugObjectName(indexBuffer->GetRawBuffer().Get(), "TheOneAndOnlySphereComponentIndexBuffer");

	return std::make_pair(vertexBuffer, indexBuffer);
}

Vector3 GetCircleVector(size_t i, size_t tesselation)
{
	float angle = i * DirectX::XM_2PI / tesselation;
	float dx, dz;

	DirectX::XMScalarSinCos(&dx, &dz, angle);

	Vector3 v(dx, 0, dz);
	return v;
}

void BuildCap(std::vector<Vertex>& vertices, std::vector<WORD>& indices, bool isTop, float theHeight, float diamater, size_t tess)
{
	using namespace DirectX;

	for (size_t i = 0; i < tess - 2; i++)
	{
		size_t i1 = (i + 1) % tess;
		size_t i2 = (i + 2) % tess;

		if (isTop)
		{
			std::swap(i1, i2);
		}

		size_t vbase = vertices.size();
		indices.push_back(vbase);
		indices.push_back(vbase + i1);
		indices.push_back(vbase + i2);

		XMVECTOR normal = g_XMIdentityR1;
		XMVECTOR texScale = g_XMNegativeOneHalf;

		if (!isTop)
		{
			normal = -normal;
			texScale *= g_XMNegateX;
		}

		float raduis = diamater / 2;
		float height = theHeight / 2;

		for (size_t i = 0; i < tess; i++)
		{
			XMVECTOR circleVector = GetCircleVector(i, tess);
			XMVECTOR position = (circleVector * raduis) + (normal * height);
			XMVECTOR texCoord = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), texScale, g_XMOneHalf);

			vertices.push_back(Vertex(position, MyColors::White, texCoord, normal));
		}
	}
}

VertexIndexPair MeshLoader::CreateCylinder()
{
	std::vector<Vertex> vertices;
	std::vector<WORD> indices;

	float height = kCylinderHeight / 2.0f;
	Vector3 topOffset = Vector3::UnitY * height;

	float raduis = kCylinderDiamater / 2;
	size_t stride = kTessellation + 1;

	for (size_t i = 0; i <= kTessellation; i++)
	{
		Vector3 normal = GetCircleVector(i, kTessellation);
		Vector3 sideOffset = normal * raduis;
		float u = (float)i / kTessellation;

		DirectX::XMVECTOR texCoord = DirectX::XMLoadFloat(&u);

		vertices.push_back(Vertex(sideOffset + topOffset, MyColors::White, texCoord, normal));
		vertices.push_back(Vertex(sideOffset - topOffset, MyColors::White, texCoord + DirectX::g_XMIdentityR1, normal));

		indices.push_back(i * 2);
		indices.push_back((i * 2 + 2) % (stride * 2));
		indices.push_back(i * 2 + 1);

		indices.push_back(i * 2 + 1);
		indices.push_back((i * 2 + 2) % (stride * 2));
		indices.push_back((i * 2 + 3) % (stride * 2));
	}

	BuildCap(vertices, indices, true, kCylinderHeight, kCylinderDiamater, kTessellation);
	BuildCap(vertices, indices, false, kCylinderHeight, kCylinderDiamater, kTessellation);

	auto vertexBuffer = VertexBuffer::Create(GraphicsDevice::GetPtr(), &vertices[0], vertices.size(), sizeof(Vertex));
	auto indexBuffer = IndexBuffer::Create(GraphicsDevice::GetPtr(), &indices[0], indices.size());

	DirectX::SetDebugObjectName(vertexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyCylinderComponentVertexBuffer");
	DirectX::SetDebugObjectName(indexBuffer->GetRawBuffer().Get(), "TheOneAndOnlyCylinderComponentIndexBuffer");

	return std::make_pair(vertexBuffer, indexBuffer);
}


#pragma endregion