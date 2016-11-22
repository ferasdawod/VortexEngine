#pragma once

#include <memory>

class Resource;

class ResHandle
{
	friend class ResCache;

public:
	virtual ~ResHandle() {}

	std::weak_ptr<Resource>	GetResource() const { return std::weak_ptr<Resource>(_pResource); }

private:
	ResHandle() {}
	std::shared_ptr<Resource>	_pResource;

	ResCache*					_pOwnerCache;
};