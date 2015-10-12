#include "common.h"
#include "config.h"
#include "linux_terror.h"

float r(int a, float b = 0) {
  return static_cast<float>(std::rand() % (a * 1000) + b * 1000) / 1000.0;
}


struct Body {
  Body(const sf::Vector2f &position, const sf::Vector2f &direction, float rotationd = 0.0)
    : position(position), direction(direction), rotationd(rotationd) {}

  sf::Vector2f position;
  sf::Vector2f direction;
  float rotation = 0.0, rotationd;
};


struct Renderable {
  explicit Renderable(std::unique_ptr<sf::Shape> shape) : shape(std::move(shape)) {}

  std::unique_ptr<sf::Shape> shape;
};


struct Particle {
  explicit Particle(sf::Color colour, float radius, float duration)
      : colour(colour), radius(radius), alpha(colour.a), d(colour.a / duration) {}

  sf::Color colour;
  float radius, alpha, d;
};


struct Collideable {
  explicit Collideable(float radius) : radius(radius) {}

  float radius;
};


// Emitted when two entities collide.
struct CollisionEvent {
  CollisionEvent(ex::Entity left, ex::Entity right) : left(left), right(right) {}

  ex::Entity left, right;
};


class SpawnSystem : public ex::System<SpawnSystem> {
public:
  explicit SpawnSystem(sf::RenderTarget &target, int count) : size(target.getSize()), count(count) {}

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    int c = 0;
    ex::ComponentHandle<Collideable> collideable;
    es.each<Collideable>([&](ex::Entity entity, Collideable&) { ++c; });

    for (int i = 0; i < count - c; i++) {
      ex::Entity entity = es.create();

      // Mark as collideable (explosion particles will not be collideable).
      collideable = entity.assign<Collideable>(r(10, 5));

      // "Physical" attributes.
      entity.assign<Body>(
        sf::Vector2f(r(size.x), r(size.y)),
        sf::Vector2f(r(100, -50), r(100, -50)));

      // Shape to apply to entity.
      std::unique_ptr<sf::Shape> shape(new sf::CircleShape(collideable->radius));
      shape->setFillColor(sf::Color(r(128, 127), r(128, 127), r(128, 127)));
      shape->setOrigin(collideable->radius, collideable->radius);
      entity.assign<Renderable>(std::move(shape));
    }
  }

private:
  sf::Vector2u size;
  int count;
};


// Updates a body's position and rotation.
struct BodySystem : public ex::System<BodySystem> {
  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    es.each<Body>([dt](ex::Entity entity, Body &body) {
      body.position += body.direction * static_cast<float>(dt);
      body.rotation += body.rotationd * dt;
    });
  };
};


