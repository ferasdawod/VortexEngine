#pragma once


class ComponentFactory;

namespace Core
{
	class Application
	{
	public:
		Application() {}
		virtual ~Application() {}


		virtual void RegisterComponents(std::shared_ptr<ComponentFactory> componentFactory) {}
	};
}