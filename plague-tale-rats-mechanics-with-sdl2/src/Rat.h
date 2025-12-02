#pragma once


#include "Entity.h"

struct Rat : public Entity  
{  
	Rat() = default;  
	Rat(Body* body, const char* textureFileName);  
	~Rat();
};

