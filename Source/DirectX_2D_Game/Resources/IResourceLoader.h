#pragma once

#include <memory>
#include <string>

class Resource;

class IResourceLoader
{
public:
	virtual ~IResourceLoader() {}
	
	virtual std::shared_ptr<Resource>	LoadResource(const std::string& resourceName) = 0;
};