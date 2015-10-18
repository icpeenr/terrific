#include "Game.h"
#include "Common.h"

#include "config.h"

#include "KeyboardManager.h"

struct GameState
{
	GameState() : m_Window(sf::VideoMode(600, 400), "Terrific Engine", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 ))
	{	
		m_Window.setVerticalSyncEnabled(true);
		m_Window.setFramerateLimit(60);
	    // We have to do this because we don't use SFML to draw.
		m_Window.resetGLStates();
		m_Window.setActive(true);

	};

	sf::RenderWindow m_Window;
	sfg::SFGUI m_Sfgui;
	sfg::Desktop m_SfguiDesktop;
};

Game::Game() : 	m_Entities(m_Events), m_Systems(m_Entities, m_Events),
m_Window(sf::VideoMode(600, 400), "Terrific Engine", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 )),
m_SfguiDesktop(),
m_Clock()
{
	m_fElapsedTime = 0.0f;
	m_bIsExiting = false;
	
	m_pContentManager = ContentManager::GetInstance();
	/*
	m_pActorManager = ActorManager::GetInstance();
	m_pCameraManager = CameraManager::GetInstance();
	m_pGraphicsDeviceManager = GraphicsDeviceManager::GetInstance();*/
}

Game::~Game()
{
	delete m_pContentManager;
	/*
	delete m_pActorManager;
	delete m_pCameraManager;*/
}

void Game::Initialize()
{
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
#if 0
  auto mode = modes[41]; 
  auto style = (sf::Style::Default | sf::Style::Titlebar); //sf::Style::Fullscreen;
#else
//Fullscreen
  auto mode = modes[36]; 
  //auto style = sf::Style::Fullscreen;  
#endif
  cout << "Width: " << mode.width << " Height: " << mode.height << " Bpp: " << mode.bitsPerPixel << endl;

  //auto style = (sf::Style::Titlebar | sf::Style::Close);
  //auto contextSettings = sf::ContextSettings( 16, 0, 0, 2, 1 );
  //m_Window = sf::RenderWindow(mode, "Terrific Engine", style, contextSettings);
  //m_Window(sf::VideoMode(600, 400), "Terrific Engine", sf::Style::Default, sf::ContextSettings( 16, 0, 0, 2, 1 ));

	//prepare OpenGL surface for HSR
  glClearDepth(1.f);
  glClearColor(0.3f, 0.3f, 0.3f, 0.f);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);

    //// Setup a perspective projection & Camera position
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
    gluPerspective(90.f, 1.f, 1.f, 300.0f);//fov, aspect, zNear, zFar

    cout << "Version " << terrific_VERSION_MINOR << "." << terrific_VERSION_MAJOR << endl;

    //b2World B2World(b2Vec2(0.0f, -9.8f));
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
    m_SfguiDesktop.Add( sfgui_window );

  // Update an initial time to construct the GUI before drawing begins.
  // This makes sure that there are no frames in which no GUI is visible.

	/*
	WindowManager::Initialize();
	m_pGraphicsDeviceManager->Initialize();
	m_pCameraManager->Initialize();*/
}

void Game::LoadContent()
{
	m_pContentManager->LoadContent();

	//TODO: Maybe load in ContentManager
	//      m_Font(m_pContentManager->LoadFont("Retro Computer_DEMO.ttf"))
	if (m_Font.loadFromFile( m_pContentManager->GetRootDirectory() + "/fonts/Retro Computer_DEMO.ttf")) {
	} else if (m_Font.loadFromFile( m_pContentManager->GetRootDirectory() +"/fonts/LiberationSans-Regular.ttf")) {
	} else {
		cerr << "error: failed to load Fonts" << endl;
	}
	m_SfguiDesktop.GetEngine().GetResourceManager().AddFont( "custom_font", std::make_shared<sf::Font>(m_Font));

	m_SfguiDesktop.SetProperties(m_pContentManager->GetFileContents("data/styles/sfguiDesktop.css"));

	m_SfguiDesktop.Update( 0.f );
}

