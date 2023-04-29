#pragma once
#include "banana/graphics/GameObject.h"
namespace banana::physics
{
	float distance(banana::graphics::Gameobject*& p1, banana::graphics::Gameobject*& p2)
	{
		float dis = 0;
		dis = (float)(pow((p1->transform.position.x - p2->transform.position.x), 2) + pow((p1->transform.position.y - p2->transform.position.y), 2));
		return sqrt(dis);
	}
	void resolve_collision(banana::graphics::Gameobject*& a, banana::graphics::Gameobject*& b)
	{
		float m1;
		float m2;
		float v1_x = 0;
		float v1_y = 0;
		float v2_x = 0;
		float v2_y = 0;
		m1 = a->mass;
		v1_x = a->velocity.x;
		v1_y = a->velocity.y;
		m2 = b->mass;
		v2_x = b->velocity.x;
		v2_y = b->velocity.y;
		b->velocity.x = static_cast<float>(((m2 - m1) / (m1 + m2)) * v2_x + ((2.0 * m1) / (m1 + m2)) * v1_x);
		a->velocity.x = static_cast<float>(((m1 - m2) / (m1 + m2)) * v1_x + ((2.0 * m2) / (m1 + m2)) * v2_x);
		b->velocity.y = static_cast<float>(((m2 - m1) / (m1 + m2)) * v2_y + ((2.0 * m1) / (m1 + m2)) * v1_y);
		a->velocity.y = static_cast<float>(((m1 - m2) / (m1 + m2)) * v1_y + ((2.0 * m2) / (m1 + m2)) * v2_y);

	}

	void check_collision(std::vector<banana::graphics::Gameobject*> &objects)
	{
		if (objects.size() <= 1)
		{
			return;
		}
		for (int i = 0; i < objects.size()-1; i++)
		{
			for (int j = i + 1; j < objects.size(); j++)
			{
				if (distance(objects[i],objects[j])<=objects[i]->Collider_radius+objects[j]->Collider_radius)
				{
					resolve_collision(objects[i],objects[j]);
				}
			}
		}
	}
	
	void update(std::vector<banana::graphics::Gameobject*>& objects)
	{
		std::vector<banana::graphics::Gameobject*> Rigid_Objects;
		for (int i = 0; i < objects.size(); i++)
		{
			if (objects[i]->IsRigid)
			{
				Rigid_Objects.push_back(objects[i]);
			}
		}
		check_collision(Rigid_Objects);
		for (int i = 0; i < objects.size(); i++)
		{
			objects[i]->transform.position.x += objects[i]->velocity.x;
			objects[i]->transform.position.y += objects[i]->velocity.y;
		}
	}
	
}