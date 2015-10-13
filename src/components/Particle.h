#pragma once

namespace tcp 
{

	struct Particle {
		explicit Particle(sf::Color colour, float radius, float duration)
		: colour(colour), radius(radius), alpha(colour.a), d(colour.a / duration) {}

		sf::Color colour;
		float radius, alpha, d;
	};	
}