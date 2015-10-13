#pragma once

namespace tcp
{
	
	struct Collideable {
		explicit Collideable(float radius) : radius(radius) {}

		float radius;
	};

}