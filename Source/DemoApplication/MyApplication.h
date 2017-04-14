#pragma once

#include <Engine/Application.h>
#include <memory>

#include "resource.h"


class MyApplication : public Core::Application
{
public:
	MyApplication() {}
	virtual ~MyApplication() {}

	void RegisterComponents(std::shared_ptr<ComponentFactory> componentFactory) override;
};
