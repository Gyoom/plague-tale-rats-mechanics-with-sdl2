#include "Entity.h"
#include "Graphics.h"
#include "Body.h"

Entity::Entity(std::unique_ptr<Body> body, const char* textureFileName)
{
	this->body = std::move(body);
	this->body->SetTexture(textureFileName);
	type = ENTITY;
}

Entity::~Entity()
{
}

void Entity::Update(float dt)
{
	body->Update(dt);
}

void Entity::Render()
{
	body->Render();
}
