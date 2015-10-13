

namespace tsm
{

// Render all Renderable entities and draw some informational text.
	class Render  :public ex::System<Render> {
	public:
		explicit Render(sf::RenderTarget &target, sf::Font &font) : target(target) {
			text.setFont(font);
			text.setPosition(sf::Vector2f(2, 2));
			text.setCharacterSize(18);
			text.setColor(sf::Color::White);
		}

		void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
			es.each<tcp::Body,tcp::Renderable>([this](ex::Entity entity, tcp::Body &body,tcp::Renderable &renderable) {
				renderable.shape->setPosition(body.position);
				renderable.shape->setRotation(body.rotation);
				target.draw(*renderable.shape.get());
			});
			last_update += dt;
			frame_count++;
			if (last_update >= 0.5) {
				std::ostringstream out;
				const double fps = frame_count / last_update;
				out << es.size() << " entities (" << static_cast<int>(fps) << " fps)";
				text.setString(out.str());
				last_update = 0.0;
				frame_count = 0.0;
			}
			target.draw(text);
		}

	private:
		double last_update = 0.0;
		double frame_count = 0.0;
		sf::RenderTarget &target;
		sf::Text text;
	};
}