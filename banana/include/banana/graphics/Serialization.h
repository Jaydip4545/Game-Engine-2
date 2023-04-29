#pragma once
#include"banana/graphics/GameObject.h"
#include "external/json.hpp"
#include <fstream>
#include "banana/graphics/GameObjects/Circle.h"
#include "banana/graphics/GameObjects/Rectangle.h"
#include "vector"

std::vector<banana::graphics::Gameobject*> Deserialize()
{
	std::vector<banana::graphics::Gameobject*> Objects;
	std::ifstream file("gameobjects.json");
	if (file.peek() == std::ifstream::traits_type::eof()) {
		BANANA_INFO("Empty Json file");
		return Objects;
	}
	json j;
	file >> j;

	for (const auto& objData : j)
	{
		std::string type = objData["type"];
		if (type == "circle")
		{
			auto circle = new banana::graphics::Circle();
			circle->Deserialize(objData);
			Objects.push_back(circle);
		}
		else if (type == "rectangle")
		{
			auto rectangle = new banana::graphics::Rectangle();
			rectangle->Deserialize(objData);
			Objects.push_back(rectangle);
		}
		// Add other classes as needed
	}
	return Objects;
}

void Serialize(std::vector<banana::graphics::Gameobject*> Objects)
{
	json jsonData; // Create a JSON object

	// Iterate through each Gameobject pointer and add its data to the JSON object
	for (int i = 0; i < Objects.size(); i++)
	{
		jsonData.push_back(Objects[i]->Serialize());
	}
	std::ofstream file("gameobjects.json"); // Create an output file stream
	file << jsonData.dump(4); // Write the JSON data to the file with indentation of 4 spaces
	file.close(); // Close the file stream

}