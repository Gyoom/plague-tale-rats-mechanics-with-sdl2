#include "Cell.h"

Cell::Cell(Vec2 worldPos, Vec2 logicalPos)
{
	this->_worldPos = worldPos;
	this->_logicalPos = logicalPos;
}

void Cell::ClearOccupancy()
{
	this->_occupancy = 0.0;
}





