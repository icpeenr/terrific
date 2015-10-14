#include "../components/Body.h"
#include "../components/Particle.h"

namespace tsm
{

	class ParticleRender : public ex::System<ParticleRender> {
	public:
		explicit ParticleRender(sf::RenderTarget &target) : target(target) {}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			sf::VertexArray vertices(sf::Quads);
			es.each<tcp::Particle, tcp::Body>([&vertices](ex::Entity entity, tcp::Particle &particle, tcp::Body &body) {
				const float r = particle.radius;
				vertices.append(sf::Vertex(body.position + sf::Vector2f(-r, -r), particle.colour));
				vertices.append(sf::Vertex(body.position + sf::Vector2f(r, -r), particle.colour));
				vertices.append(sf::Vertex(body.position + sf::Vector2f(r, r), particle.colour));
				vertices.append(sf::Vertex(body.position + sf::Vector2f(-r, r), particle.colour));
			});
			target.draw(vertices);
		}
	private:
		sf::RenderTarget &target;
	};
}