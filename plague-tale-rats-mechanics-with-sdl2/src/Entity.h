#pragma once

#include <vector>  
#include <iostream>  
#include "Vec2.h"

class Body;

enum EntityType {
	ENTITY,
	RAT,
	BOID,
	PLAYER,
	LIGHT_SOURCE,
};

class Entity { 

public:
  std::unique_ptr<Body> body;  
  EntityType type;

  Entity() = default;
  Entity(std::unique_ptr<Body> body, const char* textureFileName);
  ~Entity();

  void virtual Update(float dt);

  void virtual Render();
;
};  
