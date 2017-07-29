#pragma once

#include "Graphics/Window/IWindow.h"
#include <Events/EventListener.h>
#include "Components/ScriptComponent.h"

class GraphicsDevice;
class Level;
class Camera;
class Actor;
class ActorFactory;
class Material;
class MeshRenderer;

namespace Core
{
	class AssetManager;

	class GuiController : public EventListener
	{
	public:
		GuiController();
		~GuiController();

		bool Initialize(std::weak_ptr<IWindow> window, std::weak_ptr<Level> level, std::weak_ptr<ActorFactory> actorFactory);
		void Shutdown();

		void Render();

		bool HandleEvent(StrongEventDataPtr eventData) override;
		void HandleRegistering(bool isRegistering) override;

	protected:
		
		// Menu bar stuff
		void DrawMenuBar();
		void DrawFileMenu() const;
		void DrawViewMenu();
		void DrawActorsMenu();
		void DrawComponentsMenu() const;
		void DrawHelpMenu();



		void DrawActorsWindow();
		void DrawAssetsWindow();
		void DrawLogWindow();
		void DrawScriptProperties(std::shared_ptr<ScriptComponent> script_component);
		void DrawPropertiesWindow();

		void DrawMeshRendererProperties(std::shared_ptr<MeshRenderer> mesh_renderer);
		void DrawMaterialProperties(std::shared_ptr<Material> material) const;

		void DrawExtraWindows();

	protected:
		std::weak_ptr<IWindow> _pWindow;
		std::weak_ptr<Level> _pLevel;
		std::weak_ptr<Camera> _pCamera;
		std::weak_ptr<ActorFactory> _pActorFactory;

		std::unique_ptr<AssetManager> _pAssetManager;

		bool _isDarkTheme;
		float _windowsAlpha;

		bool _bShowAboutWindow;

		int _windowWidth;
		int _windowHeight;

		long _selectedActorId;
		std::weak_ptr<Actor> _pSelectedActor;
	};
}