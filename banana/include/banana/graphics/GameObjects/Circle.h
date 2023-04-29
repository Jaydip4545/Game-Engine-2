#pragma once
#include "banana/graphics/Gameobject.h"
#include "banana/graphics/vertex.h"
#include "banana/graphics/shader.h"
#include "banana/engine.h"
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/glm.hpp"

#include <memory>

namespace banana::graphics
{
    static const char* FragmentShaderCircle = R"(
					#version 410 core
                    in vec2 uvs;
					out vec4 outColor;
                    uniform vec4 objectcolor;
					void main()
					{
						outColor = objectcolor;
					}
				)";
    
    class Circle:public Gameobject
    {
    private:
        float Radius;
        
        
      
    public:
        Circle()
        {
            Initiate();
        }
        Circle(const banana::graphics::Circle& other)
        {
            this->Collider_radius = other.Collider_radius;
            this->color = other.color;
            this->IsRigid = other.IsRigid;
            this->mass = other.mass;
            this->Radius = other.Radius;
            this->transform = other.transform;
            this->velocity = other.velocity;
           
            this->name = other.name;
            this->Initiate();
            
        }
        Circle(float radius, std::string Name, bool isrigid) 
         :Gameobject(Name, isrigid),Radius(radius) {

            
            Initiate();

        }
        json Serialize() override
        {
            json j = Gameobject::Serialize();

            // Serialize Circle-specific data
            j["type"] = "circle";
            j["Radius"] = Radius;
            return j;
        }
        void Deserialize(const json& j) override
        {
            // Deserialize common data for Gameobject class
            Gameobject::Deserialize(j);

            // Deserialize Circle-specific data
            Radius = j["Radius"];
            Initiate();
        }
        void update()
        {
           
            glm::mat4 model = glm::mat4(1.f);
            model = glm::translate(model, { transform.position.x,transform.position.y,0 });
            model = glm::scale(model, { transform.scale.x,transform.scale.y, 1 });
            mShader->setUniformMat4("model", model);
            mShader->setUniformFloat4("objectcolor", color.x, color.y, color.z, color.w);

        }
        void render()
        {
           
            
            banana::Engine::Instance().getRenderManager().submit(BANANA_SUBMIT_RC(RenderVertexArray, va, mShader));

        }
        void Initiate() override
        {
            BANANA_CRETE_VERTEX_BUFFER(vb, float);
           
            int numberOfSides = 200;
            const int numberOfVertices = 202;
            std::vector<float> data;
            float twicePi = 6.2830f;

            float circleVerticesX[numberOfVertices];
            float circleVerticesY[numberOfVertices];
            float circleVerticesZ[numberOfVertices];

            circleVerticesX[0] = transform.position.x;
            circleVerticesY[0] = transform.position.y;
            circleVerticesZ[0] = 0;

            for (int i = 1; i < numberOfVertices; i++)
            {
                circleVerticesX[i] = transform.position.x + (Radius * cos(i * twicePi / numberOfSides));
                circleVerticesY[i] = transform.position.y + (Radius * sin(i * twicePi / numberOfSides));
                circleVerticesZ[i] = 0;
            }

            std::vector<uint32_t> elements;

            for (int i = 0; i < numberOfVertices; i++)
            {
                data.clear();
                data.push_back(circleVerticesX[i]);
                data.push_back(circleVerticesY[i]);
                data.push_back(circleVerticesZ[i]);
                elements.push_back(0);
                elements.push_back((i+1)%numberOfVertices);
                elements.push_back((i+2) % numberOfVertices);
                vb->pushVertex(data);
            }

            vb->setLayout({ 3 });
            va = std::make_shared<banana::graphics::VertexArray>();
            va->pushBuffer(std::move(vb));
            va->setElements(elements); //watchout for elemet array
            va->upload();
            mShader = std::make_shared<banana::graphics::Shader>(VertexShader, FragmentShaderCircle);
        }
    };


}