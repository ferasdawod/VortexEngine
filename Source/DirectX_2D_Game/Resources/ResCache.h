#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <list>

#include "Utilities/Singleton.h"
#include "ResHandle.h"
#include "Resource.h"

class IResourceLoader;

using std::string;
using HandlesMap = std::unordered_map<string, std::shared_ptr<ResHandle>>;
using LoadersMap = std::unordered_map<string, std::shared_ptr<IResourceLoader>>;

class ResCache : public Singleton<ResCache>
{
public:
	ResCache() {}
	virtual ~ResCache() {}

	std::shared_ptr<ResHandle>	GetHandle(const string& resourceName);
	void						RegisterLoader(const string& pattern, std::shared_ptr<IResourceLoader> loader);

	void RegisterDefaultLoaders();

private:
	std::shared_ptr<ResHandle>			FindHandle(const string& resourceName);
	std::shared_ptr<ResHandle>			LoadResource(const string& resourceName);
	std::shared_ptr<IResourceLoader>	FindLoader(const string& resourceName);

private:
	HandlesMap			_handlesMap;
	LoadersMap			_loadersMap;
};