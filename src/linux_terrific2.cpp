#include "common.h"
#include "config.h"
#include "linux_terrific.h"


/*
#include "components/Body.h"

#include "components/Renderable.h"

#include "components/Particle.h"

#include "components/Collideable.h"

#include "events/Collision.h"
*/
#include "systems/Spawn.cpp"
#include "systems/Body.cpp"
#include "systems/Bounce.cpp"
#include "systems/Collision.cpp"
#include "systems/Explosion.cpp"
#include "systems/Particle.cpp"
#include "systems/Render.cpp"
#include "systems/ParticleRender.cpp"

class Game : public ex::EntityX {
public:
  explicit Game(sf::RenderTarget &target, sf::Font &font, b2World &B2World) {
    /*
    systems.add<tsm::Spawn>(target, (target.getSize().x * target.getSize().y) / 8000);
    systems.add<tsm::Body>();
    systems.add<tsm::Bounce>(target);
    systems.add<tsm::Collision>(target);
    systems.add<tsm::Explosion>();
    systems.add<tsm::Particle>();
    systems.add<tsm::Render>(target, font);
    systems.add<tsm::ParticleRender>(target);*/
    systems.configure();
  }

  void update(ex::TimeDelta dt) {
    /*
    systems.update<tsm::Body>(dt);
    systems.update<tsm::Bounce>(dt);
    //static sf::Clock updateClock;
    //auto microseconds = updateClock.getElapsedTime().asMicroseconds();
    // Only update every 5ms
    //if( microseconds > 5000 ) {
    //updateClock.restart();
    systems.update<tsm::Spawn>(dt);
    systems.update<tsm::Collision>(dt);
    systems.update<tsm::Explosion>(dt);
    systems.update<tsm::Particle>(dt);
    //}
    systems.update<tsm::Render>(dt);
    systems.update<tsm::ParticleRender>(dt);*/
  }
};

#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>

std::string get_file_contents(const char *filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (in)
  {
    std::ostringstream contents;
    contents << in.rdbuf();
    in.close();
    return(contents.str());
  }
  throw(errno);
}

int main() {
  std::srand(std::time(nullptr));

  
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

// Windowed
#if 1
  auto mode = modes[41]; 
  auto style = (sf::Style::Default | sf::Style::Titlebar); //sf::Style::Fullscreen;
#else
//Fullscreen
  auto mode = modes[36]; 
  auto style = sf::Style::Fullscreen;  
#endif
  cout << "Width: " << mode.width << " Height: " << mode.height << " Bpp: " << mode.bitsPerPixel << endl;

  //auto style = (sf::Style::Titlebar | sf::Style::Close);
  auto contextSettings = sf::ContextSettings( 16, 0, 0, 2, 1 );
  sf::RenderWindow window(mode, "EntityX Example", style, contextSettings);
  //sf::RenderWindow window( sf::VideoMode( 800, 600, 32 ), "SFGUI test", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 ));
  
  sfg::SFGUI sfgui;
  sfg::Desktop sfguiDesktop;

  window.setVerticalSyncEnabled(true);
  window.setFramerateLimit(60);
    // We have to do this because we don't use SFML to draw.
  window.resetGLStates();
  window.setActive(true);

  sf::Font font;
  if (font.loadFromFile("data/fonts/Retro Computer_DEMO.ttf")) {
  } else if (font.loadFromFile("data/fonts/LiberationSans-Regular.ttf")) {
  } else {
    cerr << "error: failed to load Fonts" << endl;
    cerr << "Exiting" << endl;
    return 1;
  }

  sfguiDesktop.GetEngine().GetResourceManager().AddFont( "custom_font", std::make_shared<sf::Font>(font));    
    //desktop.SetProperty( "*", "FontName",  "custom_font" );

  sfguiDesktop.SetProperties(get_file_contents("data/styles/sfguiDesktop.css"));

  cout << "Version " << terrific_VERSION_MINOR << "." << terrific_VERSION_MAJOR << endl;

  b2World B2World(b2Vec2(0.0f, -9.8f));
  
  Game game(window, font, B2World);

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
