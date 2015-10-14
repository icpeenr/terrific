#pragma once

float r(int a, float b = 0) {
	return static_cast<float>(std::rand() % (a * 1000) + b * 1000) / 1000.0;
}

namespace tcp 
{

	struct Body {
		Body(const sf::Vector2f &position, const sf::Vector2f &direction, float rotationd = 0.0)
		: position(position), direction(direction), rotationd(rotationd) {}

		sf::Vector2f position;
		sf::Vector2f direction;
		float rotation = 0.0, rotationd;
	};

}