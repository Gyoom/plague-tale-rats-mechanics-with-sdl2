
#include "Rat.h"

Rat::Rat(Body* body, const char* textureFileName) : Entity(body, textureFileName)
{
	type = RAT;
}

Rat::~Rat()
{
	
}
