#include "../components/Body.h"
#include "../components/Renderable.h"
#include "../components/Collideable.h"
#include "../components/Particle.h"

#include "../events/Collision.h"

namespace tsm
{

// For any two colliding bodies, destroys the bodies and emits a bunch of bodgy explosion particles.
	class Explosion : public ex::System<Explosion>, public ex::Receiver<Explosion> {
	public:
		void configure(ex::EventManager &events) override {
			events.subscribe<tev::Collision>(*this);
		}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			for (ex::Entity entity : collided) {
				emit_particles(es, entity);
				entity.destroy();
			}
			collided.clear();
		}

		void emit_particles(ex::EntityManager &es, ex::Entity entity) {
			ex::ComponentHandle<tcp::Body> body = entity.component<tcp::Body>();
			ex::ComponentHandle<tcp::Renderable> renderable = entity.component<tcp::Renderable>();
			ex::ComponentHandle<tcp::Collideable> collideable = entity.component<tcp::Collideable>();
			sf::Color colour = renderable->shape->getFillColor();
			colour.a = 200;

			float area = (M_PI * collideable->radius * collideable->radius) / 3.0;
			for (int i = 0; i < area; i++) {
				ex::Entity particle = es.create();
				
				float rotationd = r(720, 180);
				if (std::rand() % 2 == 0) rotationd = -rotationd;

				float offset = r(collideable->radius, 1);
				float angle = r(360) * M_PI / 180.0;
				particle.assign<tcp::Body>(
					body->position + sf::Vector2f(offset * cos(angle), offset * sin(angle)),
					body->direction + sf::Vector2f(offset * 2 * cos(angle), offset * 2 * sin(angle)),
					rotationd);

				float radius = r(3, 1);
				particle.assign<tcp::Particle>(colour, radius, radius / 2);
			}
		}

		void receive(const tev::Collision &collision) {
    // Events are immutable, so we can't destroy the entities here. We defer
    // the work until the update loop.
			collided.insert(collision.left);
			collided.insert(collision.right);
		}

	private:
		std::unordered_set<ex::Entity> collided;
	};
}