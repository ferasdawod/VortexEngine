#pragma once

#include <Engine/Application.h>
#include <memory>


class MyApplication : public Core::Application
{
public:
	MyApplication() {}
	virtual ~MyApplication() {}

	void RegisterComponents(std::shared_ptr<ComponentFactory> componentFactory) override;
};
