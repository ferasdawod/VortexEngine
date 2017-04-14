#pragma once
#include "Graphics/Window/IWindow.h"
#include <Events/EventListener.h>

class GraphicsDevice;
class Level;
class Camera;

namespace Core
{
	class GuiController : public EventListener
	{
	public:
		GuiController();
		~GuiController();

		bool Initialize(std::weak_ptr<IWindow> window, std::weak_ptr<Level> level);
		void Shutdown();

		void Render();

		bool HandleEvent(StrongEventDataPtr eventData) override;
		void HandleRegistering(bool isRegistering) override;

	protected:
		void DrawMenuBar();
		void DrawActorsWindow();
		void DrawAssetsWindow();
		void DrawLogWindow();
		void DrawPropertiesWindow();

		void DrawExtraWindows();

	protected:
		std::weak_ptr<IWindow> _pWindow;
		std::weak_ptr<Level> _pLevel;
		std::weak_ptr<Camera> _pCamera;

		bool _isDarkTheme;
		float _windowsAlpha;

		bool _bShowAboutWindow;

		int _windowWidth;
		int _windowHeight;
	};
}