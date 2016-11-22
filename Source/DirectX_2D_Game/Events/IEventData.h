#pragma once

#include <string>
#include <memory>

class IEventData;
using EventID = unsigned long;
using StrongEventDataPtr = std::shared_ptr<IEventData>;

class IEventData
{
public:
	virtual ~IEventData() {}
	virtual EventID					GetID() const = 0;
	virtual const std::string&		GetName() const = 0;
	virtual const std::string&		ToString() const = 0;
};

class BaseEventData : public IEventData
{
public:
	BaseEventData(const std::string& eventName, EventID eventID) : _name(eventName), _id(eventID) {}
	virtual ~BaseEventData() {}

	virtual EventID GetID() const override final { return _id; }
	virtual const std::string& GetName() const override final { return _name; }
	virtual const std::string& ToString() const override { static std::string data; return data; }

protected:
	std::string			_name;
	EventID				_id;
};