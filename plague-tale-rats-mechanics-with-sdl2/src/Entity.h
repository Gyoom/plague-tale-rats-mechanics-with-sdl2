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
