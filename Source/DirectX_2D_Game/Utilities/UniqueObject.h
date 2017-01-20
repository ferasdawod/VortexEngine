#pragma once

#include <Utilities/ObjectId.h>
#include <Utilities/IdGenerator.h>

class UniqueObject
{
public:
	UniqueObject() { _id = IdGenerator::GenerateId(); }
	virtual ~UniqueObject() {}

	const ObjectId& GetId() const { return _id; }
	bool IsValid() const { return _id.IsValid(); }

protected:
	ObjectId _id;
};
