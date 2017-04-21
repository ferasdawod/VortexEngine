#include "pch.h"

#include "GuiController.h"

#include <3rd Party/imgui/ImGuizmo.h>
#include <Components/Transform.h>
#include <Components/Camera.h>

#include "3rd Party/imgui/imgui_impl_dx11.h"
#include "Graphics/Rendering/GraphicsDevice.h"

#include <Level/Level.h>

#include <Debugging/Logger.h>

#include <Input/Input.h>

namespace ImGuizmo {
	struct matrix_t;
}

inline void SetupImGuiStyle(bool bStyleDark_, float alpha_)
{
	ImGuiStyle& style = ImGui::GetStyle();

	// light style from Pacôme Danhiez (user itamago) https://github.com/ocornut/imgui/pull/511#issuecomment-175719267
	style.Alpha = 1.0f;
	style.FrameRounding = 3.0f;
	style.Colors[ImGuiCol_Text] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.94f, 0.94f, 0.94f, 0.94f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.96f, 0.96f, 0.96f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.51f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.82f, 0.82f, 0.82f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.98f, 0.98f, 0.98f, 0.53f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.49f, 0.49f, 0.49f, 1.00f);
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.86f, 0.86f, 0.86f, 0.99f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.78f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.59f, 0.59f, 0.59f, 0.50f);
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

	if (bStyleDark_)
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			float H, S, V;
			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, H, S, V);

			if (S < 0.1f)
			{
				V = 1.0f - V;
			}
			ImGui::ColorConvertHSVtoRGB(H, S, V, col.x, col.y, col.z);
			if (col.w < 1.00f)
			{
				col.w *= alpha_;
			}
		}
	}
	else
	{
		for (int i = 0; i <= ImGuiCol_COUNT; i++)
		{
			ImVec4& col = style.Colors[i];
			if (col.w < 1.00f)
			{
				col.x *= alpha_;
				col.y *= alpha_;
				col.z *= alpha_;
				col.w *= alpha_;
			}
		}
	}
}

Core::GuiController::GuiController() : _isDarkTheme(true), _windowsAlpha(0.9f), _bShowAboutWindow(false), _selectedActorId(-1)
{
	HandleRegistering(true);
}

Core::GuiController::~GuiController()
{
	HandleRegistering(false);
}

bool Core::GuiController::Initialize(std::weak_ptr<IWindow> window, std::weak_ptr<Level> level)
{
	_pWindow = window;
	_pLevel = level;

	auto win = window.lock();
	auto device = GraphicsDevice::GetPtr();

	auto result = ImGui_ImplDX11_Init(win->GetHandle(), device->GetDevice().Get(), device->GetContext().Get());

	auto windowInformation = win->GetInformation();
	_windowWidth = windowInformation.Width;
	_windowHeight = windowInformation.Heigt;

	SetupImGuiStyle(_isDarkTheme, _windowsAlpha);

	ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	io.Fonts->AddFontFromFileTTF("Assets/Fonts/Roboto-Regular.ttf", 14.f);

	return result;
}

void Core::GuiController::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
}

void Core::GuiController::Render()
{
	SetupImGuiStyle(_isDarkTheme, _windowsAlpha);

	ImGui_ImplDX11_NewFrame();
	ImGuizmo::BeginFrame();

	//ImGui::ShowTestWindow();

	DrawMenuBar();
	DrawActorsWindow();
	DrawAssetsWindow();
	DrawLogWindow();
	DrawPropertiesWindow();

	DrawExtraWindows();
}

bool Core::GuiController::HandleEvent(StrongEventDataPtr eventData)
{
	auto eventId = eventData->GetID();
	if (eventId == Event_WindowResized::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_WindowResized>(eventData);

		_windowWidth = casted->Width();
		_windowHeight = casted->Height();
	}
	else if (eventId == Event_NewCamera::kEventID)
	{
		auto casted = std::dynamic_pointer_cast<Event_NewCamera>(eventData);

		_pCamera = casted->GetCamera();
	}

	return true;
}

void Core::GuiController::HandleRegistering(bool isRegistering)
{
	ToggleRegisteration(Event_WindowResized::kEventID, isRegistering);
	ToggleRegisteration(Event_NewCamera::kEventID, isRegistering);
}

