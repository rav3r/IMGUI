#include <SFML/Graphics.hpp>

#include "gfxlib.h"

#include "IMGUI.h"

sf::RenderWindow* gWindow = 0;
sf::Font* gFont = 0;

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	gWindow = &window;

	sf::Font font;
	gFont = &font;
	if(!font.loadFromFile("Amble-Bold.ttf"))
        	return EXIT_FAILURE;

	igContext gui;
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if(event.type == sf::Event::Closed)
				window.close();
		    if(event.type == sf::Event::MouseMoved)
		    {
		        gui.mouseX = event.mouseMove.x;
		        gui.mouseY = event.mouseMove.y;
		    }
		}

		window.clear(sf::Color(128, 128, 128, 255));
		
		if(gui.Button(GEN_NULL_ID, 100, 100, 100, 30, "Click me!")) ;

		window.display();

	}

	return 0;
}
