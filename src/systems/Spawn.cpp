#include "../components/Body.h"
#include "../components/Collideable.h"
#include "../components/Renderable.h"

namespace tsm
{

	class Spawn : public ex::System<Spawn> {
	public:
		explicit Spawn(sf::RenderTarget &target, int count) : size(target.getSize()), count(count) {}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			int c = 0;
			ex::ComponentHandle<tcp::Collideable> collideable;
			es.each<tcp::Collideable>([&](ex::Entity entity, tcp::Collideable&) { ++c; });

			for (int i = 0; i < count - c; i++) {
				ex::Entity entity = es.create();

// Mark as collideable (explosion particles will not be collideable).
				collideable = entity.assign<tcp::Collideable>(r(10, 5));

// "Physical" attributes.
				entity.assign<tcp::Body>(
					sf::Vector2f(r(size.x), r(size.y)),
					sf::Vector2f(r(100, -50), r(100, -50)));

// Shape to apply to entity.
				std::unique_ptr<sf::Shape> shape(new sf::CircleShape(collideable->radius));
				shape->setFillColor(sf::Color(r(128, 127), r(128, 127), r(128, 127)));
				shape->setOrigin(collideable->radius, collideable->radius);
				entity.assign<tcp::Renderable>(std::move(shape));
			}
		}

	private:
		sf::Vector2u size;
		int count;
	};
}