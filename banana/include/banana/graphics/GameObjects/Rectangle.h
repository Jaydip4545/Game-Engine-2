#pragma once
#include "banana/graphics/GameObject.h"
#include "banana/graphics/vertex.h"
#include "banana/graphics/shader.h"
#include "banana/engine.h"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/glm.hpp"
#include "random"
#include <memory>

namespace banana::graphics
{
    
	class Rectangle : public Gameobject
	{
		public:
            Rectangle()
            {
                Initiate();
            }
            Rectangle(banana::graphics::Rectangle& other)
            {
                this->Collider_radius = other.Collider_radius;
                this->color = other.color;
                this->IsRigid = other.IsRigid;
                this->mass = other.mass;
                this->transform = other.transform;
                this->velocity = other.velocity;
                this->name = other.name;
                this->use_texture = other.use_texture;
                this->tex_path = other.tex_path;

                // Generate a random integer between 0 and 999
                 

                // Convert the random integer to a string
                
                this->tex_name = other.tex_name;
                this->Initiate();
            }
            Rectangle(std::string Name, bool isrigid)
                :Gameobject(Name, isrigid){


                Initiate();

            }
            json Serialize() override
            {
                json j = Gameobject::Serialize();

                // Serialize Circle-specific data
                j["type"] = "rectangle";
                
                return j;
            }
            void Deserialize(const json& j) override
            {
                // Deserialize common data for Gameobject class
                Gameobject::Deserialize(j);

                // Deserialize Circle-specific data
                
            }
            void update()
            {
               
                glm::mat4 model = glm::mat4(1.f);
                model = glm::translate(model, { transform.position.x, transform.position.y, 0 });
                model = glm::scale(model, { transform.scale.x,transform.scale.y, 1 });
                mShader->setUniformFloat4("objectcolor", color.x, color.y, color.z, color.w);
                mShader->setUniformMat4("model", model);
            }
            void render()
            {
                if (use_texture)
                {
                    banana::Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderTexturedVertexArray, va, m_texture, mShader));
                }
                else
                {
                    banana::Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderVertexArray, va, mShader));
                }

            }
            void SetTexturePath(std::string &path)
            {
                tex_path = path;
                m_texture = std::make_shared<banana::graphics::Texture>(tex_name, tex_path, tex_unit);
            }
            void Initiate() override
            {
              
                    tex_name = "tex";
                    if(tex_path.size()==0)
                         tex_path = "C:/Users/Jaydip/Downloads/banana-Editor-main/banana-Editor-main/bananaEditor/res/pirate.png";
                    
                    m_texture = std::make_shared<banana::graphics::Texture>(tex_name, tex_path, tex_unit);
                    BANANA_CRETE_VERTEX_BUFFER(vb, float);

                    static std::vector<float> Vertex1{ 0.5f,  0.5f,  0.f,1.0f,1.0f };
                    static std::vector<float> Vertex2{ 0.5f, -0.5f,  0.f,1.0f,0.f };
                    static std::vector<float> Vertex3{ -0.5f, -0.5f, 0.f,0.f ,0.f };
                    static std::vector<float> Vertex4{ -0.5f,  0.5f, 0.f,0.f ,1.0f };
                    static std::vector<uint32_t> Elements
                    {
                        0, 3, 1,
                        1, 3, 2
                    };
                    vb->pushVertex(Vertex1);
                    vb->pushVertex(Vertex2);
                    vb->pushVertex(Vertex3);
                    vb->pushVertex(Vertex4);
                    vb->setLayout({ 3,2 });


                    va = std::make_shared<banana::graphics::VertexArray>();
                    va->pushBuffer(std::move(vb));
                    va->setElements(Elements); //watchout for elemet array
                    va->upload();
                    if(use_texture)
                    { 
                        mShader = std::make_shared<banana::graphics::Shader>(VertexShader, FragmentShader); 
                    }
                   else
                    {
                        mShader = std::make_shared<banana::graphics::Shader>(VertexShader, FragmentShaderCircle);
                    }
                   
                
               
            }
	};
}