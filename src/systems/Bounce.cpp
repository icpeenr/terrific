#pragma once

namespace tsm
{
// Bounce bodies off the edge of the screen.
  class Bounce : public ex::System<Bounce> {
  public:
    explicit Bounce(sf::RenderTarget &target) : size(target.getSize()) {}

    void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
      es.each<tcp::Body>([this](ex::Entity entity, tcp::Body &body) {
        if (body.position.x + body.direction.x < 0 ||
          body.position.x + body.direction.x >= size.x)
          body.direction.x = -body.direction.x;
        if (body.position.y + body.direction.y < 0 ||
          body.position.y + body.direction.y >= size.y)
          body.direction.y = -body.direction.y;
      });
    }

  private:
    sf::Vector2u size;
  };
}