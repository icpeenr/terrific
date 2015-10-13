#include <dlfcn.h>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "config.h"

int main(int argc, char **argv)
{
// Create the main window
	sf::Window App(sf::VideoMode(640, 480, 32), "SFML Window");

    // Create a clock for measuring the time elapsed
	sf::Clock Clock;

    // Set the color and depth clear values
	glClearDepth(1.f);
	glClearColor(0.f, 0.f, 0.f, 0.f);

    // Enable Z-buffer read and write
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);

    // Setup a perspective projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.f, 1.f, 1.f, 500.f);

	std::cout << "Version " << terrific_VERSION_MINOR << "." << terrific_VERSION_MAJOR << std::endl;

    // Start the game loop
	while (App.IsOpened())
	{
        // Process events
		sf::Event Event;
		while (App.GetEvent(Event))
		{
            // Close window : exit
			if (Event.Type == sf::Event::Closed)
				App.Close();

            // Escape key : exit
			if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
				App.Close();

            // Resize event : adjust viewport
			if (Event.Type == sf::Event::Resized)
				glViewport(0, 0, Event.Size.Width, Event.Size.Height);
		}

        // Set the active window before using OpenGL commands
        // It's useless here because the active window is always the same,
        // but don't forget it if you use multiple windows
		App.SetActive();

        // Clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Apply some transformations
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glTranslatef(0.f, 0.f, -200.f);
		glRotatef(Clock.GetElapsedTime() * 50, 1.f, 0.f, 0.f);
		glRotatef(Clock.GetElapsedTime() * 30, 0.f, 1.f, 0.f);
		glRotatef(Clock.GetElapsedTime() * 90, 0.f, 0.f, 1.f);

        // Draw a cube
		glBegin(GL_QUADS);

		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-50.f, -50.f, -50.f);
		glVertex3f(-50.f,  50.f, -50.f);
		glVertex3f( 50.f,  50.f, -50.f);
		glVertex3f( 50.f, -50.f, -50.f);

		glColor3f(1.f, 0.f, 0.f);
		glVertex3f(-50.f, -50.f, 50.f);
		glVertex3f(-50.f,  50.f, 50.f);
		glVertex3f( 50.f,  50.f, 50.f);
		glVertex3f( 50.f, -50.f, 50.f);

		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(-50.f, -50.f, -50.f);
		glVertex3f(-50.f,  50.f, -50.f);
		glVertex3f(-50.f,  50.f,  50.f);
		glVertex3f(-50.f, -50.f,  50.f);

		glColor3f(0.f, 1.f, 0.f);
		glVertex3f(50.f, -50.f, -50.f);
		glVertex3f(50.f,  50.f, -50.f);
		glVertex3f(50.f,  50.f,  50.f);
		glVertex3f(50.f, -50.f,  50.f);

		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(-50.f, -50.f,  50.f);
		glVertex3f(-50.f, -50.f, -50.f);
		glVertex3f( 50.f, -50.f, -50.f);
		glVertex3f( 50.f, -50.f,  50.f);

		glColor3f(0.f, 0.f, 1.f);
		glVertex3f(-50.f, 50.f,  50.f);
		glVertex3f(-50.f, 50.f, -50.f);
		glVertex3f( 50.f, 50.f, -50.f);
		glVertex3f( 50.f, 50.f,  50.f);

		glEnd();

        // Finally, display the rendered frame on screen
		App.display();
	}
    // release resources...
	return 0;
}
