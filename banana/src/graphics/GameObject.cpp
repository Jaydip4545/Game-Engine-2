#include "banana/graphics/GameObject.h"

banana::graphics::Gameobject::Gameobject(std::string name, bool IsRigid)
{
	this->name = name;
	this->IsRigid = IsRigid;
	transform.position = { 0,0 };
	transform.scale = { 1,1 };
	transform.angle = {0,0};
	Collider_radius = 0.1f;
	velocity = { 0.0001f,0.0001f };
	Initiate();
	
}
json banana::graphics::Gameobject::Serialize()
{
	
	// Serialize common data for Gameobject class
	json j;
	j["Name"] = name;
	j["IsRigid"] = IsRigid;
	j["Velocity"] = { velocity.x, velocity.y };
	j["Color"] = { color.r, color.g, color.b, color.a };
	j["Position"] = { transform.position.x,transform.position.y };
	j["Scale"] = { transform.scale.x,transform.scale.y };
	j["Angle"] = { transform.angle.x,transform.angle.y };
	j["Mass"] = mass;
	
	
	return j;
}
void banana::graphics::Gameobject::Deserialize(const json& j)
{
	     
		// Deserialize common data for Gameobject class
		name = j["Name"];
		IsRigid = j["IsRigid"];
		color = { j["Color"][0], j["Color"][1], j["Color"][2], j["Color"][3] };
		transform.position = { j["Position"][0],j["Position"][1] };
		transform.scale = { j["Scale"][0],j["Scale"][1] };
		transform.angle = { j["Angle"][0],j["Angle"][1] };
		velocity = { j["Velocity"][0],j["Velocity"][1] };
		mass = j["Mass"].get<float>();;
	
}
banana::graphics::Gameobject::Gameobject()
{
	transform.position = { 0,0 };
	transform.scale = { 1,1 };
	transform.angle = { 0,0 };
	Collider_radius = 0.1f;
	velocity = { 0.0001f,0.0001f };
}
banana::graphics::Gameobject::Gameobject(const Gameobject& other)
{
	this->name = other.name;
	this->transform = other.transform;
	this->IsRigid = other.IsRigid;
	this->velocity = other.velocity;
	this->color = other.color;
	this->Collider_radius = other.Collider_radius;
	this->mass = other.mass;
	Initiate();
	// Copy other data members as needed/
}
banana::graphics::Gameobject::~Gameobject()
{
}

void banana::graphics::Gameobject::Initiate()
{
	
}

void banana::graphics::Gameobject::SetTexturePath(std::string& path)
{
}



void banana::graphics::Gameobject::render()
{

}
void banana::graphics::Gameobject::update()
{

}
