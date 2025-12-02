#pragma once

#include "Graphics.h"
#include "Vec2.h"
#include "Body.h"
#include <vector>  
#include <iostream>  


enum EntityType {
	ENTITY,
	RAT,
	PLAYER,
	LIGHT_SOURCE,
};

struct Entity {  
  Body* body;  
  EntityType type;

  Entity() = default;
  Entity(Body* body, const char* textureFileName);
  ~Entity();

  void virtual Update(float dt);
;
};  
