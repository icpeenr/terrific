#pragma once

namespace tsm
{
// Updates a body's position and rotation.
	struct Body : public ex::System<Body> {
		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			es.each<tcp::Body>([dt](ex::Entity entity, tcp::Body &body) {
				body.position += body.direction * static_cast<float>(dt);
				body.rotation += body.rotationd * dt;
			});
		};
	};
}