void Game::Update(float elapsedTime)
{
	
	m_fElapsedTime = m_Clock.restart().asMicroseconds();//.asSeconds();//(float)glfwGetTime();

	KeyboardManager* keyboardManager = KeyboardManager::GetInstance();
	//MouseManager* mouseManager = MouseManager::GetInstance();
	//JoystickManager* joystickManager = JoystickManager::GetInstance();

	keyboardManager->Update();
	//mouseManager->Update();
	//joystickManager->Update();

	//m_pCameraManager->Update(elapsedTime);

	sf::Event event;
	while (m_Window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
			m_Window.close();
			case sf::Event::Resized:
			glViewport(0, 0, event.size.width, event.size.height); 
			break;
			/*
			case sf::Event::KeyPressed:
			keyboardManager->ProcessEvent(event);
			break;
			*/
			default:
			m_SfguiDesktop.HandleEvent(event);
			break;
		}
	}

	static sf::Clock updateClock;
	auto microseconds = updateClock.getElapsedTime().asMicroseconds();
	if( microseconds >= 5000 ) {
		m_SfguiDesktop.Update( static_cast<float>( microseconds ) / 1000000.f );
		updateClock.restart();
	}
}

void Game::Draw()
{
	/*
	GuiManager::GetInstance()->Draw();

	glfwSwapBuffers();*/

	// Apply some transformations for the cube
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.f, 0.f, -200.f);
	
	local_persist real32 angle = 0.0f;
	auto keyboardManager = KeyboardManager::GetInstance();
	auto rotate = keyboardManager->IsKeyHeld(sf::Keyboard::Space);
	
	
	if(rotate)
		angle=GetElapsedTime() / 1000.0f;
	
	glRotatef(angle * 50, 1.f, 0.f, 0.f);
	glRotatef(angle * 30, 0.f, 1.f, 0.f);
	glRotatef(angle * 90, 0.f, 0.f, 1.f);



		//Draw a cube
        glBegin(GL_QUADS);//draw some squares
        glColor3i(0,1,1);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f(-50.f,  50.f, -50.f);
        glVertex3f( 50.f,  50.f, -50.f);
        glVertex3f( 50.f, -50.f, -50.f);

        glColor3f(0,0,1);
        glVertex3f(-50.f, -50.f, 50.f);
        glVertex3f(-50.f,  50.f, 50.f);
        glVertex3f( 50.f,  50.f, 50.f);
        glVertex3f( 50.f, -50.f, 50.f);

        glColor3f(1,0,1);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f(-50.f,  50.f, -50.f);
        glVertex3f(-50.f,  50.f,  50.f);
        glVertex3f(-50.f, -50.f,  50.f);

        glColor3f(0,1,0);
        glVertex3f(50.f, -50.f, -50.f);
        glVertex3f(50.f,  50.f, -50.f);
        glVertex3f(50.f,  50.f,  50.f);
        glVertex3f(50.f, -50.f,  50.f);

        glColor3f(1,1,0);
        glVertex3f(-50.f, -50.f,  50.f);
        glVertex3f(-50.f, -50.f, -50.f);
        glVertex3f( 50.f, -50.f, -50.f);
        glVertex3f( 50.f, -50.f,  50.f);

        glColor3f(1,0,0);
        glVertex3f(-50.f, 50.f,  50.f);
        glVertex3f(-50.f, 50.f, -50.f);
        glVertex3f( 50.f, 50.f, -50.f);
        glVertex3f( 50.f, 50.f,  50.f);

        glEnd();

        m_Window.setActive( true );

    // Draw the GUI
        m_Sfgui.Display( m_Window );

    // Update the window
        m_Window.display();
    }

    void Game::UnloadContent()
    {
    }

    void Game::Exit()
    {
    	m_bIsExiting = true;
    }

    const float Game::GetElapsedTime() const
    {
    	return m_fElapsedTime;
    }

    const bool Game::IsExiting() const
    {
    	return m_bIsExiting;
    }

    ContentManager* Game::GetContentManager() const
    {
    	return m_pContentManager;
    }
/*
ActorManager* Game::GetActorManager() const
{
return m_pActorManager;
}

CameraManager* Game::GetCameraManager() const
{
return m_pCameraManager;
}*/