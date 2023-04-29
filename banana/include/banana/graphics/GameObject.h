#pragma once
#include "external/json.hpp"
#include "external/glm/glm.hpp"
#include "banana/graphics/vertex.h"
#include "banana/graphics/shader.h"
#include <vector>
#include <string>
#include "banana/graphics/texture.h"
using json = nlohmann::json;
struct Transform {
	public:
		glm::vec2 position;
		glm::vec2 angle;
		glm::vec2 scale;
};
namespace banana::graphics
{
	
	static const char* VertexShader = R"(
					#version 410 core
					layout (location = 0) in vec3 position;
					layout (location = 1) in vec2 texcoords;
                    out vec2 uvs;
					out vec3 vpos;
					uniform mat4 proj = mat4(1.0f);
                    uniform mat4 view = mat4(1.0f);
                    uniform mat4 model = mat4(1.0);

					void main()
					{
						vpos = position;
						uvs = texcoords;
						gl_Position = proj * view * model * vec4(position, 1.0);
						
					}
				)";

	static const char* FragmentShader = R"(
					#version 410 core
                    in vec2 uvs;
					out vec4 outColor;
                    uniform vec4 objectcolor;
                    uniform sampler2D tex;
					void main()
					{
						
						outColor = texture(tex, uvs);
					}
				)";
	class Gameobject
	{

		public:
			
			Gameobject();
			Gameobject(const Gameobject& other);
				
			Gameobject(std::string name,bool IsRigid);
			virtual json Serialize();
			virtual void Deserialize(const json& j);
			~Gameobject();
			virtual void update();
			virtual void render();
			virtual void Initiate();
			virtual void SetTexturePath(std::string &path);
			bool use_texture = true;
			std::shared_ptr<banana::graphics::Shader> mShader;
			std::shared_ptr<banana::graphics::VertexArray> va;
			std::shared_ptr<banana::graphics::Texture> m_texture;
			std::string tex_path;
			std::string name;
			std::string tex_name=name;
			int tex_unit = 1;
			Transform transform;
			bool IsRigid = true;
			glm::vec2 velocity;
			glm::vec4 color = { 1.0,0.0,0.0,1.0 };
			float Collider_radius;
			float mass = 10000;
			bool Enable_Gravity = false;
	};
}