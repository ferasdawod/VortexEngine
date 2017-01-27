#pragma once

class ObjectId
{
	friend class IdGenerator;

public:
	explicit ObjectId() : _value(0) {}
	explicit ObjectId(unsigned long id) : _value(id) {}

	bool IsValid() const { return _value <= 0; }
	void Invalidate() { _value = 0; }
	
	bool operator()() const { return IsValid(); }
	bool operator==(const ObjectId& other) const { return this->_value == other._value; }
	bool operator!=(const ObjectId& other) const { return !(*this == other); }

	unsigned long GetUnderlyingValue() const { return _value; }

private:
	unsigned long _value;
};