void Core::GuiController::DrawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "CTRL+N"))
			{
				LOG_M("New Pressed");
			}

			if (ImGui::MenuItem("Save", "CTRL+S"))
			{
				LOG_M("Save Pressed");
			}

			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "ALT+F4"))
			{
				auto window = _pWindow.lock();
				PostMessage(window->GetHandle(), WM_CLOSE, 0, 0);
				LOG_M("Exit Pressed");
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Use Dark Theme", nullptr, &_isDarkTheme);
			ImGui::SliderFloat("Windows Alpha", &_windowsAlpha, 0.f, 1.f);

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About", "CTRL+I"))
			{
				_bShowAboutWindow = true;
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Core::GuiController::DrawActorsWindow()
{
	auto level = _pLevel.lock();
	if (!level) return;

	const std::vector<StrongActorPtr>& actors = level->GetActors();


	ImGui::SetNextWindowPos(ImVec2(0, 25));
	ImGui::SetNextWindowSize(ImVec2(_windowWidth * 0.15f, _windowHeight - 30));

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::Begin("Actors", nullptr, windowFlags);

	if (ImGui::TreeNode("Scene Root"))
	{
		for (auto actor : actors)
		{
			auto name = actor->GetName();

			// gizmo testing

			if (actor->GetId().GetUnderlyingValue() == _selectedActorId)
			{
				auto camera = _pCamera.lock();
				if (!camera) continue;;

				static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
				static ImGuizmo::MODE mode = ImGuizmo::WORLD;

				if (!Input::IsMouseButtonDown(MouseKeys::RightButton))
				{
					if (Input::IsKeyPressed(KeyCode::Q))
						operation = ImGuizmo::TRANSLATE;
					else if (Input::IsKeyPressed(KeyCode::W))
						operation = ImGuizmo::ROTATE;
					else if (Input::IsKeyPressed(KeyCode::E))
						operation = ImGuizmo::SCALE;

					if (Input::IsKeyPressed(KeyCode::Z))
						mode = ImGuizmo::WORLD;
					else if (Input::IsKeyPressed(KeyCode::X))
						mode = ImGuizmo::LOCAL;
				}
				auto transform = actor->GetComponent<Transform>().lock();

				auto position = transform->GetPosition();
				Quaternion rotation;
				auto scale = transform->GetScale();


				auto matrix = transform->GetWorldMat();
				auto mat = (float*)matrix.m;

				auto& io = ImGui::GetIO();

				auto viewMatrix = camera->GetViewMatrix();
				auto projectionMatrix = camera->GetProjectionMatrix();

				auto view = (float*)viewMatrix.m;
				auto projection = (float*)projectionMatrix.m;


				ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
				ImGuizmo::Manipulate(view, projection, operation, mode, mat);

				matrix.Decompose(scale, rotation, position);

				if (operation == ImGuizmo::TRANSLATE)
					transform->SetPosition(position);
				else if (operation == ImGuizmo::ROTATE)
					transform->SetRotation(rotation);
				else 
					transform->SetScale(scale);
			}


			// end gizmo testing

			/*if (ImGui::TreeNode(actor.get(), name.c_str()))
			{
				ImGui::SameLine(200);
				ImGui::Button("Edit");

				const std::vector<StrongComponentPtr>& components = actor->GetComponents();

				for (auto component : components)
				{
					ImGui::Text(component->GetName().c_str());
					ImGui::SameLine(200);
					ImGui::Button("Edit");
				}

				ImGui::TreePop();
			}*/

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | (_selectedActorId == actor->GetId().GetUnderlyingValue() ? ImGuiTreeNodeFlags_Selected : 0);
			ImGui::TreeNodeEx(actor.get(), node_flags | ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen, name.c_str());
			if (ImGui::IsItemClicked())
			{
				_selectedActorId = actor->GetId().GetUnderlyingValue();
				_pSelectedActor = actor;
			}
		}

		ImGui::TreePop();
	}

	ImGui::End();
}

void Core::GuiController::DrawAssetsWindow()
{
}

void Core::GuiController::DrawLogWindow()
{
}

void Core::GuiController::DrawPropertiesWindow()
{
	auto actor = _pSelectedActor.lock();
	if (!actor) return;


	auto width = _windowWidth * 0.3f;
	auto height = _windowHeight;

	ImGui::SetNextWindowPos(ImVec2(_windowWidth - width, 25));
	ImGui::SetNextWindowSize(ImVec2(width, height - 30));

	ImGuiWindowFlags windowFlags = 0;
	windowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::Begin("Properties", nullptr, windowFlags);

	auto name = actor->GetName();

	if (ImGui::CollapsingHeader("Actor"))
	{
		ImGui::Text("Name");
		ImGui::SameLine(100);

		auto name = actor->GetName();
		name.reserve(255);
		ImGui::InputText("##name", &name[0], 255);
		actor->SetName(name);
	}

	auto& components = actor->GetComponents();
	for (auto component : components)
	{
		if (ImGui::CollapsingHeader(component->GetName().c_str()))
		{
			auto& props = component->GetProperties();
			for (auto& prop : props)
			{
				ImGui::Text(prop.GetName());
				ImGui::SameLine(100);

				string* str;
				char* buff;
				auto id = "##" + std::to_string((int)prop.GetValue());

				switch (prop.GetType())
				{
				case PropertyType::Float:
					ImGui::DragFloat(id.c_str(), (float*)prop.GetValue(), 0, 100);
					break;
				case PropertyType::Int:
					ImGui::DragInt(id.c_str(), (int*)prop.GetValue(), 0, 100);
					break;
				case PropertyType::Bool:
					ImGui::Checkbox(id.c_str(), (bool*)prop.GetValue());
					break;
				case PropertyType::Color:
					ImGui::ColorEdit4(id.c_str(), (float*)prop.GetValue(), false);
					break;
				case PropertyType::String: 
					str = (string*)prop.GetValue();
					buff = &((*str)[0]);
					ImGui::InputText(id.c_str(), buff, 255);
					
					break;
				case PropertyType::Vector2: break;
				case PropertyType::Vector3:
					ImGui::DragFloat3(id.c_str(), (float*)prop.GetValue(), 0, 100);
					break;
				case PropertyType::Vector4: break;
				default:;
				}
			}
		}
	}


	ImGui::End();
}

void Core::GuiController::DrawExtraWindows()
{
	if (_bShowAboutWindow)
	{
		//ImGui::SetNextWindowPos(ImVec2(200, 200));
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_Modal | ImGuiWindowFlags_AlwaysAutoResize;
		ImGui::Begin("About", &_bShowAboutWindow, windowFlags);
		ImGui::Text("Hello and welcome to our great editor");
		ImGui::End();
	}
}
