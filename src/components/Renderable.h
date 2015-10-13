#pragma once

namespace tcp
{
	struct Renderable {
		explicit Renderable(std::unique_ptr<sf::Shape> shape) : shape(std::move(shape)) {}

		std::unique_ptr<sf::Shape> shape;
	};

}