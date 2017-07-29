#include "pch.h"

#include "AssetManager.h"
#include <stack>
#include <string.h>
#include "Components/MeshRenderer.h"

bool Core::AssetManager::ListFiles(std::string path, const std::string& mask, std::vector<string>& files) 
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA ffd;
	std::string spec;
	std::stack<std::string> directories;

	directories.push(path);
	files.clear();

	while (!directories.empty()) {
		path = directories.top();
		spec = path + "\\" + mask;
		directories.pop();

		hFind = FindFirstFile(spec.c_str(), &ffd);
		if (hFind == INVALID_HANDLE_VALUE) 
		{
			return false;
		}

		do 
		{
			if (strcmp(ffd.cFileName, ".") != 0 && strcmp(ffd.cFileName, "..") != 0)
			{
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
				{
					directories.push(path + "\\" + ffd.cFileName);
				}
				else 
				{
					files.push_back(path + "\\" + ffd.cFileName);
				}
			}

		} while (FindNextFile(hFind, &ffd) != 0);

		if (GetLastError() != ERROR_NO_MORE_FILES) 
		{
			FindClose(hFind);
			return false;
		}

		FindClose(hFind);
		hFind = INVALID_HANDLE_VALUE;
	}

	return true;
}

void Core::AssetManager::AddDefaultMeshes()
{
	_meshes.push_back("Box.mesh");
	_meshes.push_back("Cylinder.mesh");
	_meshes.push_back("Plane.mesh");
	_meshes.push_back("Sphere.mesh");
}

void Core::AssetManager::LoadAssets()
{
	ListFiles("Assets\\Textures", "*", _textures);
	ListFiles("Assets\\Actors", "*", _actors);
	ListFiles("Assets\\Materials", "*", _materials);
	ListFiles("Assets\\Scripts", "*.lua", _scripts);
	ListFiles("Assets\\Levels", "*", _levels);
	ListFiles("Assets\\Sounds", "*", _sounds);
	
	AddDefaultMeshes();
	std::vector<string> tempMeshesVector;
	ListFiles("Assets\\Meshes", "*", tempMeshesVector);
	for (auto itr = tempMeshesVector.begin(); itr != tempMeshesVector.end(); ++itr)
	{
		if (!Utils::WildcardMatch("*.meta", *itr))
		{
			_meshes.push_back(*itr);
		}
	}
}
