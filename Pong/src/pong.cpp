#include"banana/engine.h"
#include"banana/main.h"
#include"banana/log.h"
#include"banana/input/keyboard.h"
#include"banana/graphics/framebuffer.h"
#include"external/imgui/imgui.h"
#include"factory.h"
#include"banana/graphics/GameObject.h"
#include "external/json.hpp"
#include <fstream>
#include "banana/graphics/GameObjects/Circle.h"
#include "banana/graphics/GameObjects/Rectangle.h"
#include "banana/graphics/Serialization.h"
#include "banana/Physics/physics.h"
#include "unordered_map"
#include"banana/graphics/camera.h"
#include "ClientApp.h"


using namespace banana;
using json = nlohmann::json;


class Pong :public App
{
public:
	

	ClientApp* userapp = new ClientApp();
	int selectedObjectIndex_add = 0;
	int selectedObjectIndex = 0;
	std::vector<banana::graphics::Gameobject*> Objects;
	std::vector<banana::graphics::Gameobject*> InGame_Objects;
	std::unordered_map<std::string, banana::graphics::Gameobject*> Object_Map;
	std::shared_ptr<graphics::Camera2D> m_Camera2D;
	glm::vec3 m_CamPos;
	float m_CamRot;
	float m_CamSpeed;
	bool wireframe_mode = false;
	int leftScore = 0;
	int rightScore = 0;
	int m_PointsTowin = 5;
	bool inGamePlay = false;
	std::string winnerName;
	bool enable_physics = false;
	std::string path = "C:/Users/Jaydip/Downloads/banana-Editor-main/banana-Editor-main/bananaEditor/res/grass.png";

	core::WindowProperties getWindowProperties()	override
	{
		core::WindowProperties props;
		props.w = 800;
		props.h = 600;
		props.imguiProps.isDockingEnable = true;
		return props;
	}

	void initialize() override
	{

		 //Deserialization 
		
		Objects = Deserialize();
		

		m_Camera2D = std::make_shared<banana::graphics::Camera2D>();
		m_Camera2D->setHeight(2.f);
		m_CamPos = { 0.f,0.f, 0.f };
		m_Camera2D->setView(m_CamPos, m_CamRot);
		m_CamSpeed = 0.01f;
		

		
		
	
		reset(false);
		
	}
	void update(float delta) override
	{
		float wheel = ImGui::GetIO().MouseWheel;
		if (wheel > 0 && input::Keyboard::key(BANANA_INPUT_KEY_F))
		{
			m_Camera2D->setHeight(m_Camera2D->getHeight()+0.04f); // Increase the value when the mouse wheel is scrolled up
		}
		if (wheel < 0 && input::Keyboard::key(BANANA_INPUT_KEY_F))
		{
			m_Camera2D->setHeight(m_Camera2D->getHeight() - 0.04f); // Decrease the value when the mouse wheel is scrolled down
		}
		if (input::Keyboard::key(BANANA_INPUT_KEY_RIGHT))
		{
			m_CamPos.x += m_CamSpeed;
		}
		if (input::Keyboard::key(BANANA_INPUT_KEY_LEFT))
		{
			m_CamPos.x -= m_CamSpeed;
		}

		m_Camera2D->setView(m_CamPos, m_CamRot);
		if (input::Keyboard::keyDown(BANANA_INPUT_KEY_ESCAPE))
		{
			Engine::Instance().quit();
			return;
		}
		
		
		if (inGamePlay)
		{
			if (enable_physics)
			{
				banana::physics::update(InGame_Objects);
			}
			for (int i = 0; i < InGame_Objects.size(); i++)
			{
				InGame_Objects[i]->update();
			}
			userapp->Update(Object_Map,m_Camera2D);
			
		
		}
		else
		{
			for (int i = 0; i < Objects.size(); i++)
			{
				Objects[i]->update();
			}
		}
	}
	void shutdown() override
	{
		Serialize(Objects);
		BANANA_INFO("Shutting Down");
		
		
	}
	void render() override
	{
		auto& rm = Engine::Instance().getRenderManager();
		rm.submit(BANANA_SUBMIT_RC(PushCamera, m_Camera2D));
		if (inGamePlay)
		{
			for (int i = 0; i < InGame_Objects.size(); i++)
			{
				InGame_Objects[i]->render();	
			}
		}
		else
		{
			for (int i = 0; i < Objects.size(); i++)
			{
				Objects[i]->render();
			
			}
		}
		rm.submit(BANANA_SUBMIT_RC(PopCamera));
	}

