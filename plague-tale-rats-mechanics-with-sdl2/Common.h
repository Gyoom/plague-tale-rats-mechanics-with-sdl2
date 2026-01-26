#pragma once

#include "src/Graphics.h"

class Common
{
public:
	static float lerp(float a, float b, float t) {
		return (a * (1.0 - t)) + (b * t);
	}

	static Uint32 lerpColor(Uint32 c1, Uint32 c2, float t)
	{
		uint8_t A1 = (c1 >> 24) & 0xFF;
		uint8_t R1 = (c1 >> 16) & 0xFF;
		uint8_t G1 = (c1 >> 8) & 0xFF;
		uint8_t B1 = c1 & 0xFF;


		uint8_t A2 = (c2 >> 24) & 0xFF;
		uint8_t R2 = (c2 >> 16) & 0xFF;
		uint8_t G2 = (c2 >> 8) & 0xFF;
		uint8_t B2 = c2 & 0xFF;


		uint8_t A = (uint8_t)lerp(A1, A2, t);
		uint8_t R = (uint8_t)lerp(R1, R2, t);
		uint8_t G = (uint8_t)lerp(G1, G2, t);
		uint8_t B = (uint8_t)lerp(B1, B2, t);


		return (A << 24) | (R << 16) | (G << 8) | B;
	}

	static float clamp(float value, float min, float max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

};

