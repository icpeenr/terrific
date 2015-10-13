#pragma once

namespace tsm
{

	class Particle : public ex::System<Particle> {
	public:
		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			es.each<tcp::Particle>([dt](ex::Entity entity, tcp::Particle &particle) {
				particle.alpha -= particle.d * dt;
				if (particle.alpha <= 0) {
					entity.destroy();
				} else {
					particle.colour.a = particle.alpha;
				}
			});
		}
	};
}