#pragma once

namespace tev 
{
	struct Collision {
		Collision(ex::Entity left, ex::Entity right) : left(left), right(right) {}

		ex::Entity left, right;
	};
}