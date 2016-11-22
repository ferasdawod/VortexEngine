#pragma once

#include <string>
using std::string;

class Resource
{
public:
	Resource(const string& name) : _sName(name) {}
	virtual ~Resource() {}

	const string&		GetName() const { return _sName; }

protected:
	string		_sName;
};