#include "Entity.h"
#include "Graphics.h"
#include "Body.h"

Entity::Entity(Body* body, const char* textureFileName)
{
	this->body = body;
	body->SetTexture(textureFileName);
	type = ENTITY;
}

Entity::~Entity()
{
	delete body;
}

void Entity::Update(float dt)
{
	body->Update(dt);
}
