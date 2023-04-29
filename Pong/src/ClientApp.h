#pragma once
#include"banana/engine.h"
#include"banana/main.h"
#include"banana/log.h"
#include"banana/input/keyboard.h"
#include"banana/graphics/framebuffer.h"
#include"external/imgui/imgui.h"

#include"banana/graphics/GameObject.h"
#include "external/json.hpp"
#include <fstream>
#include "banana/graphics/GameObjects/Circle.h"
#include "banana/graphics/GameObjects/Rectangle.h"
#include "banana/graphics/Serialization.h"
#include "banana/Physics/physics.h"
#include "unordered_map"
#include"banana/graphics/camera.h"
namespace banana
{
	class ClientApp
	{
		public:
		

			void Update(std::unordered_map<std::string,banana::graphics::Gameobject*> &Object_Map, std::shared_ptr<graphics::Camera2D> &m_Camera2D)
			{
				
			};
			void Initialize(std::unordered_map<std::string, banana::graphics::Gameobject*>& Object_Map)
			{
				
			};
			void ImGuiRender()
			{
				if (ImGui::Begin("Score"))
				{
					
				}
				ImGui::End();

			};
	};
}