// Bounce bodies off the edge of the screen.
class BounceSystem : public ex::System<BounceSystem> {
public:
  explicit BounceSystem(sf::RenderTarget &target) : size(target.getSize()) {}

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    es.each<Body>([this](ex::Entity entity, Body &body) {
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


// Determines if two Collideable bodies have collided. If they have it emits a
// CollisionEvent. This is used by ExplosionSystem to create explosion
// particles, but it could be used by a SoundSystem to play an explosion
// sound, etc..
//
// Uses a fairly rudimentary 2D partition system, but performs reasonably well.
class CollisionSystem : public ex::System<CollisionSystem> {
  static const int PARTITIONS = 200;

  struct Candidate {
    sf::Vector2f position;
    float radius;
    ex::Entity entity;
  };

public:
  explicit CollisionSystem(sf::RenderTarget &target) : size(target.getSize()) {
    size.x = size.x / PARTITIONS + 1;
    size.y = size.y / PARTITIONS + 1;
  }

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    reset();
    collect(es);
    collide(events);
  };

private:
  std::vector<std::vector<Candidate>> grid;
  sf::Vector2u size;

  void reset() {
    grid.clear();
    grid.resize(size.x * size.y);
  }

  void collect(ex::EntityManager &entities) {
    entities.each<Body, Collideable>([this](ex::Entity entity, Body &body, Collideable &collideable) {
      unsigned int
          left = static_cast<int>(body.position.x - collideable.radius) / PARTITIONS,
          top = static_cast<int>(body.position.y - collideable.radius) / PARTITIONS,
          right = static_cast<int>(body.position.x + collideable.radius) / PARTITIONS,
          bottom = static_cast<int>(body.position.y + collideable.radius) / PARTITIONS;
        Candidate candidate {body.position, collideable.radius, entity};
        unsigned int slots[4] = {
          left + top * size.x,
          right + top * size.x,
          left  + bottom * size.x,
          right + bottom * size.x,
        };
        grid[slots[0]].push_back(candidate);
        if (slots[0] != slots[1]) grid[slots[1]].push_back(candidate);
        if (slots[1] != slots[2]) grid[slots[2]].push_back(candidate);
        if (slots[2] != slots[3]) grid[slots[3]].push_back(candidate);
    });
  }

  void collide(ex::EventManager &events) {
    for (const std::vector<Candidate> &candidates : grid) {
      for (const Candidate &left : candidates) {
        for (const Candidate &right : candidates) {
          if (left.entity == right.entity) continue;
          if (collided(left, right))
            events.emit<CollisionEvent>(left.entity, right.entity);
        }
      }
    }
  }

  float length(const sf::Vector2f &v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
  }

  bool collided(const Candidate &left, const Candidate &right) {
    return length(left.position - right.position) < left.radius + right.radius;
  }
};


class ParticleSystem : public ex::System<ParticleSystem> {
public:
  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    es.each<Particle>([dt](ex::Entity entity, Particle &particle) {
      particle.alpha -= particle.d * dt;
      if (particle.alpha <= 0) {
        entity.destroy();
      } else {
        particle.colour.a = particle.alpha;
      }
    });
  }
};


class ParticleRenderSystem : public ex::System<ParticleRenderSystem> {
public:
  explicit ParticleRenderSystem(sf::RenderTarget &target) : target(target) {}

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    sf::VertexArray vertices(sf::Quads);
    es.each<Particle, Body>([&vertices](ex::Entity entity, Particle &particle, Body &body) {
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


// For any two colliding bodies, destroys the bodies and emits a bunch of bodgy explosion particles.
class ExplosionSystem : public ex::System<ExplosionSystem>, public ex::Receiver<ExplosionSystem> {
public:
  void configure(ex::EventManager &events) override {
    events.subscribe<CollisionEvent>(*this);
  }

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    for (ex::Entity entity : collided) {
      emit_particles(es, entity);
      entity.destroy();
    }
    collided.clear();
  }

  void emit_particles(ex::EntityManager &es, ex::Entity entity) {
    ex::ComponentHandle<Body> body = entity.component<Body>();
    ex::ComponentHandle<Renderable> renderable = entity.component<Renderable>();
    ex::ComponentHandle<Collideable> collideable = entity.component<Collideable>();
    sf::Color colour = renderable->shape->getFillColor();
    colour.a = 200;

    float area = (M_PI * collideable->radius * collideable->radius) / 3.0;
    for (int i = 0; i < area; i++) {
      ex::Entity particle = es.create();
      
      float rotationd = r(720, 180);
      if (std::rand() % 2 == 0) rotationd = -rotationd;

      float offset = r(collideable->radius, 1);
      float angle = r(360) * M_PI / 180.0;
      particle.assign<Body>(
        body->position + sf::Vector2f(offset * cos(angle), offset * sin(angle)),
        body->direction + sf::Vector2f(offset * 2 * cos(angle), offset * 2 * sin(angle)),
        rotationd);

      float radius = r(3, 1);
      particle.assign<Particle>(colour, radius, radius / 2);
    }
  }

  void receive(const CollisionEvent &collision) {
    // Events are immutable, so we can't destroy the entities here. We defer
    // the work until the update loop.
    collided.insert(collision.left);
    collided.insert(collision.right);
  }

private:
  std::unordered_set<ex::Entity> collided;
};


// Render all Renderable entities and draw some informational text.
class RenderSystem  :public ex::System<RenderSystem> {
public:
  explicit RenderSystem(sf::RenderTarget &target, sf::Font &font) : target(target) {
    text.setFont(font);
    text.setPosition(sf::Vector2f(2, 2));
    text.setCharacterSize(18);
    text.setColor(sf::Color::White);
  }

  void update(ex::EntityManager &es, ex::EventManager &events, ex::TimeDelta dt) override {
    es.each<Body, Renderable>([this](ex::Entity entity, Body &body, Renderable &renderable) {
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


class Game : public ex::EntityX {
public:
  explicit Game(sf::RenderTarget &target, sf::Font &font) {
    systems.add<SpawnSystem>(target, 100);
    systems.add<BodySystem>();
    systems.add<BounceSystem>(target);
    systems.add<CollisionSystem>(target);
    systems.add<ExplosionSystem>();
    systems.add<ParticleSystem>();
    systems.add<RenderSystem>(target, font);
    systems.add<ParticleRenderSystem>(target);
    systems.configure();
  }

  void update(ex::TimeDelta dt) {
    systems.update<BodySystem>(dt);
    systems.update<BounceSystem>(dt);
    static sf::Clock updateClock;
    auto microseconds = updateClock.getElapsedTime().asMicroseconds();
    // Only update every 5ms
    if( microseconds > 5000 ) {
      updateClock.restart();
      systems.update<SpawnSystem>(dt);
      systems.update<CollisionSystem>(dt);
      systems.update<ExplosionSystem>(dt);
      systems.update<ParticleSystem>(dt);
    }
    systems.update<RenderSystem>(dt);
    systems.update<ParticleRenderSystem>(dt);
  }
};



int main() {
  std::srand(std::time(nullptr));

  sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "EntityX Example", sf::Style::Fullscreen);
  //sf::RenderWindow window( sf::VideoMode( 800, 600, 32 ), "SFGUI test", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 ));
  
  sfg::SFGUI sfgui;
  sfg::Desktop sfguiDesktop;
  
  std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
#if 0
    for (std::size_t i = 0; i < modes.size(); ++i)
    {
      sf::VideoMode mode = modes[i];    
      std::cout << "Mode #" << i << ": "
      << mode.width << "x" << mode.height << " - "
      << mode.bitsPerPixel << " bpp" << std::endl;
    }
#endif

    //uint8 defaultStyle = (sf::Style::Titlebar | sf::Style::Close);
    auto mode = modes[17];

    cout << "Width: " << mode.width << " Height: " << mode.height << " " << endl;
        

    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);
    // We have to do this because we don't use SFML to draw.
    window.resetGLStates();
    window.setActive(true);

    //window->setPosition(sf::Vector2i(1100, 100)); 
    //GameState->Font.loadFromFile("../data/fonts/Retro Computer_DEMO.ttf");
  
    sf::Font font;
    if (!font.loadFromFile("../data/font/LiberationSans-Regular.ttf")) {
      cerr << "error: failed to load LiberationSans-Regular.ttf" << endl;
      return 1;
    }

    
    sfguiDesktop.GetEngine().GetResourceManager().AddFont( "custom_font", std::make_shared<sf::Font>(font));    
    //desktop.SetProperty( "*", "FontName",  "custom_font" );
    sfguiDesktop.SetProperties(
      "* {"
      "   FontName: custom_font;"
      "   FontSize: 18;"
      "}"
      "Window {"
      "   BackgroundColor: #5c5c5c55;"
      "   BorderColor: #5c5c5cff;"
      "   TitleBackgroundColor: #5c5c5cff;"
      "}"
      "Button {"
      "   BackgroundColor: #444444ff;"
      "   FontSize: 14;"
      "}"
      "Window#inventory > * > * > Button {"
      "   BackgroundColor: #202020fa;"
      "}"
      );

  cout << "Version " << terrific_VERSION_MINOR << "." << terrific_VERSION_MAJOR << endl;

  b2World B2World(b2Vec2(0.0f, -9.8f));

  Game game(window, font);

  //
  // Create our main SFGUI window
  auto sfgui_window = sfg::Window::Create();
  sfgui_window->SetTitle( "Title" );

  // Since only being able to add one widget to a window is very limiting
  // there are Box widgets. They are a subclass of the Container class and
  // can contain an unlimited amount of child widgets. Not only that, they
  // also have the ability to lay out your widgets nicely.

  // Create the box.
  // For layout purposes we must specify in what direction new widgets
  // should be added, horizontally or vertically.
  auto box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL );

  auto button1 = sfg::Button::Create();
  auto button2 = sfg::Button::Create();
  auto label = sfg::Label::Create();

  button1->SetLabel( "Foo" );
  button2->SetLabel( "Bar" );
  label->SetText( "Baz" );

  // To add our widgets to the box we use the Pack() method instead of the
  // Add() method. This makes sure the widgets are added and layed out
  // properly in the box.
  box->Pack( button1 );
  box->Pack( label );
  box->Pack( button2 );

  // Just as with the window we can set the spacing between widgets
  // withing a box.
  box->SetSpacing( 5.f );

  // Finally we add our box to the window as it's only child.
  // Notice that we don't have to add the children of a box to it's parent
  // Because all children and grandchildren and .... are automatically
  // considered descendents of the parent.
  sfgui_window->Add( box );
  sfguiDesktop.Add( sfgui_window );

  // Update an initial time to construct the GUI before drawing begins.
  // This makes sure that there are no frames in which no GUI is visible.
  sfguiDesktop.Update( 0.f );

  sf::Clock clock;
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
        case sf::Event::KeyPressed:
          window.close();
          break;

        default:
          sfguiDesktop.HandleEvent(event);
          break;
      }
    }


    window.clear();
    
    static sf::Clock updateClock;
    auto microseconds = updateClock.getElapsedTime().asMicroseconds();
    // Only update every 5ms
    // Update the GUI every 5ms
    if( microseconds >= 5000 ) {
      // Update() takes the elapsed time in seconds.
      sfguiDesktop.Update( static_cast<float>( microseconds ) / 1000000.f );
      updateClock.restart();
    }
    sf::Time elapsed = clock.restart();
    
    B2World.Step(elapsed.asSeconds(), 6, 2);
    game.update(elapsed.asSeconds());
    
    // This is important.
    window.setActive( true );

    // Draw the GUI
    sfgui.Display( window );

    // Update the window
    window.display();
  }
}