	void imguiRender() override
	{

	
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		userapp->ImGuiRender();
		if (ImGui::Begin("Controls"))
		{
			if (ImGui::RadioButton("wireframe Mode", wireframe_mode))
			{
				auto& rm = Engine::Instance().getRenderManager();
				rm.setWireframeMode(true);
				wireframe_mode = true;
			}
			if (ImGui::RadioButton("Fill Mode", !wireframe_mode))
			{
				auto& rm = Engine::Instance().getRenderManager();
				rm.setWireframeMode(false);
				wireframe_mode = false;
			}
	
		}
		ImGui::End();



		const char* objectNames[] = { "Circle", "Rectangle" }; // Add more object names as needed
		  // Index of the selected object type

		// Inside your game engine's main loop or update function:

		// Start a new ImGui frame
		ImGui::Begin("Add Object");

		// Create a dropdown list with the object names
		if (ImGui::BeginCombo("Object Type", objectNames[selectedObjectIndex_add], 0))
		{
			for (int i = 0; i < IM_ARRAYSIZE(objectNames); i++)
			{
				bool isSelected = (selectedObjectIndex_add == i);
				if (ImGui::Selectable(objectNames[i], isSelected))
				{
					// Set the selected object index
					selectedObjectIndex_add = i;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		static char name[256] = "";
		// Input field for object name
		ImGui::InputText("Object Name", name, IM_ARRAYSIZE(name));

		// Create a button to create the selected object
		if (ImGui::Button("Create Object"))
		{
			std::string objectName = name;
			if (!objectName.empty())
			{
				if (selectedObjectIndex_add == 0)
				{
					// Create a circle object
					Objects.push_back(new banana::graphics::Circle(0.4f, objectName, true));
				}
				else if (selectedObjectIndex_add == 1)
				{
					// Create a rectangle object
					Objects.push_back(new banana::graphics::Rectangle(objectName, true));
				}
				BANANA_INFO(objectName);
			}
		}

		// End the ImGui frame
		ImGui::End();



		bool showObjectInfo = true; // Flag to control visibility of the window

		ImGui::Begin("Object Information", &showObjectInfo, ImGuiWindowFlags_MenuBar); // Add a menu bar to the window
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Options")) // Add a drop-down menu for options
				{
					if (ImGui::MenuItem("Delete Selected")) // Add a delete selected option
					{
						if (selectedObjectIndex >= 0 && selectedObjectIndex < Objects.size()) {

							delete Objects[selectedObjectIndex];
							Objects.erase(Objects.begin() + selectedObjectIndex); // Delete selected object from the list
							selectedObjectIndex = -1; // Reset selected object index
						}
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			// Display a list of created objects
			for (int i = 0; i < Objects.size(); i++) {
				banana::graphics::Gameobject* obj = Objects[i];

				if (ImGui::Selectable(obj->name.c_str(), selectedObjectIndex == i)) {
					selectedObjectIndex = i; // Update selected object index
				}
			}

			ImGui::Separator();

			if (selectedObjectIndex >= 0 && selectedObjectIndex < Objects.size()) {
				// Display information of the selected object
				banana::graphics::Gameobject* obj = Objects[selectedObjectIndex];

				ImGui::InputText("Name", &obj->name[0], obj->name.size() + 1, ImGuiInputTextFlags_EnterReturnsTrue);
				ImGui::InputFloat("X", &obj->transform.position.x);
				ImGui::InputFloat("Y", &obj->transform.position.y);

				ImGui::InputFloat("SizeX", &obj->transform.scale.x);
				ImGui::InputFloat("SizeY", &obj->transform.scale.y);
				ImGui::InputFloat("Velocity_X", &obj->velocity.x);
				ImGui::InputFloat("Velocity_Y", &obj->velocity.y);
				ImGui::InputFloat("Mass", &obj->mass);
				ImGui::ColorEdit4("Color", (float*)&obj->color);
				ImGui::Checkbox("IsRigid", &obj->IsRigid);
				bool useTexture = obj->use_texture; // Store the current value of useTexture
				if (ImGui::Checkbox("Use Texture", &useTexture)) {
					obj->use_texture = useTexture; // Update the useTexture value based on the checkbox
				}
				
				if (obj->use_texture) {
					// Show the input field for texture path
					
						
					ImGui::InputText("Texture Path", &path[0], path.size() + 1, ImGuiInputTextFlags_EnterReturnsTrue);
					
					if (ImGui::Button("Apply Changes"))
					{
						// If the "Apply Changes" button is pressed, update the path variable with the new value
						obj->SetTexturePath(path);
						
					}
					
					
				}
				
				
			}
		}
		ImGui::End();


	
		ImGui::StyleColorsDark(); // Use the built-in dark style as a base
		ImVec4 primaryColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); // Custom primary color
		ImVec4 secondaryColor = ImVec4(0.1f, 0.2f, 0.3f, 1.0f); // Custom secondary color
		ImVec4 accentColor = ImVec4(0.8f, 0.3f, 0.1f, 1.0f); // Custom accent color

		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_WindowBg] = primaryColor;
		style.Colors[ImGuiCol_Border] = secondaryColor;
		style.Colors[ImGuiCol_Button] = primaryColor;
		style.Colors[ImGuiCol_ButtonHovered] = accentColor;
		style.Colors[ImGuiCol_ButtonActive] = accentColor;
		style.Colors[ImGuiCol_FrameBg] = secondaryColor;
		style.Colors[ImGuiCol_FrameBgHovered] = secondaryColor;
		style.Colors[ImGuiCol_FrameBgActive] = secondaryColor;
		style.Colors[ImGuiCol_TitleBg] = primaryColor;
		style.Colors[ImGuiCol_TitleBgActive] = primaryColor;
		style.Colors[ImGuiCol_TitleBgCollapsed] = primaryColor;
		style.Colors[ImGuiCol_CheckMark] = accentColor;
		style.Colors[ImGuiCol_SliderGrab] = accentColor;
		style.Colors[ImGuiCol_SliderGrabActive] = accentColor;
		style.Colors[ImGuiCol_TextSelectedBg] = secondaryColor;

		if (ImGui::Begin("Game View"))
		{
			if (ImGui::IsWindowHovered())
			{
				ImGui::CaptureMouseFromApp(false);
				ImGui::CaptureKeyboardFromApp(false);
			}

			// Set cursor position
			ImGui::SetCursorPosX(20);

			// Set image size and UV coordinates
			ImVec2 size{ 640, 480 };
			ImVec2 uv0{ 0, 1 };
			ImVec2 uv1{ 1, 0 };

			auto& window = Engine::Instance().getWindow();
			ImGui::Image((ImTextureID)(intptr_t)window.getFramebuffer()->getTextureId(), size, uv0, uv1);
		}

		ImGui::End();

		if(ImGui::Begin("Menu"))
		{
			ImGui::Checkbox("Enable-Physics", &enable_physics);
			if (!inGamePlay)
			{
				if (ImGui::Button("Start Game"))
				{
					userapp->Initialize(Object_Map);
					InGame_Objects.clear();
					for (banana::graphics::Gameobject*& obj : Objects) {

						if (dynamic_cast<banana::graphics::Circle*>(obj))
						{
							banana::graphics::Circle* c = dynamic_cast<banana::graphics::Circle*>(obj);
							banana::graphics::Circle* newObj = new banana::graphics::Circle(*c); // Create a new object and copy the contents
							InGame_Objects.push_back(newObj); // Add the new object to vector u
						}
						else
						{
							banana::graphics::Rectangle* c = dynamic_cast<banana::graphics::Rectangle*>(obj);
							banana::graphics::Rectangle* newObj = new banana::graphics::Rectangle(*c); // Create a new object and copy the contents
							InGame_Objects.push_back(newObj);
						}
						
					}
					
					for (int i = 0; i < Objects.size(); i++)
					{
						Object_Map[Objects[i]->name] = InGame_Objects[i];
					}
				
					startGame();
				}
				ImGui::Text("%s ", winnerName.c_str());
			}
			else
			{
				if (ImGui::Button("Pause Game"))
				{
					inGamePlay = false;
				}
			}
		}
		ImGui::End();
	}
private:
	

	void startGame()
	{
		leftScore = 0;
		rightScore = 0;
		inGamePlay = true;
		winnerName = "";	
	}

	void handlePaddleCollision(int upkey, int downkey)
	{
		
	}
	void reset(bool leftScored)
	{
		
		
	}
};

App* createApp()
{
	return new Pong();
}
//predefined game objects , Shape  