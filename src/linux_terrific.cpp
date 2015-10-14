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
  explicit Game(sf::RenderTarget &target, sf::Font &font) {
    systems.add<tsm::Spawn>(target, (target.getSize().x * target.getSize().y) / 8000);
    systems.add<tsm::Body>();
    systems.add<tsm::Bounce>(target);
    systems.add<tsm::Collision>(target);
    systems.add<tsm::Explosion>();
    systems.add<tsm::Particle>();
    systems.add<tsm::Render>(target, font);
    systems.add<tsm::ParticleRender>(target);
    systems.configure();
  }

  void update(ex::TimeDelta dt) {
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
    systems.update<tsm::ParticleRender>(dt);
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

inline uint32
RoundReal32ToUInt32(real32 Real32)
{
  uint32 Result = (uint32)roundf(Real32);
  return(Result);
}

void
CreateGameInventory(std::shared_ptr<sfg::Window> Window, real32 Width, real32 Height)
{


    //real32 Width  = Window->GetAllocation().left;
    //real32 Height = Window->GetAllocation().top;
  real32 SlotSize = 42;

  auto InventorySize = sf::Vector2f(Width*0.3f, Height*0.3f);
  uint32 SlotsX = RoundReal32ToUInt32(InventorySize.x / SlotSize);
  uint32 SlotsY = RoundReal32ToUInt32(InventorySize.y / SlotSize);

  Window->RemoveAll();
  Window->SetPosition( sf::Vector2f(Width*0.015f, Height*0.015f) );
  Window->SetRequisition( InventorySize );
  Window->SetId("inventory");

  auto Label = sfg::Label::Create();
  Label->SetText("Inventory!");
    //Box->Pack(Label);

  auto Boxes = sfg::Box::Create(sfg::Box::Orientation::VERTICAL, 5.f);
  Window->Add(Boxes);



  char CharBuffer[256];

  for(uint32 Row = 0;
    Row <= SlotsY-1;
    Row++)
  {
    auto Box = sfg::Box::Create( sfg::Box::Orientation::HORIZONTAL, 5.f );
    Boxes->Pack(Box);
    for(uint32 Column = 0;
      Column <= SlotsX-1;
      Column++)
    {
      sprintf(CharBuffer, "Item %d", Row+Column);
      auto Slot = sfg::Button::Create();
            //Slot->SetLabel(CharBuffer);
      Slot->SetState(sfg::Widget::State::INSENSITIVE);
      Slot->SetRequisition(sf::Vector2f(SlotSize, SlotSize));
      Box->Pack(Slot);
    }
  }
}

void
CreateGameUI(sfg::Desktop &Desktop, real32 Width, real32 Height)
{
  auto Window = sfg::Window::Create( sfg::Window::Style::TITLEBAR | sfg::Window::Style::BACKGROUND | sfg::Window::Style::RESIZE | sfg::Window::Style::CLOSE);
  Window->SetTitle("Inventory");

  Desktop.Add(Window);
  CreateGameInventory(Window, Width, Height);
    //CreateMainMenu(Desktop, Width, Height);

    //Window->GetSignal(sfg::Widget::OnSizeAllocate).Connect(std::bind(&CreateGameInventory, Window->GetId(), 800, 600));


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

    //window->setPosition(sf::Vector2i(1100, 100)); 
    //GameState->Font.loadFromFile("data/fonts/Retro Computer_DEMO.ttf");
#if 0
  sf::Font font;
  if (!font.loadFromFile("data/font/LiberationSans-Regular.ttf")) {
    cerr << "error: failed to load LiberationSans-Regular.ttf" << endl;
    return 1;
  }
#else
  sf::Font font;
  if (font.loadFromFile("data/fonts/Retro Computer_DEMO.ttf")) {
  } else if (font.loadFromFile("data/fonts/LiberationSans-Regular.ttf")) {
  } else {
    cerr << "error: failed to load Fonts" << endl;
    cerr << "Exiting" << endl;
    return 1;
  }
#endif

  sfguiDesktop.GetEngine().GetResourceManager().AddFont( "custom_font", std::make_shared<sf::Font>(font));    
    //desktop.SetProperty( "*", "FontName",  "custom_font" );

#if 1
  sfguiDesktop.SetProperties(get_file_contents("data/styles/sfguiDesktop.css"));
#else
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
#endif

cout << "Version " << terrific_VERSION_MINOR << "." << terrific_VERSION_MAJOR << endl;

    //b2World B2World(b2Vec2(0.0f, -9.8f));
//CreateGameUI(sfguiDesktop, mode.width, mode.height);
Game game(window, font);

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

      //B2World.Step(elapsed.asSeconds(), 6, 2);
  game.update(elapsed.asSeconds());

    // This is important.
  window.setActive( true );

    // Draw the GUI
  sfgui.Display( window );

    // Update the window
  window.display();
}
}
