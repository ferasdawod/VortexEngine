#pragma once

#include "SubMesh.h"
#include "Resources/Resource.h"

#include <vector>
#include <memory>
#include <string>

using SubMeshPtr = std::shared_ptr<SubMesh>;

class Mesh : public Resource
{
	friend class MeshLoader;
	friend class MeshRenderer;

private:
	Mesh(const std::string& name) : Resource(name) {}

public:
	virtual ~Mesh() = default;

	SubMeshPtr	GetSubMesh(size_t index)
	{
		assert(index < _subMeshes.size());

		return _subMeshes[index];
	}
	size_t		GetSubMeshesCount() const { return _subMeshes.size(); }

private:
	void	AddSubMesh(SubMeshPtr subMesh) { _subMeshes.emplace_back(subMesh); }

protected:
	std::vector<SubMeshPtr>	_subMeshes;
};