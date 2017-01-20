#pragma once

#include <Utilities/ObjectId.h>

class IdGenerator
{
public:
	static ObjectId GenerateId() { return ObjectId(_idCounter++); }

	static void SetIdSeed(unsigned long seed) { _idCounter = seed; }
	static unsigned long GetIdSeed() { return _idCounter; }

	static bool IsIdTaken(const ObjectId& id) { return id._value < _idCounter; }

private:
	static unsigned long _idCounter;
};

__declspec(selectany) unsigned long IdGenerator::_idCounter = 0;