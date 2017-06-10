#pragma once

#include <vector>

namespace Core
{
	class AssetManager
	{
	public:
		AssetManager() {}
		virtual ~AssetManager() {}

	public:
		void LoadAssets();
		const std::vector<string>& GetTextures() const { return _textures; }
		const std::vector<string>& GetActors() const { return _actors; }
		const std::vector<string>& GetMaterials() const { return _materials; }
		const std::vector<string>& GetLevels() const { return _levels; }
		const std::vector<string>& GetMeshes() const { return _meshes; }

	protected:
		bool ListFiles(std::string path, const std::string& mask, std::vector<string>& files);

	protected:
		std::vector<string> _textures;
		std::vector<string> _actors;
		std::vector<string> _materials;
		std::vector<string> _levels;
		std::vector<string> _meshes;
	};
}